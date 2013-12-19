/*
 * @file    rna_backends_types.h
 * @brief   This file contains all the definitions needed in fideo
 *
 * @author   Santiago Videla
 * @email    santiago.videla AT gmail.com
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file providing fideo definitions.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date October 27, 2010, 2012 6:42 PM
 *
 * Copyright (C) 2010  Santiago Videla, Franco Riberi, FuDePAN
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

#ifndef _RNA_BACKENDS_TYPES_H
#define _RNA_BACKENDS_TYPES_H

#include <string>
#include <set>

namespace fideo
{

/**
 * Free eneregy in secodary structure
 */
typedef double Fe;

/**
 * Distance between sequences
 */
typedef float Distance;

/**
 * Similitude between sequences or structures
 */
typedef float Similitude;

typedef std::set<std::string> StringSequencesCt;

/**
 * Container of raw (string) RNA sequences
 */
typedef std::set<std::string> StringSequencesCt;

/**
 * Number of combination attemps.
 */
typedef unsigned int CombinationAttempts;

/**
 * Represents a temperature to fold.
 */
typedef double Temperature;	

}

#endif  /* _RNA_BACKENDS_TYPES_H */
