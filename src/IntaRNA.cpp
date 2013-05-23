/*
 * @file   IntaRNA.cpp
 * @brief  IntaRNA is the implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend IntaRNA implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 02, 2012, 19:35 PM
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

#include <unistd.h>
#include <etilico/etilico.h>
#include "fideo/IHybridize.h"

namespace fideo
{
class IntaRNA : public IHybridize
{
private:
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;
    static const unsigned int OBSOLETE_LINES = 9; ///obsolete lines in file

	///Class that allows parsing the body of a file
    class BodyParser
    {    
    public:
        Fe dG; ///free energy to read the file

		/** @brief Parse the file and get the value dG
         *
         * @param file: file to parser
         * @return void
         */
        void parse(File& file);
 
    private:
        static const unsigned int DELTA_G = 1;
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int OBSOLETE_dG = 1000; ///no significant hybridization found

        enum Columns
        {
            ColEnergy,
            ColdG,
            ColUnit,
            NumberOfColumns
        };
    };
};

void IntaRNA::BodyParser::parse(File& file)
{
    std::string temp;
    ///advance to the required line
    for (size_t i = 0; i < OBSOLETE_LINES; ++i)
    {
        getline(file, temp);
    }
    std::stringstream ss(temp);
    std::vector<std::string> result;
    ss >> mili::Separator(result, ' ');
    if (result.size() != SIZE_LINE)
    {
        dG = OBSOLETE_dG;
    }
    else
    {
        const std::string deltaG = result[DELTA_G];
        helper::convertFromString(deltaG, dG);        
    }
}

static const std::string EXECUTABLE_PATH = "runIntaRNA"; ///name executable to find

REGISTER_FACTORIZABLE_CLASS(IHybridize, IntaRNA, std::string, "IntaRNA");

Fe IntaRNA::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }

    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    std::string tmpFileOutput;
    etilico::createTemporaryFilename(tmpFileOutput);

    std::stringstream exec;
    exec << "./IntaRNA ";
    exec << seq1;
    exec << " " << seq2;
    exec << " > " << tmpFileOutput;

    //move to the directory where is the folding
    std::string executablePath;
    etilico::Config::getInstance()->getPath(EXECUTABLE_PATH, executablePath);
    if (chdir(executablePath.c_str()) != 0)
    {
        throw RNABackendException("Invalid path of IntaRNA executable.");
    }

    const etilico::Command cmd = exec.str();   ///./IntaRNA seq1 seq2 > /temp/myTmpFile-******
    etilico::runCommand(cmd);

    File fileOutput(tmpFileOutput.c_str());
    if (!fileOutput)
    {
        throw RNABackendException("Output file not found.");
    }
    BodyParser body;
    body.parse(fileOutput);    
    mili::assert_throw<ExceptionUnlink>(unlink(tmpFileOutput.c_str()) == 0);

    return body.dG;
}
} // namespace fideo


