/*
 * @file     RNAcofold.cpp
 * @brief    RNAcofold is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAcofold implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date October 28, 2012, 10:32 AM
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

#define RNA_COFOLD_H
#include "fideo/RNAcofold.h"
#undef RNA_COFOLD_H

namespace fideo
{

void RNAcofold::BodyParser::parse(std::string& line)
{
    std::stringstream ss(line);
    ResultLine result;
    ss >> mili::Separator(result, ' ');
    mili::assert_throw<InvalidOutputRNACofold>(result.size() == NumberOfColumns);
    const std::string deltaG = result[ColdG].substr(1, result[ColdG].size() - 2);
    helper::convertFromString(deltaG, _dG);
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAcofold, std::string, "RNAcofold");

void RNAcofold::prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                            etilico::Command& command, InputFiles& inFiles, OutputFile& outFile) const
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

    std::ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    std::stringstream exec;
    exec << "RNAcofold ";
    exec << "< " << inputTmpFile;
    exec << " > " << outputTmpFile;

    command = exec.str(); /// RNAcofold < inputTmpFile > outputTmpFile
}

void RNAcofold::processingResult(const OutputFile& outFile, Fe& freeEnergy) const
{
    File outputFile(outFile.c_str());
    mili::assert_throw<NotFoundFileException>(outputFile);
    std::string temp;
    getline(outputFile, temp);
    getline(outputFile, temp);

    BodyParser body;
    body.parse(temp);
    freeEnergy = body._dG;
}

void RNAcofold::deleteObsoleteFiles(const InputFiles& inFiles, const OutputFile& outFile) const
{
    for (size_t i(0); i < inFiles.size(); ++i)
    {
        mili::assert_throw<UnlinkException>(unlink(inFiles[i].c_str()) == 0);
    }
    mili::assert_throw<UnlinkException>(unlink(outFile.c_str()) == 0);
}
} // namespace fideo

