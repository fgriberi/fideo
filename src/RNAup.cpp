/*
 * @file     RNAup.cpp
 * @brief    RNAup is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAup implementation.
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

using mili::operator>>;

/** @brief RNAup is an implementation of IHybridize interface that use Vienna package
 *
 */
class RNAup : public IHybridize
{
private:

    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const bool longerCirc, const biopp::NucSequence& shorterSeq) const;
    virtual ~RNAup() {}

    /** @brief Class that allows parsing the body of a file
    *
    */
    class BodyParser
    {
    public:
        void parse(File& file);

        Fe dG; ///free energy
    private:

        /** @brief Represents the columns of the file to parse
         *
         */
        enum Columns
        {
            ColRNAduplexResults,
            ColIndiceIJ,
            ColTwoPoints,
            ColIndiceKL,
            ColdGTotal,
            ColEqualSymbol,
            ColdGInt,
            ColPlusSymbol,
            ColdGu_l,
            NumberOfColumns
        };
    };

};

void RNAup::BodyParser::parse(File& file)
{
    std::vector<std::string> aux;
    if (file >> aux)
    {
        if (aux.size() != NumberOfColumns)
        {
            throw RNABackendException("Invalid output RNAup.");
        }
        const std::string deltaG = aux[ColdGTotal].substr(1, aux[ColdGTotal].length());
        helper::convertFromString(deltaG, dG);
    }
    else
    {
        throw RNABackendException("Failured operation >>.");
    }
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAup, std::string, "RNAup");
static const std::string OUT_FILE = "RNA_w25_u3_4_up.out"; ///file generated to RNAup

Fe RNAup::hybridize(const biopp::NucSequence& longerSeq, const bool longerCirc, const biopp::NucSequence& shorterSeq) const
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
    std::string outputTmpFile;
    etilico::createTemporaryFile(outputTmpFile, path, prefix);

    ///Constructed as required by RNAup
    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    std::stringstream cmd2;
    cmd2 << "RNAup -u 3,4 -c SH ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outputTmpFile;

    const etilico::Command cmd = cmd2.str();  //RNAup -u 3,4 -c SH < inputTmpFile > outputTmpFile
    etilico::runCommand(cmd);

    File fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
    {
        throw NotFoundFileException();
    }
    BodyParser body;
    body.parse(fileOutput);

    mili::assert_throw<ExceptionUnlink>(unlink(OUT_FILE.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(inputTmpFile.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(outputTmpFile.c_str()) == 0);

    return body.dG;
}
}