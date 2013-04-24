/*
 * @file   FideoConfig.h
 * @brief  FideoConf provides the interface that allows setting the path to the executable.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing class FideoConfig.
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

#ifndef _FIDEO_CONFIG_H
#define _FIDEO_CONFIG_H

#include <string>
#include <map>

namespace fideo
{

///Map between execute name and path file.<executable,path>
typedef std::map<std::string, std::string> configuration;

class FideoConfig
{
public:
    /** @brief Get concrete instance
     *
     * @return a concrete instance of this.
     */
    static FideoConfig* getInstance();

    /** @brief Get specific path
     *
     * Method responsible for searching the executabe path corresponding to  a given executable name
     * @param  exec: executable name
     * @return 
     */
    void getPath(const std::string& exec, std::string& executablePath) const;

    /** @brief Destructor of class
     *
     */
    ~FideoConfig();

private:   	
	/** @brief Read diferent paths
     *
     * Read diferent paths and fill configuration map 
     * @return void
     */
    void fillConfigurationDataFromFile();

     /** @brief Constructor of class
     *
     * Method responsible for building the class. Read the configuration file
     * and complete the configuration map
     */
    FideoConfig();

	/// Path of configuration file
    static const std::string FILE_NAME;

    configuration config;

	/// Concrete instance of FideoConfig class
    static FideoConfig* instance;

}; // FideoConfig
} // fideo

#endif  /* _FIDEO_CONFIG_H */

