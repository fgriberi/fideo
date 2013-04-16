/*
 * File:   HelperTest.h
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on April 2013.
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
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
 * along with vac-o.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _HELPER_TEST_H
#define _HELPER_TEST_H

#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <mili/mili.h>
#include <fideo/rna_backends_exceptions.h>

using namespace std;

typedef vector<string> ParserResult;

class HelperTest
{
public:
    static bool isMyTmpFile(const std::string& fileTmpName);
    static bool checkDirTmp();
};

#endif  /* _HELPER_TEST_H */
