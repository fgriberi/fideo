/*
 * File:   RNABackendProxy.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *         Franco Riberi   <fgriberi at gmail.com>
 *
 * Created on October 26, 2010, 2012, 4:31 PM
 *
 * Copyright (C) 2010  Santiago Videla, Franco Riberi FuDePAN
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

#ifndef _RNABACKENDPROXY_H
#define _RNABACKENDPROXY_H

#include <fstream>
#include "rna_backends_types.h"

using std::string;

/**
 * Execute a give command using a system call
 * @param cmd the RNA backend command
 */
int runCommand(const Command& cmd);

/**
 * Return temp file name
 * @param
*/
inline FilePath get_temp_file()
{
    char nameBuff[32] = "/tmp/myTmpFile-XXXXXX";     // buffer to hold the temporary file name
    int fileDescriptor = -1;
    fileDescriptor = mkstemp(nameBuff);              // Create the temporary file, this function will replace the 'X's
    //unlink(nameBuff);                              //Call unlink so that whenever the file is closed or the program exits
    // the temporary file is deleted
    if (fileDescriptor < 1)
        throw RNABackendException("Creation of temp file failed with error.");
    else
        return nameBuff;
}

/**
 * Return input file name
 * @param
*/
inline const FilePath get_input_file_name()
{
    return "fold.in";
}

/**
 * Return output file name
 * @param
*/
inline const FilePath get_output_file_name()
{
    return "fold.out";
}

void remove_file(const std::string& file_name);

/**
 * Write a file with multiple lines.
 * @param file the file path
 * @param lines the lines to write
 */
void write(const FilePath& file, FileLinesCt& lines);

/**
 * Write a file with a single line.
 * @param file the file path
 * @param line the line to write
 */
void write(const FilePath& file, FileLine& line);

/**
 * Read a line from a file
 * @param file the file path
 * @param lineno the line number to read
 * @param line where to write the read line
 */
void read_line(const FilePath& file, FileLineNo lineno, FileLine& line);

/**
 * Read a value from a file line using offset and length
 * @param line the file line
 * @param offset the offset in the line
 * @param n the length to read
 * @param t where to write the value.
 */
template<class T>
inline void read_value(const FileLine& line, FileLine::size_type offset, size_t n, T& t)
{
    const bool success = mili::from_string(line.substr(offset, n), t);
    if (!success)
        throw RNABackendException("Could not read the value from given line, offset and length");
}

/**
 * Read a value from a line file.
 * @param line the file line
 * @param t where to write the value
 */
template<class T>
inline void read_value(const FileLine& line, T& t)
{
    const bool success = from_string(line, t);
    if (!success)
        throw RNABackendException("Could not read the value from given line");
}

template<class T>
void convert_from_string(const std::string& from, T& to)
{
    if (!mili::from_string(from, to))
        throw RNABackendException("Wrong column type.");
}

#endif  /* _RNABACKENDPROXY_H */

