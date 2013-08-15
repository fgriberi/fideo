/*
 * @file     RNAduplex.cpp
 * @brief    RNAduplex is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAduplex implementation.
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

#define RNA_DUPLEX_H
#include "fideo/RNAduplex.h"
#undef RNA_DUPLEX_H

namespace fideo
{

void RNAduplex::BodyParser::parse(std::string& line)
{
    std::stringstream ss(line);
    ResultLine result;
    ss >> result;
    mili::assert_throw<InvalidOutputRNADuplex>(result.size() == NumberOfColumns);
    const std::string deltaG = result[ColdG].substr(1, result[ColdG].length() - 2);
    helper::convertFromString(deltaG, dG);
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAduplex, std::string, "RNAduplex");

void RNAduplex::prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, etilico::Command& command, IntermediateFiles& outputFiles) const
{
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string inputTmpFile;
    etilico::createTemporaryFile(inputTmpFile, path, prefix);
    outputFiles.push_back(inputTmpFile);
    std::string outputTmpFile;
    etilico::createTemporaryFile(outputTmpFile, path, prefix);
    outputFiles.push_back(outputTmpFile);

    ///Constructed as required by RNAduplex
    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1;
    toHybridize << "\n";
    toHybridize << seq2;
    toHybridize.close();

    std::stringstream cmd2;
    cmd2 << "RNAduplex ";
    cmd2 << "< " << inputTmpFile;
    cmd2 << " > " << outputTmpFile;

    command = cmd2.str();   ///RNAduplex < outputTmpFile > outputTmpFile
}

void RNAduplex::processingResult(const IntermediateFiles& inputFiles, Fe& freeEnergy) const
{
    File outputFile(inputFiles[FILE_2].c_str());
    mili::assert_throw<NotFoundFileException>(outputFile);   
    BodyParser body;
    std::string line;
    getline(outputFile, line);
    body.parse(line);

    mili::assert_throw<UnlinkException>(unlink(inputFiles[FILE_1].c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink(inputFiles[FILE_2].c_str()) == 0);
    freeEnergy = body.dG;
}

} // namespace fideo
