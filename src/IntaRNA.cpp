/*
 * @file     IntaRNA.cpp
 * @brief    IntaRNA is an implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend IntaRNA implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date November 02, 2012, 19:35 PM
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

#include <unistd.h>
#define INTA_RNA_H
#include "fideo/IntaRNA.h"
#undef INTA_RNA_H

/** @brief Temporal method requerid to execute remo
 *
 * @param derivedKey: name of derived class
 * @return pointer to the base class
 */
fideo::IHybridize* getDerivedHybridize(const std::string& derivedKey)
{
    fideo::IHybridize* const ptr = fideo::Hybridize::new_class(derivedKey);
    mili::assert_throw<fideo::InvalidDerived>(ptr != NULL);
    return ptr;
}

namespace fideo
{

void IntaRNA::BodyParser::parse(File& file)
{
    std::string temp;
    //advance to the required line
    for (size_t i = 0; i < OBSOLETE_LINES; ++i)
    {
        getline(file, temp);
    }
    std::stringstream ss(temp);
    ResultLine result;
    ss >> mili::Separator(result, ' ');
    if (result.size() != SIZE_LINE)
    {
        _dG = OBSOLETE_dG;
    }
    else
    {
        const std::string deltaG = result[DELTA_G];
        helper::convertFromString(deltaG, _dG);
    }
}

static const std::string EXECUTABLE_PATH = "runIntaRNA"; ///name executable to find

REGISTER_FACTORIZABLE_CLASS(IHybridize, IntaRNA, std::string, "IntaRNA");

void IntaRNA::prepareData(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq,
                          etilico::Command& command, InputFiles& /*inFiles*/, OutputFile& outFile) const
{
    const std::string seq1 = longerSeq.getString();
    const std::string seq2 = shorterSeq.getString();

    //ver si puedo cargar las dos secuencias en un archivo

    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string tmpOutputFile;
    etilico::createTemporaryFile(tmpOutputFile, path, prefix);
    outFile = tmpOutputFile;
    std::stringstream exec;
    exec << "./IntaRNA ";
    exec << seq1;
    exec << " " << seq2;
    exec << " > " << tmpOutputFile;

    //move to the directory where is the folding
    std::string executablePath;
    etilico::Config::getInstance()->getPath(EXECUTABLE_PATH, executablePath);
    mili::assert_throw<InvalidIntaPath>(chdir(executablePath.c_str()) == 0);
    command = exec.str();   ///./IntaRNA seq1 seq2 > /temp/myTmpFile-******
}

void IntaRNA::processingResult(const OutputFile& outFile, Fe& freeEnergy) const
{
    File outputFile(outFile.c_str());
    mili::assert_throw<NotFoundFileException>(outputFile);
    BodyParser body;
    body.parse(outputFile);
    freeEnergy = body._dG;
}

void IntaRNA::deleteObsoleteFiles(const InputFiles& /*inFiles*/, const OutputFile& outFile) const
{
    mili::assert_throw<UnlinkException>(unlink(outFile.c_str()) == 0);
}

} // namespace fideo