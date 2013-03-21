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

#include <iostream>
#include <fstream>
#include <mili/mili.h>
#include "fideo/FideoConfig.h"
#include "fideo/RNABackendProxy.h"

using namespace std;
using namespace mili;

const string FideoConfig::FILE_NAME = "/home/gringusi/Escritorio/Tesis/fbuild/fudepan-build/install/.paths";
FideoConfig* FideoConfig::instance = NULL;

FideoConfig* FideoConfig::getInstance()
{
    if (instance== NULL) {
        instance = new FideoConfig();
    }
    return instance;
}


void FideoConfig::readPathsFile()
{
    ifstream pathsFile;
    pathsFile.open(FILE_NAME.c_str());
    if (!pathsFile)
        throw RNABackendException("The file cannot be read.");
    else
    {
        string temp;
        while (getline(pathsFile,temp).good())
        {
            stringstream ss(temp);
            vector<string> result;
            ss >> Separator(result, ' ');
            config.insert(pair<string,string>(result[0],result[1])); //<executable,path>
        }
    }
    pathsFile.close();
}

string FideoConfig::getPath(const string& exec)
{
    const map<string, string>::const_iterator it = getInstance()->config.find(exec);
    if ( it == getInstance()->config.end())
        throw RNABackendException("Path not specified in the file <.paths>");
    else
        return it->second;
}
