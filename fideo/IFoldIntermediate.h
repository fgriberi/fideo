/*
 * @file     IFoldIntermediate.h
 * @brief    Provides the interface to modularize fold service.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing struct IFoldIntermediate.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date September 09, 2013
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _IFOLD_INTERMEDIATE_H
#define _IFOLD_INTERMEDIATE_H

#include <vector>
#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include "fideo/IFold.h"
#include "fideo/IMotifObserver.h"

namespace fideo
{

/** @brief Represents intermediate files (input files and output files)
 *
 */
typedef std::vector<FilePath> IntermediateFiles;

class IFoldIntermediate : public IFold
{
public:

    /** @brief Represent a string separated by a char
     *
     */
    typedef std::vector<std::string> ResultLine;

    /** @brief Fold an RNA sequence and deletes all file generated
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm);

    /** @brief Fold an RNA sequence and deletes all file generated
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param motifObserver: specific implementation of IMotifObserfer
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver) = 0;

    /** @brief Fold the RNA sequence and load result in a specific file
     *
     * @param seqRNAm: input RNA sequence.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param outputFile: generate output file
     * @return void
     */
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, FilePath& outputFile);

    /** @brief Fold the RNA sequence and load result in a specific file with observer
     *
     * @param seqRNAm: input RNA sequence.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param outputFile: generate output file
     * @param motifObserver: specific implementation of IMotifObserfer
     * @return void
     */
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, FilePath& outputFile, IMotifObserver* motifObserver) = 0;

    /** @brief Fold a specific file
     *
     * @param inputFile: input file to fold
     * @param structureRNAm: the structure where to write the folding.
     * @return free energy
     */
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm);

    /** @brief Fold a specific file with motif
     *
     * @param inputFile: input file to fold
     * @param structureRNAm: the structure where to write the folding.
     * @param motifObserver: specific implementation of IMotifObserfer
     * @return free energy
     */
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver) = 0;

    /** @brief Destructor of class
     *
     */
    virtual ~IFoldIntermediate() {}

    static const size_t INPUT_FILE = 0;
    static const size_t OUTPUT_FILE = 1;

private:

    /** @brief Call external tool to fold
     *
     * @param sequence: the RNA sequence to fold.
     * @param isCirc: if the sequence's circular.
     * @param structure: secondary structure
     * @param files: files generated
     * @return void
     */
    void commonFold(const biopp::NucSequence& sequence, const bool isCirc, biopp::SecStructure& structure, IntermediateFiles& files);

    /** @brief Prepare the necessary data for folding service
     *
     * @param sequence: the RNA sequence to fold.
     * @param isCirc: if the sequence's circular.
     * @param command: to fill with execute Command
     * @param outputFiles: temporary file names created
     * @return void
     */
    virtual void prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, IntermediateFiles& outputFiles) = 0;

    /** @brief Processing folding results
     *
     * @param structureRNAm: structure to fill
     * @param sizeSequence: size of sequence
     * @param freeEnergy: to fill with free energy
     * @param inputFiles: temporal file names
     * @return void
     */
    virtual void processingResult(biopp::SecStructure& structureRNAm, const IntermediateFiles& inputFiles, const bool deleteOutputFile, Fe& freeEnergy) = 0;

    /** @brief Delete all obsolete files
     *
     * @param nameFile: file name
     * @return void
     */
    virtual void deleteObsoleteFiles(const std::string& nameFile) = 0;

    /** @brief Rename specific files
     *
     * @param nameFile: name file to rename
     * @param newNameFile: new name of file
     * @return void
     */
    virtual void renameNecessaryFiles(const std::string& nameFile, const std::string& newNameFile) = 0;

protected:

    /** @brief Rename a file
     *
     * @param fileToRename: The original name file
     * @param nameFile: The new name
     * @param void
     */
    void renameFile(const std::string& fileToRename, const std::string& nameFile);

    /** @brief Get the name of sequence. This is a substring in the description sequence
     *
     * @param inputName: string to parse
     * @param nameSequence: name of sequence
     * @return void
     */
    void getNameOfSequence(const std::string& inputName, std::string& nameSequence);
};

} //namespace fideo
#endif  /* _IFOLD_INTERMEDIATE_H */