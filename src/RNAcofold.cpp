/*
 * File:   RNAcofold.cpp
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on October 28, 2012, 10:32 AM
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

using namespace biopp;
using namespace mili;
using namespace std;

//Vienna package
class RNAcofold : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& firstSeq, const biopp::NucSequence& secondSeq, bool firstSeqIsCirc) const;

    class ParseBody
    {
        enum Columns
        {
            ColRNAcofoldResult,
            ColdG,
            NumberOfColumns
        };

    public:
        Fe dG;

        void parse(string& line)
        {
            stringstream ss(line);
            vector<string> result;
            ss >> mili::Separator(result, ' ');
            if (result.size() != NumberOfColumns)
                throw RNABackendException("Invalid output RNAup.");
            const string deltaG = result[ColdG].substr(1, result[ColdG].size() - 2);
            from_string(deltaG, dG);
        }
    };
};

static const string FILE_NAME_OUTPUT = "outputRNAcofold.out";
static const string FILE_AUX = "toHybridizeCofold";

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAcofold, std::string, "RNAcofold");

Fe RNAcofold::hybridize(const NucSequence& firstSeq, const NucSequence& secondSeq, bool firstSeqIsCirc) const
{
    if (firstSeqIsCirc)
        throw RNABackendException("Unsupported Sequence.");
    string seq1;
    for (size_t i = 0; i < firstSeq.length(); ++i)
        seq1 += firstSeq[i].as_char();
    string seq2;
    for (size_t i = 0; i < secondSeq.length(); ++i)
        seq2 += secondSeq[i].as_char();

    ofstream toHybridize(FILE_AUX.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAcofold ";
    cmd2 << "< " << FILE_AUX;
    cmd2 << " > " << FILE_NAME_OUTPUT;

    const Command CMD2 = cmd2.str();
    runCommand(CMD2);

    ifstream fileOutput(FILE_NAME_OUTPUT.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");

    string temp;
    getline(fileOutput, temp);
    getline(fileOutput, temp);

    ParseBody body;
    body.parse(temp);
    remove_file(FILE_NAME_OUTPUT.c_str());
    remove_file(FILE_AUX);
    return body.dG;
}
