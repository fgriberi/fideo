/*
 * File:   FideoConfig.h
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on March 13, 2012, 9:50 PM
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

#ifndef _FIDEO_CONFIG_H
#define _FIDEO_CONFIG_H

#include <string>
#include <map>

typedef std::map<std::string, std::string> configuration;

class FideoConfig
{
    /*
     * Path of configuration file
     */
    static const std::string FILE_NAME;

	/*
     * Map between execute file and path file.
     */
	configuration config;

    /*
     * Read diferent paths
     */
    void readPathsFile();

    /*
     * Constructor of class
     */
    FideoConfig()
    {
        readPathsFile();
    }

    /*
     * Concrete instance of class
     */
    static FideoConfig* instance;

public:
    /*
     * Get concrete instance
     */
    static FideoConfig* getInstance();

    /*
     * Get specific path
     */
    std::string getPath(const std::string& exec);

    /*
     * Destructor of class
     */
    ~FideoConfig() {};

};

#endif  /* _FIDEO_CONFIG_H */