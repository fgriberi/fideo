/*
 * @file     IFold.h
 * @brief    IFold provides the interface to folding service.
 *
 * @author   Santiago Videla
 * @email    santiago.videla AT gmail.com
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing struct IFold.
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

#ifndef _IFOLD_H
#define _IFOLD_H

#include <fstream>
#include <cerrno>
#include <unistd.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include "fideo/RnaBackendsTypes.h"
#include "fideo/FideoHelper.h"
#include "fideo/IMotifObserver.h"

namespace fideo
{

struct IFold;

/** @brief Represent a factory registry type
 *
 */
typedef mili::FactoryRegistry<IFold, std::string> Fold;

/** @brief Interface for sequence's folding services.
 *
 */
struct IFold
{
    typedef mili::Factory<std::string, IFold> Factory;    

    /** @brief Fold an RNA sequence and deletes all file generated
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param temp: temperature to fold. By default is 37 grades.
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const Temperature temp = 37) = 0;

    /** @brief Fold an RNA sequence and deletes all file generated. Using observer
     *
     * @param seqRNAm: the RNA sequence to fold.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param motifObserver: specific implementation of IMotifObserfer
     * @param temp: temperature to fold. By default is 37 grades.
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver, const Temperature temp = 37) = 0;

    /** @brief Fold the RNA sequence and load result in a specific file
     *
     * @param seqRNAm: input RNA sequence.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param outputFile: generate output file
     * @param temp: temperature to fold. By default is 37 grades.
     * @return void
     */
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const FilePath& outputFile, const Temperature temp = 37) = 0;

    /** @brief Fold the RNA sequence and load result in a specific file with observer
     *
     * @param seqRNAm: input RNA sequence.
     * @param isCircRNAm: if the structure it's circular.
     * @param structureRNAm: the structure where to write the folding.
     * @param outputFile: generate output file
     * @param motifObserver: specific implementation of IMotifObserfer
     * @param temp: temperature to fold. By default is 37 grades.
     * @return void
     */
    virtual void foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, const FilePath& outputFile, IMotifObserver* motifObserver, const Temperature temp = 37) = 0;

    /** @brief Fold a specific file
     *
     * @param inputFile: input file to fold
     * @param structureRNAm: the structure where to write the folding.
     * @return free energy
     */
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm) = 0;

    /** @brief Fold a specific file with observer
     *
     * @param inputFile: input file to fold
     * @param structureRNAm: the structure where to write the folding.
     * @param motifObserver: specific implementation of IMotifObserfer
     * @return free energy
     */
    virtual Fe foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver) = 0;

    /** @brief Class destructor
     *
     */
    virtual ~IFold() {}

    /** @brief Get availables backend
     *
     * Method that provide the available backends for folding service.
     * @param slist: to fill with different backends
     * @return void
     */
    static void getAvailableBackends(Backend& slist)
    {
        Factory::KeyIterator it(Fold::getConstructibleObjectsKeys());
        while (!it.end())
        {
            slist.push_back(*it);
            it++;
        }
    }
};

} //namespace fideo
#endif  /* _IFOLD_H */
