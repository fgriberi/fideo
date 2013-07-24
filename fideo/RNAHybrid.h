/*
 * @file     RNAHybrid.h
 * @brief    Provides the interface to hibridize service using RNAHybrid.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing RNAHybrid interface.
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

#ifndef RNA_HYBRID_H
#error Internal header file, DO NOT include this.
#endif

#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include "fideo/IHybridizeIntermediate.h"

namespace fideo
{

/** @brief RNAHybrid is an implementation of IHybridize interface
 *
 */
class RNAHybrid : public IHybridizeIntermediate
{
private:

    virtual void prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, etilico::Command& command, IntermediateFiles& outputFiles) const;
    virtual void processingResult(const IntermediateFiles& inputFiles, Fe& freeEnergy) const;

    /** @brief Destructor of class
     *
     */
    virtual ~RNAHybrid() {}

    static const size_t OBSOLETE_LINES = 6;

    /** @brief Class that allows parsing the body of a file
     *
     */
    class BodyParser
    {
    public:

        /** @brief Parse the file and get the value dG
         *
         * @param file: file to parser
         * @return void
         */
        void parse(File& file);

        Fe dG; ///free energy
        static const size_t OBSOLETE_dG = 1000; //no significant hybridization found
        static const size_t SIZE_LINE = 3;
        static const size_t DELTA_G = 1;
    };
};

} //namespace fideo