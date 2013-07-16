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
typedef std::vector<std::string> IntermediateFiles;

class IFoldIntermediate : public IFold
{		
public:

    /** @brief Represent a string separated by a char
    *
    */
    typedef std::vector<std::string> ResultLine;

    /** @brief Fold a RNA sequence
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the sequence's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm)
    {
        structureRNAm.clear();
        structureRNAm.set_circular(isCircRNAm);   
        etilico::Command cmd;
        IntermediateFiles files;
        prepareData(seqRNAm, isCircRNAm, cmd, files);    
        etilico::runCommand(cmd);
        Fe freeEnergy;
        processingResult(isCircRNAm, structureRNAm, seqRNAm.length(), files, freeEnergy);
        return freeEnergy;    
    }

    /** @brief Fold a RNA sequence
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the sequence's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param motif: specific implementation of IMotifObserfer
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver) = 0;

    /** @brief Destructor of class
    *
    */
    virtual ~IFoldIntermediate() {} 

    static const size_t INPUT_FILE = 0;    
    static const size_t OUTPUT_FILE = 1;
    
private:

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
     * @param isCirc: the sequence analyzed is circular
     * @param structureRNAm: structure to fill
     * @param sizeSequence: size of sequence
     * @param freeEnergy: to fill with free energy
     * @param inputFiles: temporal file names 
     * @return void
     */
	virtual void processingResult(const bool isCirc, biopp::SecStructure& structureRNAm, size_t sizeSequence, const IntermediateFiles& inputFiles, Fe& freeEnergy) = 0;
};

} //namespace fideo
#endif  /* _IFOLD_INTERMEDIATE_H */