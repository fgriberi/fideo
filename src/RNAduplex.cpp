/*
 * @file     RNAduplex.cpp
 * @brief    RNAduplex is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAduplex implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date October 26, 2012, 7:48 PM
 *
 * Copyright (C) 2012 Franco Riberi, FuDePAN
 *
 * This file is part of fideo.
 *
 * fideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * fideo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <etilico/etilico.h>
#include "fideo/IHybridize.h"

namespace fideo
{

using namespace mili;

/** @brief RNAduplex is an implementation of IHybridize interface that use Vienna package
 *
 */
class RNAduplex : public IHybridize
{
private:

    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

    /** @brief Class that allows parsing the body of a file
     *
     */
    class BodyParser
    {
    public:

        /** @brief Parse the line and get the value dG
         *
         * @param line: line to parser
         * @return void
         */
        void parse(std::string& line);

        Fe dG; /// free energy

    private:

        /** @brief Represents the columns of the file to parse
         *
         */
        enum Columns
        {
            ColRNAResults,
            ColIndiceIJ,
            ColTwoPoints,
            ColIndiceKL,
            ColdG,
            NumberOfColumns
        };
    };
};

void RNAduplex::BodyParser::parse(std::string& line)
{
    std::stringstream ss(line);
    std::vector<std::string> result;
    ss >> result;
    if (result.size() != NumberOfColumns)
    {
        throw RNABackendException("Invalid output RNAduplex.");
    }
    const std::string deltaG = result[ColdG].substr(1, result[ColdG].length() - 2);
    helper::convertFromString(deltaG, dG);
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAduplex, std::string, "RNAduplex");

Fe RNAduplex::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string inputTmpFile;
    etilico::createTemporaryFile(inputTmpFile, path, prefix);
    std::string outpTmpFile;
    etilico::createTemporaryFile(outpTmpFile, path, prefix);

    ///Constructed as required by RNAduplex
    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1;
    toHybridize << "\n";
    toHybridize << seq2;
    toHybridize.close();

    std::stringstream cmd2;
    cmd2 << "RNAduplex ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outpTmpFile;

    const etilico::Command cmd = cmd2.str();   ///RNAduplex < outputTmpFile > outputTmpFile
    etilico::runCommand(cmd);

    File fileOutput(outpTmpFile.c_str());
    if (!fileOutput)
    {
        throw NotFoundFileException();
    }
    BodyParser body;
    std::string line;
    getline(fileOutput, line);
    body.parse(line);

    mili::assert_throw<ExceptionUnlink>(unlink(inputTmpFile.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(outpTmpFile.c_str()) == 0);

    return body.dG;
}
} // namespace fideo
