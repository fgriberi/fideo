/*
 * File:   TmpFile.cpp
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

#include <fideo/TmpFile.h>

char TmpFile::tmpFileName[] = "/tmp/myTmpFile-XXXXXX";

TmpFile::TmpFile()
{
	getTmpFile();
}

TmpFile::~TmpFile()
{
	unlink(tmpFileName); 
}

void TmpFile::getTmpFile()
{
	int fileDescriptor = -1;
	fileDescriptor = mkstemp(tmpFileName);
	if (fileDescriptor < 1)
		throw RNABackendException("Creation of temp file failed with error.");
}

const FilePath TmpFile::getTmpName() const
{
	return tmpFileName;
}

