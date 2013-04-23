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

#include "fideo/IHybridize.h"
#include "fideo/FideoConfig.h"

using namespace biopp;
using namespace mili;
using namespace std;

namespace fideo
{
class IntaRNA : public IHybridize
{
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
        void parse(std::ifstream& file)
        {
            string temp;
			///advance to the required line
            for (size_t i = 0; i < OBSOLETE_LINES; ++i)
            {
                getline(file, temp);
            }
            stringstream ss(temp);
            vector<string> result;
            ss >> Separator(result, ' ');
            if (result.size() != SIZE_LINE)
            {
                dG = OBSOLETE_dG;
            }
            else
            {
                const string deltaG = result[DELTA_G];
                helper::convert_from_string(deltaG, dG);
            }
        }

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

static const string EXECUTABLE_PATH = "runIntaRNA"; ///name executable to find

REGISTER_FACTORIZABLE_CLASS(IHybridize, IntaRNA, std::string, "IntaRNA");

Fe IntaRNA::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }

    const string seq1 = longerSeq.getString();
    const string seq2 = shorterSeq.getString();

    string tmpFileOutput;
    helper::createTmpFile(tmpFileOutput);

    stringstream cmd;
    cmd << "./IntaRNA ";
    cmd << seq1;
    cmd << " " << seq2;
    cmd << " > " << tmpFileOutput;

    //move to the directory where is the folding
    std::string executablePath;
    fideo::FideoConfig::getInstance()->getPath(EXECUTABLE_PATH, executablePath);
    if (chdir(executablePath.c_str()) != 0)
    {
        throw RNABackendException("Invalid path of IntaRNA executable.");
    }

    const Command command = cmd.str();   ///./IntaRNA seq1 seq2 > /temp/myTmpFile-******
    helper::runCommand(command);

    ifstream fileOutput(tmpFileOutput.c_str());
    if (!fileOutput)
    {
        throw RNABackendException("Output file not found.");
    }
    BodyParser body;
    body.parse(fileOutput);
    helper::removeFile(tmpFileOutput);
    return body.dG;
}
} // namespace fideo
