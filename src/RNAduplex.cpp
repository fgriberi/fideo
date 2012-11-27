/*
 * File:   RNAduplex.cpp
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on October 26, 2012, 7:48 PM
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
class RNAduplex : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const;

    class ParseBody
    {
        enum Columns
        {
            ColRNAResults,
            ColIndiceIJ,
            ColTwoPoints,
            ColIndiceKL,
            ColdG,
            NumberOfColumns
        };

    public:
        Fe dG;

        void parse(string& line)
        {
            stringstream ss(line);
            vector<string> result;
            ss >> result;
            if (result.size() != NumberOfColumns)
                throw RNABackendException("Invalid output RNAup.");
            const string deltaG = result[ColdG].substr(1, result[ColdG].length() - 2);
            from_string(deltaG, dG);
        }
    };
};

static const string FILE_NAME_OUTPUT = "outputRNAduplex.out";
static const string FILE_AUX = "toHybridizeDuplex";

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAduplex, std::string, "RNAduplex");

Fe RNAduplex::hybridize(const NucSequence& longerSeq, const NucSequence& shorterSeq, bool longerCirc) const
{
    if (longerCirc)
        throw RNABackendException("Unsupported Sequence.");
    string seq1;
    for (size_t i = 0; i < longerSeq.length(); ++i)
        seq1 += longerSeq[i].as_char();
    string seq2;
    for (size_t i = 0; i < shorterSeq.length(); ++i)
        seq2 += shorterSeq[i].as_char();

    ofstream toHybridize(FILE_AUX.c_str());
    toHybridize << seq1;
    toHybridize << "\n";
    toHybridize << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAduplex ";
    cmd2 << "< " << FILE_AUX;
    cmd2 << " > " << FILE_NAME_OUTPUT;

    const Command CMD2 = cmd2.str();
    runCommand(CMD2);

    ifstream fileOutput(FILE_NAME_OUTPUT.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    string line;
    getline(fileOutput, line);
    body.parse(line);
    remove_file(FILE_NAME_OUTPUT.c_str());
    remove_file(FILE_AUX);
    return body.dG;
}
