/*
 * @file     RNAFold.h
 * @brief    Provides the interface to folding service using RNAFold.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing RNAFold interface.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date July 15, 2013
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
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

#ifndef RNA_FOLD_H
#error Internal header file, DO NOT include this.
#endif

#include <etilico/etilico.h>
#include "fideo/IFoldIntermediate.h"

namespace fideo
{

/** @brief RNAFold is an implementation of IFold interface that use Vienna package
 *
 */
class RNAFold : public IFoldIntermediate
{
private:

    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver, const Temperature temp = 37);
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const FilePath& outputFile, IMotifObserver* motifObserver, const Temperature temp = 37);
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver);
    virtual void prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, InputFile& inputFile, OutputFile& outputFile, const Temperature temp = 37);
    virtual void processingResult(biopp::SecStructure& structureRNAm, const InputFile& inputFile, Fe& freeEnergy);
    virtual void deleteAllFilesAfterProcessing(const InputFile& inFile, const OutputFile& outFile);
    virtual void deleteObsoleteFiles(const InputFile& inFile);
    virtual void renameNecessaryFiles(const std::string& fileToRename, const std::string& newNameFile);

    /** @brief Destructor of class
     *
     */
    virtual ~RNAFold() {}

    /** @brief Read free energy of line
     *
     * @param file: to read
     * @param energy: to fill with free energy
     * @return index of first ")" in file
     */
    size_t readFreeEnergy(FileLine& file, size_t offset, Fe& energy) const;

    /** @brief Obtain secondary structure
     *
     * @param str: to parse
     * @param secStrucute: to fill with structure
     * @return void
     */
    static void parseStructure(std::string& str, biopp::SecStructure& secStructure);

    /** @brief Gets the size of the folded sequence from a file
     *
     * @param file: input file to read
     * @return size of sequence
     */
    size_t getSizeOfSequence(const FilePath& file) const;

    static const FileLineNo LINE_NO;
    static const char OPEN_PAIR = '(';
    static const char CLOSE_PAIR = ')';
    static const char UNPAIR = '.';
};

} //namespace fideo
