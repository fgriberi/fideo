/*
 * @file   FideoHelper.cpp
 * @brief  This is the implementation of FideoHelper interface.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing class FideoHelper implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date April 2013
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
 *
 * This file is part of fideo.
 *
 * fideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of tghe License, or
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

#include "fideo/FideoHelper.h"

namespace fideo
{
namespace helper
{
static const int SYSTEM_ERROR = -1;
static const int FILE_ERROR = -1;

int runCommand(const command& cmd)
{
    const int status = system(cmd.c_str());
    if (status == SYSTEM_ERROR)
    {
		throw SystemCallException();	        
    }
    else
    {
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            if (WIFSIGNALED(status))
            {
                throw RNABackendException("Termination signal " + mili::to_string(WTERMSIG(status)) + " in " + cmd);
            }
            else
            {
				throw NonTerminationException();                
            }
        }
    }
}

void createTmpFile(std::string& nameTmpFile)
{
    char nameFileAux[] = "/tmp/myTmpFile-XXXXXX";
    const int fileDescriptor = mkstemp(nameFileAux);
    if (fileDescriptor < 1)
    {
        throw TmpFileCreateException();
    }
    nameTmpFile = nameFileAux;
}

void removeFile(const std::string& file_name)
{
    if (unlink(file_name.c_str()) == FILE_ERROR)
    {
        throw RNABackendException("Error in unlink of '" + file_name + "': " + std::string(strerror(errno)));
    }
}

void write(const filePath& file, fileLinesCt& lines)
{
    std::ofstream out;
    out.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        out.open(file.c_str());
        mili::CAutonomousIterator<fileLinesCt> it(lines);
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

void write(const filePath& file, fileLine& line)
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

void read_line(const filePath& file, fileLineNo lineno, fileLine& line)
{
    std::ifstream in;
    in.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        in.open(file.c_str());
        if (lineno > 0)
        {
            std::string aux;
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

} //namespace helper
} //namespace fideo
