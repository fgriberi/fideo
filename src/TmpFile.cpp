/*
 * File:   TmpFile.cpp
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
 * along with vac-o. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <fideo/TmpFile.h>

static const string PATH_TMP = "/tmp/";

TmpFile::TmpFile()
{
    getTmpFile();
}

TmpFile::~TmpFile()
{
    unlink(tmpFileName.c_str());
}

void TmpFile::getTmpFile()
{
    char nameFileAux[] = "/tmp/myTmpFile-XXXXXX";
    const int fileDescriptor = mkstemp(nameFileAux);
    if (fileDescriptor < 1)
        throw RNABackendException("Creation of temp file failed with error.");
    tmpFileName = nameFileAux;
}

FilePath TmpFile::getTmpName()
{
    return tmpFileName;
}

void TmpFile::changeDirectory()
{
    if (chdir(PATH_TMP.c_str()) != 0)
        throw RNABackendException("Invalid path of temp files.");

}
