/*
 * File:   FideoConfig.cpp
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on November 02, 2012, 19:35 PM
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "fideo/FideoConfig.h"
#include "fideo/FideoHelper.h"

using namespace std;
using namespace mili;

namespace fideo
{
const string FideoConfig::FILE_NAME = "/home/gringusi/Escritorio/Tesis/fbuild/fudepan-build/install/.paths";
FideoConfig* FideoConfig::instance = NULL;

FideoConfig* FideoConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new FideoConfig();
    }
    return instance;
}


void FideoConfig::readPathsFile()
{
    ifstream pathsFile;
    pathsFile.open(FILE_NAME.c_str());
    if (!pathsFile)
    {
        throw RNABackendException("The configure file couldn't be read.");
    }
    else
    {
        string line;
        while (getline(pathsFile, line))
        {
            stringstream ss(line);
            vector<string> result;
            ss >> Separator(result, ' ');
            config[result[0]] = result[1]; //<executable,path>
        }
    }
}

string FideoConfig::getPath(const string& exec)
{
    const configuration::const_iterator it = getInstance()->config.find(exec);
    if (it == getInstance()->config.end())
    {
        return string();
    }
    else
    {
        return it->second;
    }
}
}
