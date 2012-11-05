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
    virtual Fe hybridize(const NucSequence& firstSeq, const NucSequence& secondSeq) const;

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
        static const size_t DELTA_G = 4;
        Fe dG;

        void parse(string& line)
        {
            stringstream ss(line);
            vector<string> result;
            ss >> result;
            if (result.size() != NumberOfColumns)
                throw RNABackendException("Invalid output RNAup.");
            string deltaG = result[DELTA_G].substr(1, result[DELTA_G].length() - 2);
            from_string(deltaG, dG);
        }
    };
};

static const string FILE_NAME_OUTPUT = "outputRNAduplex.out";

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAduplex, std::string, "RNAduplex");

Fe RNAduplex::hybridize(const NucSequence& firstSeq, const NucSequence& secondSeq) const
{
    string seq1;
    for (size_t i = 0; i < firstSeq.length(); ++i)
        seq1 += firstSeq[i].as_char();
    string seq2;
    for (size_t i = 0; i < secondSeq.length(); ++i)
        seq2 += secondSeq[i].as_char();

    ofstream toHybridize("toHybridizeDuplex");
    toHybridize << seq1;
    toHybridize << "\n";
    toHybridize << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAduplex ";
    cmd2 << "< " << "toHybridizeDuplex ";
    cmd2 << "> " << FILE_NAME_OUTPUT;

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
    return body.dG;
}
