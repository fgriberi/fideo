/*
 * @file   RNAcofold.cpp
 * @brief  RNAcofold is the implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend RNAcofold implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date October 28, 2012, 10:32 AM
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
//Vienna package
class RNAcofold : public IHybridize
{
private:
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

	///Class that allows parsing the body of a file
    class BodyParser
    {     
    public:      
		/** @brief Parse the line and get the value dG
         *
         * @param line: line to parser
         * @return void
         */
        void parse(std::string& line);      

        Fe dG; ///free energy

    private:
        enum Columns
        {
            ColRNAcofoldResult,
            ColOpenParenthesis,
            ColdG,
            NumberOfColumns
        };
    };
};

void RNAcofold::BodyParser::parse(std::string& line)
{
    std::stringstream ss(line);
    std::vector<std::string> result;
    ss >> mili::Separator(result, ' ');
    if (result.size() != NumberOfColumns)
    {
        throw RNABackendException("Invalid output RNAcofold.");
    }
    const std::string deltaG = result[ColdG].substr(0, result[ColdG].size() - 1);
    helper::convertFromString(deltaG, dG);    
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAcofold, std::string, "RNAcofold");

Fe RNAcofold::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    std::string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    std::string outputTmpFile;
    helper::createTmpFile(outputTmpFile);

    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    std::stringstream exec;
    exec << "RNAcofold ";
    exec << "< " << inputTmpFile;
    exec << " > " << outputTmpFile;

    const etilico::Command cmd = exec.str(); /// RNAcofold < inputTmpFile > outputTmpFile
    etilico::runCommand(cmd);

    std::ifstream fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
    {
        throw NotFoundFileException();
    }

    std::string temp;
    getline(fileOutput, temp);
    getline(fileOutput, temp);

    BodyParser body;
    body.parse(temp);

    mili::assert_throw<ExceptionUnlink>(unlink(inputTmpFile.c_str()));
    mili::assert_throw<ExceptionUnlink>(unlink(outputTmpFile.c_str()));

    return body.dG;
}
} // namespace fideo

