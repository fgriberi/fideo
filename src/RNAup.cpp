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

#include "fideo/IHybridize.h"

using namespace biopp;
using namespace mili;
using namespace std;

namespace fideo
{
//Vienna package
class RNAup : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

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
                {   
                    throw RNABackendException("Invalid output RNAup.");
                }
                const string deltaG = aux[ColdGTotal].substr(1, aux[ColdGTotal].length());
                helper::convert_from_string(deltaG, dG);              
            }
            else
                throw RNABackendException("Failured operation >>.");
        }
    };

};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAup, std::string, "RNAup");
static const string OUT_FILE = "RNA_w25_u3_4_up.out"; //file generated to RNAup

Fe RNAup::hybridize(const NucSequence& longerSeq, bool longerCirc, const NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }
    const string seq1 = longerSeq.getString();
    const string seq2 = shorterSeq.getString();

    string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    string outputTmpFile;
    helper::createTmpFile(outputTmpFile);

    ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    stringstream cmd2;
    cmd2 << "RNAup -u 3,4 -c SH ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outputTmpFile;

    const Command command = cmd2.str();  //RNAup -u 3,4 -c SH < toHybridize > output.out
    helper::runCommand(command);

    ifstream fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
    {
        throw RNABackendException("Output file not found.");
    }
    ParseBody body;
    body.parse(fileOutput);
    helper::removeFile(OUT_FILE.c_str());
    helper::removeFile(inputTmpFile);
    helper::removeFile(outputTmpFile);
    return body.dG;
}
}
