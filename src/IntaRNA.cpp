/*
 * File:   IntaRNA.cpp
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on November 02, 2012, 19:35 PM
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
    static const unsigned int OBSOLETE_LINES = 9;
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

    class ParseBody
    {
        static const unsigned int DELTA_G = 1;
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int OBSOLETE_dG = 1000;

        enum Columns
        {
            ColEnergy,
            ColdG,
            ColUnit,
            NumberOfColumns
        };

    public:
        Fe dG;

        void parse(std::ifstream& file)
        {
            string temp;
            for (size_t i = 0; i < OBSOLETE_LINES; ++i)
            {
                getline(file, temp);
            }

            stringstream ss(temp);
            vector<string> result;
            ss >> Separator(result, ' ');
            if (result.size() != SIZE_LINE)
            {
                dG = OBSOLETE_dG; //no significant hybridization found
            }
            else
            {
                const string deltaG = result[DELTA_G];
                if (!from_string(deltaG, dG))
                {
                    throw RNABackendException("Failed to convert the string to value type.");
                }
            }
        }
    };
};

static const string INTA_RNA = "runIntaRNA";

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
    if (chdir(fideo::FideoConfig::getInstance()->getPath(INTA_RNA).c_str()) != 0)
    {
        throw RNABackendException("Invalid path of IntaRNA executable.");
    }

    const Command command = cmd.str();  //./IntaRNA seq1 seq2 > outputIntaRNA.out
    helper::runCommand(command);

    ifstream fileOutput(tmpFileOutput.c_str());
    if (!fileOutput)
    {
        throw RNABackendException("Output file not found.");
    }
    ParseBody body;
    body.parse(fileOutput);
    helper::removeFile(tmpFileOutput);
    return body.dG;
}
}
