/*
 * @file   FideoConfig.cpp
 * @brief  This is the implementation of FideoConf interface and is responsible for
 *         setting the path to the executable
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing class FideoConfig implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date March 13, 2012, 9:50 PM
 *
 * Copyright (C) 2010  Franco Riberi, FuDePAN
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

#include "fideo/FideoConfig.h"
#include "fideo/FideoHelper.h"

namespace fideo
{

/// Path of configuration file
const std::string FideoConfig::FILE_NAME = "/home/gringusi/Escritorio/Tesis/fbuild/fudepan-build/install/.paths";

///Initial concrete instance
FideoConfig* FideoConfig::instance = NULL;

FideoConfig::FideoConfig()
{
    readPathsFile();
}

FideoConfig* FideoConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new FideoConfig();
    }
    return instance;
}

void FideoConfig::getPath(const std::string& exec, std::string& executablePath) const
{
    const configuration::const_iterator it = getInstance()->config.find(exec);
    if (it == instance->config.end())
    {
        executablePath = string();        
    }
    else
    {
        executablePath = it->second;
    }
}

void FideoConfig::readPathsFile() 
{
    std::ifstream pathsFile;
    pathsFile.open(FILE_NAME.c_str());
    if (!pathsFile)
    {
		throw FileReadException();        
    }
    else
    {
        std::string line;
        while (getline(pathsFile, line))
        {
            std::stringstream ss(line);
            std::vector<std::string> result;
            ss >> mili::Separator(result, ' ');
            config[result[0]] = result[1]; //<executable,path>            
        }
    }
}
} // namespace fideo
