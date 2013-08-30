/*
 * @file     UNAFold.h
 * @brief    UNAFold is a specific header backend to folding.
 *
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing UNAFold interface.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date September 26, 2010, 5:25 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, FuDePAN
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

#ifndef UNA_FOLD_H
#error Internal header file, Only include in UNAFold.cpp file.
#endif

#include <unistd.h>
#include <map>
#include <etilico/etilico.h>
#include "fideo/IFoldIntermediate.h"

namespace fideo
{

/** @brief UNAFold is an implementation of IFold interface that use UNAFold package
 *
 */
class UNAFold : public IFoldIntermediate
{
private:

    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver);
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const FilePath& outputFile, IMotifObserver* motifObserver);
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver);
    virtual void prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, InputFile& inputFile, OutputFile& outputFile);
    virtual void processingResult(biopp::SecStructure& structureRNAm, const InputFile& inputFile, Fe& freeEnergy);
    virtual void deleteAllFilesAfterProcessing(const InputFile& inFile, const OutputFile& outFile);
    virtual void deleteObsoleteFiles(const InputFile& inFile);
    virtual void renameNecessaryFiles(const std::string& fileToRename, const std::string& newNameFile);

    /** @brief Destructor of class
     *
     */
    virtual ~UNAFold();

    /** @brief Parse .det file using the observer
     *
     * @param file: specific .det file
     * @param observer: specific implementation if IMotifObserver
     * @return void
     */
    void commonParse(const FilePath& file, IMotifObserver* observer);

    /** @brief Class that allows parsing the header of a file
     *
     */
    class HeaderParser
    {
    public:
        /** @brief Parser header file
         *
         * @param file: input file
         * @return void
         */
        void parse(File& file);

        biopp::SeqIndex _numberOfBases;
        Fe _deltaG;
        std::string _sequenceName;

    private:

        /** @brief Represents the header columns of the file to parse
         *
         */
        enum Columns
        {
            ColNumberOfBases,
            ColDeltaGStr,
            ColEqualSymbol,
            ColDeltaG,
            ColSeqName,
            NumberOfColumns
        };
    };

    /** @brief Class that allows parsing the body of a file
     *
     */
    class BodyLineParser
    {
    public:
        /** @brief Parser body line
         *
         * @param file: input file
         * @return true if correct parse, otherwise false
         */
        bool parse(File& file);

        char _nuc;                  /// a nucleotid
        biopp::SeqIndex _nucNumber; /// starts at 1!. Number of nucleotid in sequence
        biopp::SeqIndex _pairedNuc; /// starts at 1!. Nucleotid paired.

    private:

        /** @brief Represents the body columns of the file to parse
         *
         */
        enum Columns
        {
            ColNucleotideNumber,
            ColNucl,
            ColPrevious,
            ColNext,
            ColPairedWith,
            ColNumberOfBases2,
            Col7,  // not used
            Col8,  // not used
            NumberOfColumns
        };
    };

    /** @brief Class that allows parse to '.det' file
     *
     */
    class DetFileParser;

    /** @brief fill structure
     *
     * @param bodyLine: to parse.
     * @param secStructure: structure to fill
     * @return void
     */
    void fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure);

    /** @brief To store the temporal file name generated
     *
     */
    std::string _temporalFileName;
    const static std::string _det;
};

/** @brief constant that represents motif name
 *
 */
static const std::string HELIX         = "Helix";
static const std::string EXTERNAL_LOOP = "External loop";
static const std::string INTERIOR_LOOP = "Interior loop";
static const std::string HAIRPIN_LOOP  = "Hairpin loop";
static const std::string MULTI_LOOP    = "Multi-loop";
static const std::string BULGE_LOOP    = "Bulge loop";
static const std::string STACK         = "Stack";

/** @brief constant that represents specific interior loop
 *
 */
static const std::string ASYMMETRIC = "Interior Asymmetric";
static const std::string SYMMETRIC = "Interior Symmetric";

#define DET_FILE_PARSER_H
#include "fideo/DetFileParser.h"
#undef DET_FILE_PARSER_H

} //namespace fideo