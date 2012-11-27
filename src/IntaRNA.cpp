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
    static const unsigned int OBSOLETE_LINES = 8;
    string argPath;
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const;
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
        Fe dG;

        void parse(std::ifstream& file)
        {
            vector<string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                    throw RNABackendException("Invalid output IntaRNA.");
                string deltaG = aux[ColdG];
                from_string(deltaG, dG);
            }
            else
                throw RNABackendException("Failed operation >>.");
        }
    };
};

static const string FILE_NAME_OUTPUT = "outputIntaRNA.out";

REGISTER_FACTORIZABLE_CLASS(IHybridize, IntaRNA, std::string, "IntaRNA");

void IntaRNA::setArgument(const string& arg)
{
    argPath = arg;
}

Fe IntaRNA::hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const
{
    if (longerCirc)
        throw RNABackendException("Unsupported Sequence.");
    string seq1;
    for (size_t i = 0; i < longerSeq.length(); ++i)
        seq1 += longerSeq[i].as_char();
    string seq2;
    for (size_t i = 0; i < shorterSeq.length(); ++i)
        seq2 += shorterSeq[i].as_char();

    stringstream cmd;
    cmd << "./IntaRNA ";
    cmd << seq1;
    cmd << " " << seq2;
    cmd << " > " << FILE_NAME_OUTPUT;

    //move to the directory where is the humanizer
    if (chdir(argPath.c_str()) != 0)
        throw RNABackendException("Invalid path of IntaRNA executable.");

    const Command CMD = cmd.str();  //./IntaRNA seq1 seq2 > outputIntaRNA.out
    runCommand(CMD);

    ifstream fileOutput(FILE_NAME_OUTPUT.c_str());
    if (!fileOutput)
        throw RNABackendException("Output file not found.");
    ParseBody body;
    string temp;
    for (size_t i = 0; i < OBSOLETE_LINES; ++i)
        getline(fileOutput, temp);
    body.parse(fileOutput);
    remove_file(FILE_NAME_OUTPUT.c_str());
    return body.dG;
}
