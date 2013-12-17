/*
 * @file     IMotifObserver.h
 * @brief    IMotifObserver provides the interface to folding service.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing struct IMotifObserver.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date May 11, 2013
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

#ifndef _IMOTIF_OBSERVER_H
#define _IMOTIF_OBSERVER_H

namespace fideo
{

/** @brief Interface for process motif services.
 *
 */
struct IMotifObserver
{
    /** @brief Represent a Motif
     *
     */
    struct Motif
    {
        std::string nameMotif;
        size_t attribute;
        size_t amountStacks;
    };

    /** @brief Start observer
     *
     * @return void
     */
    virtual void start() = 0;

    /** @brief Processes the motif
     *
     * @param motif: current motif
     * @return void
     */
    virtual void processMotif(const Motif& Motif) = 0;

    /** @brief Finalize observer
     *
     * @return void
     */
    virtual void finalize() = 0;

    /** @brief Destructor of class
     *
     */
    virtual ~IMotifObserver() {}
};

} //namespace fideo

#endif  /* _IMOTIF_OBSERVER_H */
