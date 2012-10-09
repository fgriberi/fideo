/*
 * File:   rna_backends_types.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *         Franco Riberi <fgriberi at gmail.com>
 *
 * Created on October 27, 2010, 2012 6:42 PM
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

#include <set>
#include <vector>

#include "rna_backends_exceptions.h"

/**
 * Distance between sequences
 */
typedef float Distance;

/**
 * Similitude between sequences or structures
 */
typedef float Similitude;

/**
 * Free eneregy in secodary structure
 */
typedef float Fe;

/**
 * Container of raw (string) RNA sequences
 */
typedef std::set<std::string> StringSequencesCt;

/**
 * Number of combination attemps.
 */
typedef unsigned int CombinationAttempts;

/**
 * Filename path
 */
typedef std::string FilePath;

typedef std::string FileLine;

typedef std::string Command;

typedef unsigned int FileLineNo;

typedef std::list<std::string> FileLinesCt;

#define SUCCESS_EXEC 0
#define MATRIX_SIZE 4

#endif  /* _RNA_BACKENDS_TYPES_H */

