/*
 * @file     RNAHybrid.cpp
 * @brief    RNAHybrid is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RANHybrid implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date November 02, 2012, 19:35 PM
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

#define RNA_HYBRID_H
#include "fideo/RNAHybrid.h"
#undef RNA_HYBRID_H

namespace fideo
{

void RNAHybrid::BodyParser::parse(File& file)
{
    std::string temp;
    for (size_t i = 0; i < OBSOLETE_LINES; ++i)
    {
        getline(file, temp);
    }
    std::stringstream ss(temp);
    ResultLine result;
    ss >> mili::Separator(result, ' ');
    if (result.size() != SIZE_LINE)
    {
        _dG = OBSOLETE_dG; //no significant hybridization found
    }
    else
    {
        const std::string deltaG = result[DELTA_G];
        helper::convertFromString(deltaG, _dG);
    }
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAHybrid, std::string, "RNAHybrid");

void RNAHybrid::prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                            etilico::Command& command, InputFiles& inFiles, OutputFile& outFile) const
{
    ///Add obsolete description in sequence. RNAHybrid requires FASTA formatted file
    FileLine targetSequence = ">HeadToTargetSequence \n" + longerSeq.getString();
    FileLine querySequence = ">HeadToQuerySequence \n" + shorterSeq.getString();

    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string tmpTargetFile;
    etilico::createTemporaryFile(tmpTargetFile, path, prefix);
    inFiles.push_back(tmpTargetFile);
    std::string tmpQueryFile;
    etilico::createTemporaryFile(tmpQueryFile, path, prefix);
    inFiles.push_back(tmpQueryFile);
    std::string tmpOutputFile;
    etilico::createTemporaryFile(tmpOutputFile, path, prefix);
    outFile = tmpOutputFile;

    helper::write(tmpTargetFile, targetSequence);
    helper::write(tmpQueryFile, querySequence);

    std::stringstream exec;
    exec << "RNAhybrid -s 3utr_human ";
    exec << "-t " << tmpTargetFile;
    exec << " -q " << tmpQueryFile;
    exec << " > " << tmpOutputFile;

    command = exec.str();  /// RNAhybrid -s 3utr_human -t fileRNAm -q filemiRNA > tmpOutputFile
}

void RNAHybrid::deleteObsoleteFiles(const InputFiles& inFiles, const OutputFile& outFile) const
{
    for (size_t i(0); i < inFiles.size(); ++i)
    {
        mili::assert_throw<UnlinkException>(unlink(inFiles[i].c_str()) == 0);
    }
    mili::assert_throw<UnlinkException>(unlink(outFile.c_str()) == 0);
}

void RNAHybrid::processingResult(const OutputFile& outFile, Fe& freeEnergy) const
{
    File outputFile(outFile.c_str());
    mili::assert_throw<NotFoundFileException>(outputFile);
    BodyParser body;
    body.parse(outputFile);
    freeEnergy = body._dG;
}

} // namespace fideo