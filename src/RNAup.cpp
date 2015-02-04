/*
 * @file     RNAup.cpp
 * @brief    RNAup is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAup implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
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

#define RNA_UP_H
#include "fideo/RNAup.h"
#undef RNA_UP_H

namespace fideo
{

void RNAup::BodyParser::parse(File& file)
{
    ResultLine aux;
    if (file >> aux)
    {
        mili::assert_throw<InvalidOutputRNAUp>(aux.size() == NumberOfColumns);
        const std::string deltaG = aux[ColdGTotal].substr(1, aux[ColdGTotal].length());
        helper::convertFromString(deltaG, _dG);
    }
    else
    {
        throw RNABackendException("Failured operation >>.");
    }
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAup, std::string, "RNAup");
static const std::string OUT_FILE = "RNA_w25_u2.out"; ///file generated to RNAup


void RNAup::prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                        etilico::Command& command, InputFiles& inFiles, OutputFile& outFile, const Temperature temp) const
{
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string inputTmpFile;
    etilico::createTemporaryFile(inputTmpFile, path, prefix);
    inFiles.push_back(inputTmpFile);
    std::string outputTmpFile;
    etilico::createTemporaryFile(outputTmpFile, path, prefix);
    outFile = outputTmpFile;

    ///Constructed as required by RNAup
    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    std::stringstream cmd2;
    cmd2 << "RNAup -u 3,4 -c SH -T " << temp;
    cmd2 << " < " << inputTmpFile;
    cmd2 << " > " << outputTmpFile;

    command = cmd2.str();  //RNAup -u 3,4 -c SH --temp=temp < inputTmpFile > outputTmpFile
}

void RNAup::processingResult(const OutputFile& outFile, Fe& freeEnergy) const
{
    File outputFile(outFile.c_str());
    mili::assert_throw<NotFoundFileException>(outputFile);
    BodyParser body;
    body.parse(outputFile);

    File outputfile(OUT_FILE.c_str());
    if (outputfile)
    {
        mili::assert_throw<UnlinkException>(unlink(OUT_FILE.c_str()) == 0);
    }
    freeEnergy = body._dG;
}

void RNAup::deleteObsoleteFiles(const InputFiles& inFiles, const OutputFile& outFile) const
{
    for (size_t i(0); i < inFiles.size(); ++i)
    {
        mili::assert_throw<UnlinkException>(unlink(inFiles[i].c_str()) == 0);
    }
    mili::assert_throw<UnlinkException>(unlink(outFile.c_str()) == 0);
}

} // end namespace
