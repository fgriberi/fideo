/*
 * File:   IStructureCmp.h
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

#ifndef _ISTRUCTURECMP_H
#define _ISTRUCTURECMP_H

#include <string>

#include <mili/mili.h>
#include <biopp/biopp.h>
#include "fideo/RnaBackendsTypes.h"

namespace fideo
{

/**
 * Interface for structure's comparing services.
 */
struct IStructureCmp
{
	typedef mili::FactoryRegistry<IStructureCmp, std::string> Factory;

    /**
     * Compare two structures.
     * @param struct1 a secondary structure.
     * @param struct2 another secondary structure.
     * @return The similitude between structures.
     */
    virtual Similitude compare(const biopp::SecStructure& struct1, const biopp::SecStructure& struct2) const = 0;

    virtual ~IStructureCmp() {}
};

} //End namespace
#endif  /* _ISTRUCTURECMP_H */