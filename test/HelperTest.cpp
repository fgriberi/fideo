/*
 * File:   HelperTest.cpp
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

#include <HelperTest.h>

static const std::string DIRECTORY_PATH = "/tmp";

bool HelperTest::isMyTmpFile (const std::string& fileTmpName)  
{
	bool ret = false;
	stringstream ss(fileTmpName);
    ParserResult result;  
    ss >> mili::Separator(result, '-');
    if (result.size() == 2 && result[0] == "myTmpFile")
		ret = true;
    return ret; 
}

bool HelperTest::checkDirTmp()
{
	bool ret = false;
	DIR *dir;
	struct dirent *ent;
	dir = opendir(DIRECTORY_PATH.c_str());
	if (dir == NULL) 
  		throw SeparatorException();
	while ((ent = readdir (dir)) != NULL && !ret) 
	{
		ret = isMyTmpFile(ent->d_name);
	}
	closedir (dir);
	return ret;
}

