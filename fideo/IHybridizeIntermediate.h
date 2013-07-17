/*
 * @file     IHybridizeIntermediate.h
 * @brief    Provides the interface to modularize hybridize service.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing struct IHybridizeIntermediate.
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

#ifndef _IHYBRIDIZE_INTERMEDIATE_H
#define _IHYBRIDIZE_INTERMEDIATE_H

#include <vector>
#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include "fideo/IHybridize.h"
#include "fideo/IMotifObserver.h"

namespace fideo
{

class IHybridizeIntermediate : public IHybridize
{
public:

    /** @brief Represent intermediate files (input files and output files)
    *
    */
    typedef std::vector<std::string> IntermediateFiles;

    /** @brief Represent a string separated by a char
    *
    */
    typedef std::vector<std::string> ResultLine;

    /** @brief Hybridize an RNA sequence
     *
     * @param longerSeq: longer sequence the RNA sequence to Hybridize.
     * @param shorterSeq: shorter sequence the RNA sequence to Hybridize
     * @param longerCirc: if the longerSeq it's circular.
     * @return The free energy.
     */
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const bool longerCirc,
                         const biopp::NucSequence& shorterSeq) const
    {
        if (longerCirc)
        {
            throw RNABackendException("Unsupported Sequence.");
        }
        IntermediateFiles files;
        etilico::Command cmd;
        prepareData(longerSeq, shorterSeq, cmd, files);
        etilico::runCommand(cmd);

        Fe freeEnergy;
        processingResult(files, freeEnergy);
        return freeEnergy;
    }

    /** @brief Destructor of class
    *
    */
    virtual ~IHybridizeIntermediate() {}

    static const size_t FILE_1 = 0;
    static const size_t FILE_2 = 1;
    static const size_t FILE_3 = 2;

private:

    /** @brief Prepare the necessary data for hybridize service
    *
    * @param longerSeq: longer sequence to Hybridize.
    * @param shorterSeq: shorter sequence to Hybridize
    * @param command: to fill with execute Command
    * @param outputFiles: created temporal file names
    * @return void
    */
    virtual void prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                             etilico::Command& command, IntermediateFiles& outputFiles) const = 0;

    /** @brief  Processing hybridize results
     *
     * @param inputFiles: temporal file names
     * @param freeEnergy: to fill with free energy
     * @return void
     */
    virtual void processingResult(const IntermediateFiles& inputFiles, Fe& freeEnergy) const = 0;
};

} //namespace fideo
#endif  /* _IHYBRIDIZE_INTERMEDIATE_H */