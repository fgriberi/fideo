/*
 * File:   RNABackendProxy.cpp
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *         Franco Riberi <fgriberi at gmail.com>
 *
 * Created on November 10, 2010, 2012 4:26 PM
 *
 * Copyright (C) 2010  Santiago Videla, Franco Riberi, FuDePAN
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

#include <errno.h>
#include "fideo/RNABackendProxy.h"

namespace fideo
{
static const int SYSTEM_ERROR = -1;
static const int FILE_ERROR = -1;

int runCommand(const Command& cmd)
{
    const int status = system(cmd.c_str());
    if (status == SYSTEM_ERROR)
        throw RNABackendException("System call failed");
    else
    {
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
        {
            if (WIFSIGNALED(status))
                throw RNABackendException("Termination signal " + mili::to_string(WTERMSIG(status)) + " in " + cmd);
            else
                throw RNABackendException("Non termination for some reason");
        }
    }
}

void remove_file(const std::string& file_name)
{
    if (unlink(file_name.c_str()) == FILE_ERROR)
        throw RNABackendException("Error in unlink of '" + file_name + "': " + std::string(strerror(errno)));
}

void write(const FilePath& file, FileLinesCt& lines)
{
    std::ofstream out;
    out.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        out.open(file.c_str());
        mili::CAutonomousIterator<FileLinesCt> it(lines);
        while (!it.end())
        {
            out << *it << std::endl;
            ++it;
        }
    }
    catch (const std::ifstream::failure& e)
    {
        throw RNABackendException("An error ocurred trying to write " + file);
    }
}

void write(const FilePath& file, FileLine& line)
{
    std::ofstream out;
    out.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        out.open(file.c_str());
        out << line << std::endl;
    }
    catch (const std::ifstream::failure& e)
    {
        throw RNABackendException("An error ocurred trying to write " + file);
    }
}

void read_line(const FilePath& file, FileLineNo lineno, FileLine& line)
{
    std::ifstream in;
    in.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        in.open(file.c_str());
        if (lineno > 0)
        {
            string aux;
            for (size_t i = 0; i < lineno; ++i)
            {
                getline(in, aux);
            }
        }
        getline(in, line);
    }
    catch (const std::ifstream::failure& e)
    {
        throw RNABackendException("An error ocurred trying to read " + file);
    }
}
