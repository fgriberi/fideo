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

#include "fideo/IHybridize.h"

using namespace biopp;
using namespace mili;
using namespace std;

namespace fideo
{
//Vienna package
class RNAduplex : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

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
                throw RNABackendException("Invalid output RNAduplex.");
            const string deltaG = result[ColdG].substr(1, result[ColdG].length() - 2);
            if (!from_string(deltaG, dG))
                throw RNABackendException("Failed to convert the string to value type.");
        }
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAduplex, std::string, "RNAduplex");

Fe RNAduplex::hybridize(const NucSequence& longerSeq, bool longerCirc, const NucSequence& shorterSeq) const
{
    if (longerCirc)
        throw RNABackendException("Unsupported Sequence.");
    const string seq1 = longerSeq.getString();
    const string seq2 = shorterSeq.getString();

    string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    string outpTmpFile;
    helper::createTmpFile(outpTmpFile);

    ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1;
    toHybridize << "\n";
    toHybridize << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAduplex ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outpTmpFile;

    const Command cmd = cmd2.str();
    helper::runCommand(cmd);

    ifstream fileOutput(outpTmpFile.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    string line;
    getline(fileOutput, line);
    body.parse(line);
    helper::removeFile(inputTmpFile);
    helper::removeFile(outpTmpFile);
    return body.dG;
}
}
