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

#include <sstream>
#include <string>
#include <mili/mili.h>
#include "fideo/IHybridize.h"
#include "fideo/RNABackendProxy.h"
#include "fideo/TmpFile.h"

using namespace biopp;
using namespace mili;
using namespace std;

class RNAHybrid : public IHybridize
{
    static const unsigned int OBSOLETE_LINES = 6;
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const;

    class ParseBody
    {
    public:
        Fe dG;
        static const unsigned int OBSOLETE_dG = 1000;
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int DELTA_G = 1;

        void parse(std::ifstream& file)
        {
            string temp;
            for (size_t i = 0; i < OBSOLETE_LINES; ++i)
                getline(file, temp);
            stringstream ss(temp);
            vector<string> result;
            ss >> Separator(result, ' ');
            if (result.size() != SIZE_LINE)
                dG = OBSOLETE_dG; //no significant hybridization found
            else
            {
                const string deltaG = result[DELTA_G];
                if (!from_string(deltaG, dG))
                    throw RNABackendException("Failed to convert the string to value type.");
            }
        }
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAHybrid, std::string, "RNAHybrid");

Fe RNAHybrid::hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const
{
    if (longerCirc)
        throw RNABackendException("Unsupported Sequence.");

    FileLine targetSequence = ">HeadToTargetSequence \n" + longerSeq.getString();
    FileLine querySequence = ">HeadToQuerySequence \n" + shorterSeq.getString();

    TmpFile temporalTargetFile;
    TmpFile temporalQueryFile;
    TmpFile temporalOutputFile;

    const string fileTmpTarget = temporalTargetFile.getTmpName();
    const string fileTmpQuery = temporalQueryFile.getTmpName();
    const string fileTmpOutput = temporalOutputFile.getTmpName();

    write(fileTmpTarget, targetSequence);
    write(fileTmpQuery, querySequence);

    stringstream cmd;
    cmd << "RNAhybrid -s 3utr_human ";
    cmd << "-t " << fileTmpTarget;
    cmd << " -q " << fileTmpQuery;
    cmd << " > " << fileTmpOutput;

    const Command command = cmd.str();  //RNAhybrid -s 3utr_human -t fileRNAm -q filemiRNA > FILE_NAME_OUTPUT
    runCommand(command);

    ifstream fileOutput(fileTmpOutput.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    body.parse(fileOutput);
    return body.dG;
}
