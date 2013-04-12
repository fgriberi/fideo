/*
 * File:   IFold.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *		   Franco Riberi <fgriberi at gmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _IFOLD_H
#define _IFOLD_H

#include <biopp/biopp.h>
#include "rna_backends_types.h"
#include "rna_backends_exceptions.h"

namespace fideo
{
/**
 * Interface for sequence's folding services.
 */
struct IFold
{
public:
    /**
     * Fold an ARN sequence
     * @param sequence the ARN sequence to fold.
     * @param structure the structure where to write the folding.
     * @param circ if the structure it's circular.
     * @return The free energy in the structure.
     */
    virtual Fe fold(const biopp::NucSequence& seqRNAm, biopp::SecStructure& structureRNAm, bool isCircRNAm) const = 0;

	/**
	 * Class destructor
	 */
    virtual ~IFold() {}

	/**
	 * Method that provide the available backends for hybridize service.
	 * @param List to fill with different backends
     * @return 
	 */	
	void getAvailableBackends(StringList& slist) const
	{
		mili::Factory<std::string,IFold>::KeyIterator it(mili::FactoryRegistry<IFold, std::string>::getConstructibleObjectsKeys());
		while(!it.end())
	    {
    	    slist.push_back(*it);
    	    it++;
    	}
	}
};
}
#endif  /* _IFOLD_H */
