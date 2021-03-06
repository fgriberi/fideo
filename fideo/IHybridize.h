/*
 * @file     IHybridize.h
 * @brief    IHybridize provides the interface to hybridize service.
 *
 * @author   Santiago Videla
 * @email    santiago.videla AT gmail.com
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing struct IHybridize.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date October 26, 2012, 7:37 PM
 *
 * Copyright (C) 2012  Franco Riberi, FuDePAN
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

#ifndef _IHYBRIDIZE_H
#define _IHYBRIDIZE_H

#include <fstream>
#include <cerrno>
#include <unistd.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include "fideo/FideoHelper.h"
#include "fideo/RnaBackendsTypes.h"

namespace fideo
{

/** @brief Represent a factory registry type
 *
 */
struct IHybridize;

typedef mili::FactoryRegistry<IHybridize, std::string> Hybridize;

/** @brief Interface for sequence's hybridize services.
 *
 */
struct IHybridize
{

    typedef mili::Factory<std::string, IHybridize> Factory;

    /** @brief Hybridize an RNA sequence
     *
     * @param longerSeq: longer sequence the RNA sequence to Hybridize.
     * @param shorterSeq: shorter sequence the RNA sequence to Hybridize
     * @param longerCirc: if the longerSeq it's circular.
     * @param temp: temperature to hybridize. By default is 37 grades. 
     * @return The free energy.
     */
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const bool longerCirc, const biopp::NucSequence& shorterSeq, const Temperature temp = 37) const = 0;

    /** @brief Class destructor
     *
     */
    virtual ~IHybridize() {}

    /** @brief Get availables backend
     *
     * Method that provide the available backends for hybridize service.
     * @param slist: to fill with different backends
     * @return void
     */
    static void getAvailableBackends(Backend& slist)
    {
        Factory::KeyIterator it(Hybridize::getConstructibleObjectsKeys());
        while (!it.end())
        {
            slist.push_back(*it);
            it++;
        }
    }
};

} //namespace fideo

#endif  /* _IHYBRIDIZE_H */
