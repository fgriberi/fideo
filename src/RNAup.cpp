/*
 * File:   RNAup.cpp
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
class RNAup : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const;

    class ParseBody
    {
        enum Columns
        {
            ColRNAduplexResults,
            ColIndiceIJ,
            ColTwoPoints,
            ColIndiceKL,
            ColdGTotal,
            ColEqualSymbol,
            ColdGInt,
            ColPlusSymbol,
            ColdGu_l,
            NumberOfColumns
        };

    public:
        Fe dG;

        void parse(std::ifstream& file)
        {
            vector<string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                    throw RNABackendException("Invalid output RNAup.");
                const string deltaG = aux[ColdGTotal].substr(1, aux[ColdGTotal].length());
                from_string(deltaG, dG);
            }
            else
                throw RNABackendException("Failured operation >>.");
        }
    };

};

static const string FILE_NAME_OUTPUT = "outputHybridize.out";
static const string FILE_AUX = "toHybridizeUp";

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAup, std::string, "RNAup");

Fe RNAup::hybridize(const NucSequence& longerSeq, const NucSequence& shorterSeq, bool longerCirc) const
{
    if (longerCirc)
        throw RNABackendException("Unsupported Sequence.");
    string seq1 = longerSeq.getString();
    string seq2 = shorterSeq.getString();

    ofstream toHybridize(FILE_AUX.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAup ";
    cmd2 << "< " << FILE_AUX;
    cmd2 << " > " << FILE_NAME_OUTPUT;

    const Command CMD2 = cmd2.str();  //RNAup -u 3,4 -c SH < toHybridize > output.out
    runCommand(CMD2);

    ifstream fileOutput(FILE_NAME_OUTPUT.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    body.parse(fileOutput);
    remove_file(FILE_NAME_OUTPUT.c_str());
    remove_file(FILE_AUX);
    return body.dG;
}
