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

#include <sstream>
#include <string>
#include <mili/mili.h>
#include "fideo/IHybridize.h"
#include "fideo/RNABackendProxy.h"

using namespace biopp;
using namespace mili;
using namespace std;

//Vienna package (version 1.8.5)
class IntaRNA : public IHybridize
{
    string argPath;
    virtual Fe hybridize(const NucSequence& firstSeq, const NucSequence& secondSeq) const;
    void setArgument(const string& arg);

    class ParseBody
    {
        enum Columns
        {
            ColEnergy,
            ColdG,
            ColUnit,
            NumberOfColumns
        };

    public:
        static const size_t DELTA_G = 2;
        Fe dG;

        void parse(std::ifstream& file)
        {
            vector<string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                    throw RNABackendException("Invalid output RNAup.");
                string deltaG = aux[DELTA_G];
                from_string(deltaG, dG);
            }
            else
                throw RNABackendException("Failured operation >>.");
        }
    };
};

static const string FILE_NAME_OUTPUT = "outputIntaRNA.out";
static const unsigned int LINE = 8;

REGISTER_FACTORIZABLE_CLASS(IHybridize, IntaRNA, std::string, "IntaRNA");

void IntaRNA::setArgument(const string& arg)
{
    argPath = arg;
}

Fe IntaRNA::hybridize(const NucSequence& firstSeq, const NucSequence& secondSeq) const
{
    string seq1;
    for (size_t i = 0; i < firstSeq.length(); ++i)
        seq1 += firstSeq[i].as_char();
    string seq2;
    for (size_t i = 0; i < secondSeq.length(); ++i)
        seq2 += secondSeq[i].as_char();

    stringstream cmd;
    cmd << "./IntaRNA ";
    cmd << seq1;
    cmd << " " << seq2;
    cmd << " > " << FILE_NAME_OUTPUT;

    //move to the directory where is the humanizer
    if (chdir(argPath.c_str()) != 0)
        throw RNABackendException("Invalid path of IntaRNA execute.");

    const Command CMD = cmd.str();  //./IntaRNA seq1 seq2 > outputIntaRNA.out
    runCommand(CMD);

    ifstream fileOutput(FILE_NAME_OUTPUT.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    string temp;
    for (size_t i = 0; i < LINE; ++i)
        getline(fileOutput, temp);
    body.parse(fileOutput);
    remove_file(FILE_NAME_OUTPUT.c_str());
    return body.dG;
}
