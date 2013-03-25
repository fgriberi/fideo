/*
 * File:   TmpFile.h
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on November 10, 2010, 4:26 PM
 *
 * Copyright (C) 2012 Franco Riberi, FuDePAN
 *
 * This file is part of fideo.
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
 * along with vac-o. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <cstdlib>
#include "fideo/rna_backends_types.h"
#include "fideo/rna_backends_types.h"

using namespace std;

class TmpFile
{
    static char tmpFileName[32];  // buffer to hold the temporary file name

    /**
    * Method that is responsible for creating a temporary file
    */
    void getTmpFile();

public:
    /**
    * Constructor of class
    */
    TmpFile();

    /**
    * Destructor of class
    */
    ~TmpFile();

    /**
    * Method that returns the name of the temporary file
    */
    const FilePath getTmpName() const;
};
