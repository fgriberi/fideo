/*
 * @file   RNAup.cpp
 * @brief  RNAup is the implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend RNAup implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date October 26, 2012, 7:48 PM
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

namespace fideo
{

using namespace mili;

//Vienna package
class RNAup : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

    class BodyParser
    {        
    public:
        
        void parse(std::ifstream& file)
        {
            std::vector<std::string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                {   
                    throw RNABackendException("Invalid output RNAup.");
                }
                const std::string deltaG = aux[ColdGTotal].substr(1, aux[ColdGTotal].length());
                helper::convertFromString(deltaG, dG);              
            }
            else
                throw RNABackendException("Failured operation >>.");
        }

        Fe dG; ///free energy
    private:
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
    };

};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAup, std::string, "RNAup");
static const std::string OUT_FILE = "RNA_w25_u3_4_up.out"; ///file generated to RNAup

Fe RNAup::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    std::string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    std::string outputTmpFile;
    helper::createTmpFile(outputTmpFile);

    ///Constructed as required by RNAup
    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    std::stringstream cmd2;
    cmd2 << "RNAup -u 3,4 -c SH ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outputTmpFile;

    const Command cmd = cmd2.str();  //RNAup -u 3,4 -c SH < inputTmpFile > outputTmpFile
    helper::runCommand(cmd);

    std::ifstream fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
    {
        throw NotFoundFileException();
    }
    BodyParser body;
    body.parse(fileOutput);
    helper::removeFile(OUT_FILE.c_str());
    helper::removeFile(inputTmpFile);
    helper::removeFile(outputTmpFile);
    return body.dG;
}
}
