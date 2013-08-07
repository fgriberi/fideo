/*
 * File:   IFoldInverse.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on September 26, 2010, 5:25 PM
 *
 * Copyright (C) 2010  Santiago Videla, FuDePAN
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

#ifndef _IFOLDINVERSE_H
#define _IFOLDINVERSE_H

#include <mili/mili.h>
#include <biopp/biopp.h>
#include "fideo/IStartProvider.h"
#include "fideo/RnaBackendsTypes.h"



namespace fideo
{

struct InverseFoldParams;
class IStartProvider;

/**
 * Interface for sequence's inverse folding services.
 */
struct IFoldInverse
{
    static IFoldInverse* factory(const std::string& backend, const InverseFoldParams& param) 
    {
        return mili::FactoryRegistry<IFoldInverse, std::string, const InverseFoldParams&>::new_class(backend, param);
    }

    /**
     * Make the inverse folding
     * @param sequence the NucSequence to write to.
     */
    virtual void fold_inverse(biopp::NucSequence&) = 0;
    /**
     * Sets the start sequence for the backend.
     * @param seq the NucSequence.
     */
    virtual void set_start(const biopp::NucSequence&) = 0;

    /**
     * Start the chain of calls to set the start sequence using
     * the given IStartProvider.
     * @param provider a start sequences provider.
     */
    virtual void query_start(IStartProvider*) = 0;

    virtual ~IFoldInverse() {}
};

struct InverseFoldParams
{
    InverseFoldParams(const biopp::SecStructure& structure, Similitude sd, Distance hd, CombinationAttempts ca)
        : structure(structure),
          sd(sd),
          hd(hd),
          ca(ca)
    {}
    const biopp::SecStructure& structure;
    const Similitude sd;
    const Distance hd;
    const CombinationAttempts ca;
};

} //namespace fideo

#endif  /* _IFOLDINVERSE_H */

