/*
 * @file     RNAcofold.h
 * @brief    Provides the interface to hibridize service using RNAcofold.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing RNAcofold interface.
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

#ifndef RNA_COFOLD_H
#error Internal header file, DO NOT include this.
#endif

#include <etilico/etilico.h>
#include "fideo/IHybridizeIntermediate.h"

namespace fideo
{

/** @brief RNAcofold is an implementation of IHybridize interface that use Vienna package
 *
 */
class RNAcofold : public IHybridizeIntermediate
{
private:

    virtual void prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                             etilico::Command& command, InputFiles& inFiles, OutputFile& outFile, const Temperature temp = 37) const;
    virtual void processingResult(const OutputFile& outFile, Fe& freeEnergy) const;
    virtual void deleteObsoleteFiles(const InputFiles& inFiles, const OutputFile& outFile) const;

    /** @brief Destructor of class
     *
     */
    virtual ~RNAcofold() {}

    /** @brief Class that allows parsing the body of a file
     *
     */
    class BodyParser
    {
    public:

        /** @brief Parse the line and get the value dG
         *
         * @param line: line to parser
         * @return void
         */
        void parse(std::string& line);

        Fe _dG; ///free energy

    private:
        enum Columns
        {
            ColOpenParenthesis,
            ColdG,
            NumberOfColumns
        };
    };
};

} //namespace fideo
