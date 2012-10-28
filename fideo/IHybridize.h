/*
 * File:   IHybridize.h
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on October 26, 2012, 7:37 PM
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

#include <biopp/biopp.h>
#include "rna_backends_types.h"

/**
 * Interface for sequence's hybridize services.
 */
class IHybridize
{
public:
    /**
     * Hybridize an RNA sequence
     * @param firt sequence the RNA sequence to Hybridize.
     * @param second sequence the RNA sequence to Hybridize
     * @return The free energy.
     */
    virtual Fe hybridize(const biopp::NucSequence& firstSeq, const biopp::NucSequence& secondSeq) const = 0;

    /**
     * Destroyer
     */
    virtual ~IHybridize() {}
};

#endif  /* _IHYBRIDIZE_H */
