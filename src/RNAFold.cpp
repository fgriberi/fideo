/*
 * @file     RNAFold.cpp
 * @brief    RNAFold is an implementation of IFold interface. It's a specific backend to folding.
 *
 * @author   Santiago Videla
 * @email    santiago.videla AT gmail.com
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend RNAFold implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date November 10, 2010, 2012 4:26 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, Franco Riberi, FuDePAN
 *
 * This file is part of fideo
 *
 * fideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * fideo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stack>
#define RNA_FOLD_H
#include "fideo/RNAFold.h"
#undef RNA_FOLD_H

/** @brief Temporal method requerid to execute remo
*
* @param derivedKey: name of derived class
* @return pointer to the base class
*/
fideo::IFold* getDerivedFold(const std::string& derivedKey)
{
    fideo::IFold* const ptr = fideo::Fold::new_class(derivedKey);
    mili::assert_throw<fideo::InvalidDerived>(ptr != NULL);
    return ptr;
}

namespace fideo
{

const FileLineNo RNAFold::LINE_NO = 1;

REGISTER_FACTORIZABLE_CLASS(IFold, RNAFold, std::string, "RNAFold");

size_t RNAFold::readFreeEnergy(FileLine& line, size_t offset, Fe& energy) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_of("(", offset)) + 1;
        const size_t to = mili::ensure_found(line.find_first_of(")", from)) - 1;
        helper::readValue(line, from, to - from, energy);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read free energy");
    }
}

void RNAFold::parseStructure(std::string& str, biopp::SecStructure& secStructure)
{
    secStructure.set_sequence_size(str.length());
    std::stack<biopp::SeqIndex> stackIndex;
    for (size_t i = 0; i < str.length(); ++i)
    {
        biopp::SeqIndex open;
        switch (str[i])
        {
            case UNPAIR:
                secStructure.unpair(i);
                break;
            case OPEN_PAIR:
                stackIndex.push(i);
                break;
            case CLOSE_PAIR:
                if (!stackIndex.empty())
                {
                    open = stackIndex.top();
                    secStructure.pair(open, i);
                    stackIndex.pop();
                }
                else
                {
                    throw InvalidStructureException("Unexpected closing pair");
                }
                break;
            default:
                throw InvalidStructureException("Unexpected symbol: " + secStructure.paired_with(i));
                break;
        }
    }
    if (!stackIndex.empty())
    {
        throw InvalidStructureException("Pairs pending to close");
    }
}

size_t RNAFold::getSizeOfSequence(const FilePath& file) const
{
    File fileIn(file.c_str());
    if (!fileIn)
    {
        throw NotFoundFileException();
    }
    std::string lineSequence;
    getline(fileIn, lineSequence);
    return lineSequence.length();
}

void RNAFold::renameNecessaryFiles(const std::string& fileToRename, const std::string& newNameFile)
{
    renameFile(fileToRename, newNameFile);
}

void RNAFold::deleteObsoleteFiles(const std::string& nameFile)
{
    mili::assert_throw<UnlinkException>(unlink(nameFile.c_str()) == 0);
}

void RNAFold::prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, IntermediateFiles& outputFiles)
{
    FileLine sseq = sequence.getString();
    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string inputFile;
    etilico::createTemporaryFile(inputFile, path, prefix);
    outputFiles.push_back(inputFile);
    std::string outputFile;
    etilico::createTemporaryFile(outputFile, path, prefix);
    outputFiles.push_back(outputFile);
    helper::write(inputFile, sseq);
    std::stringstream ss;
    ss << "RNAfold" << " --noPS ";
    if (isCirc)
    {
        ss << "--circ ";
    }
    ss << "< " << inputFile << " > " << outputFile;
    command = ss.str(); /// RNAfold --noPS ("" | --circ) < inputFile > outputFile
}

void RNAFold::processingResult(biopp::SecStructure& structureRNAm, const IntermediateFiles& inputFiles, const bool deleteOutputFile, Fe& freeEnergy)
{
    const size_t sizeSequence = getSizeOfSequence(inputFiles[OUTPUT_FILE]);
    FileLine aux;
    helper::readLine(inputFiles[OUTPUT_FILE], LINE_NO, aux);

    std::string str;
    helper::readValue(aux, 0, sizeSequence, str);
    parseStructure(str, structureRNAm);
    readFreeEnergy(aux, sizeSequence, freeEnergy);
    if (deleteOutputFile)
    {
        deleteObsoleteFiles(inputFiles[INPUT_FILE]);
        deleteObsoleteFiles(inputFiles[OUTPUT_FILE]);
    }
}

Fe RNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    return 0; //temporal
}

Fe RNAFold::foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    return 0; //temporal
}

} //namespace fideo