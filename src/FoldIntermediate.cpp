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

static const size_t SIZE_EXPECTED = 2;
static const size_t NAME = 1;

void IFoldIntermediate::getNameOfSequence(const std::string& inputName, std::string& nameSequence)
{
    std::stringstream ss(inputName);
    ResultLine result;
    ss >> mili::Separator(result, '-');
    mili::assert_throw<InvalidInputName>(result.size() == SIZE_EXPECTED);
    nameSequence = result[NAME];
}

void IFoldIntermediate::commonFold(const biopp::NucSequence& sequence, const bool isCirc, const Temperature temp, biopp::SecStructure& structure, InputFile& inputFile, OutputFile& outputFile)
{
    structure.clear();
    structure.set_circular(isCirc);
    etilico::Command cmd;
    prepareData(sequence, isCirc, cmd, inputFile, outputFile);
    etilico::runCommand(cmd);
}

Fe IFoldIntermediate::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const Temperature temp)
{
    InputFile inFile;
    OutputFile outFile;
    commonFold(seqRNAm, isCircRNAm, temp, structureRNAm, inFile, outFile);
    Fe freeEnergy;
    processingResult(structureRNAm, outFile, freeEnergy);
    deleteAllFilesAfterProcessing(inFile, outFile);
    return freeEnergy;
}

void IFoldIntermediate::foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const FilePath& outputFile, const Temperature temp)
{
    InputFile inFile;
    OutputFile outFile;
    commonFold(seqRNAm, isCircRNAm, temp, structureRNAm, inFile, outFile);
    renameNecessaryFiles(outFile, outputFile);
    deleteObsoleteFiles(inFile);
}

Fe IFoldIntermediate::foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm)
{
    Fe freeEnergy;
    processingResult(structureRNAm, inputFile, freeEnergy);
    return freeEnergy;
}

} //namespace fideo
