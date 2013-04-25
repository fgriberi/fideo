/*
 * @file   FideoHelper.h
 * @brief  This header defines necessary methods in fideo
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing necessary methods
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date September 26, 2010, 5:25 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, FuDePAN
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef FIDEO_HELPER_H
#define FIDEO_HELPER_H

#include <fstream>
#include <cerrno>
#include <unistd.h>
#include "RnaBackendsException.h"

namespace fideo
{

/**
 * Filename path
 */
typedef std::string FilePath;

typedef std::string FileLine;

typedef std::string Command;

typedef unsigned int FileLineNo;

typedef std::list<std::string> FileLinesCt;

typedef std::list<std::string> Backend;

namespace helper
{
//-----------------------------------System methods----------------------------------- 

/** @brief execute a Command
 *
 * Execute a give Command using a system call
 * @param cmd: the RNA Backend Command
 * @return int: status 
 */
int runCommand(const Command& cmd);

//-----------------------------------File Methods----------------------------------- 

/** @brief Create a temporal file
 * 
 * @param nameTmpFile: to fill with name file created
 * @return void
 */
void createTmpFile(std::string& nameTmpFile);

/** @brief Remove a file
 * 
 * @param fileName: name file to remove
 * @return void
 */
void removeFile(const std::string& fileName);

/** @brief Write a file with multiple lines.
 *
 * @param file: file path
 * @param lines: lines to write
 * @return void
*/
void write(const FilePath& file, FileLinesCt& lines);

/** @brief Write a file with a single line.
 * 
 * @param file: file path
 * @param line: line to write
 * @return void
 */
void write(const FilePath& file, FileLine& line);

/** @brief Read a line from a file
 * 
 * @param file: file path
 * @param lineno: line number to read
 * @param line: where to write the read line
 * @return void
*/
void readLine(const FilePath& file, FileLineNo lineno, FileLine& line);

#define FIDEO_HELPER_INLINE_H
#include "FideoHelperInline.h"
#undef FIDEO_HELPER_INLINE_H

} //namespace helper
} //namespace fideo

#endif  /* FIDEO_HELPER_H */
