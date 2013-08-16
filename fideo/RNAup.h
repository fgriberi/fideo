/*
 * @file     RNAup.h
 * @brief    Provides the interface to hibridize service using RNAup.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing RNAup interface.
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

#ifndef RNA_UP_H
#error Internal header file, DO NOT include this.
#endif

#include <etilico/etilico.h>
#include "fideo/IHybridizeIntermediate.h"

namespace fideo
{

using mili::operator>>;

/** @brief RNAup is an implementation of IHybridize interface that use Vienna package
 *
 */
class RNAup : public IHybridizeIntermediate
{
private:

    virtual void prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, etilico::Command& command, IntermediateFiles& outputFiles) const;
    virtual void processingResult(const IntermediateFiles& inputFiles, Fe& freeEnergy) const;

    /** @brief Destructor of class
     *
     */
    virtual ~RNAup() {}

    /** @brief Class that allows parsing the body of a file
    *
    */
    class BodyParser
    {
    public:
        void parse(File& file);

        Fe _dG; ///free energy
    private:

        /** @brief Represents the columns of the file to parse
         *
         */
        enum Columns
        {
            ColRNAduplexResults,
            ColIndiceIJ,
            ColTwoPoints,
            ColIndiceKL,
            ColdGTotal,
            ColEqualSymbol,
            ColdGInt,
            ColPlusSymbol,
            ColdGu_l,
            NumberOfColumns
        };
    };
};

} //namespace fideo