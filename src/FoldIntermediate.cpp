/*
 * @file     FoldIntermediate.cpp
 * @brief    This is the implementation of IFoldIntermediate interface.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing class FoldIntermediate implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date July 2013
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
 *
 * This file is part of fideo.
 *
 * fideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of tghe License, or
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

#include "fideo/IFoldIntermediate.h"

namespace fideo
{

void IFoldIntermediate::commonFold(const biopp::NucSequence& sequence, const bool isCirc, biopp::SecStructure& structure, IntermediateFiles& files)
{
    structure.clear();
    structure.set_circular(isCirc);
    etilico::Command cmd;
    prepareData(sequence, isCirc, cmd, files);
    etilico::runCommand(cmd);
}

Fe IFoldIntermediate::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm)
{
    IntermediateFiles intermediateFiles;
    commonFold(seqRNAm, isCircRNAm, structureRNAm, intermediateFiles);
    const bool deleteOutput = true;
    Fe freeEnergy;
    processingResult(structureRNAm, intermediateFiles, deleteOutput, freeEnergy);
    return freeEnergy;
}

void IFoldIntermediate::foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, FilePath& outputFile)
{
    IntermediateFiles intermediateFiles;
    commonFold(seqRNAm, isCircRNAm, structureRNAm, intermediateFiles);
    deleteObsoleteFiles(intermediateFiles[INPUT_FILE]);
    etilico::Command removeCmd = "mv " + intermediateFiles[OUTPUT_FILE] + " " + outputFile;
    etilico::runCommand(removeCmd);
}

Fe IFoldIntermediate::foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm)
{
    IntermediateFiles intermediateFiles;
    intermediateFiles.push_back("obsoleteFile");
    intermediateFiles.push_back(inputFile);
    Fe freeEnergy;
    const bool deleteOutput = false;
    processingResult(structureRNAm, intermediateFiles, deleteOutput, freeEnergy);
    return freeEnergy;
}

} //namespace fideo