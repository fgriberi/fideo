/*
 * File:   RNAHybrid.cpp
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

using namespace biopp;
using namespace mili;
using namespace std;

namespace fideo
{
class RNAHybrid : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;
    static const unsigned int OBSOLETE_LINES = 6;

    class BodyParser
    {
    public:       
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
                helper::convert_from_string(deltaG, dG);                
            }
        }

        Fe dG;
        static const unsigned int OBSOLETE_dG = 1000;
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int DELTA_G = 1;
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAHybrid, std::string, "RNAHybrid");

Fe RNAHybrid::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }

    FileLine targetSequence = ">HeadToTargetSequence \n" + longerSeq.getString();
    FileLine querySequence = ">HeadToQuerySequence \n" + shorterSeq.getString();

    string fileTmpTarget;
    helper::createTmpFile(fileTmpTarget);
    string fileTmpQuery;
    helper::createTmpFile(fileTmpQuery);
    string fileTmpOutput;
    helper::createTmpFile(fileTmpOutput);

    helper::write(fileTmpTarget, targetSequence);
    helper::write(fileTmpQuery, querySequence);

    stringstream cmd;
    cmd << "RNAhybrid -s 3utr_human ";
    cmd << "-t " << fileTmpTarget;
    cmd << " -q " << fileTmpQuery;
    cmd << " > " << fileTmpOutput;

    const Command command = cmd.str();  //RNAhybrid -s 3utr_human -t fileRNAm -q filemiRNA > FILE_NAME_OUTPUT
    helper::runCommand(command);

    ifstream fileOutput(fileTmpOutput.c_str());
    if (!fileOutput)
    {
        throw RNABackendException("Output file not found.");
    }
    BodyParser body;
    body.parse(fileOutput);

    helper::removeFile(fileTmpTarget);
    helper::removeFile(fileTmpQuery);
    helper::removeFile(fileTmpOutput);
    return body.dG;
}
}
