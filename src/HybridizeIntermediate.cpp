/*
 * @file     HybridizeIntermediate.cpp
 * @brief    This is the implementation of IHybridizeIntermediate interface.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing class HybridizeIntermediate implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date July 2013
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

#include "fideo/IHybridizeIntermediate.h"

namespace fideo
{

Fe IHybridizeIntermediate::hybridize(const biopp::NucSequence& longerSeq, const bool longerCirc,
                                     const biopp::NucSequence& shorterSeq) const
{
    mili::assert_throw<UnsupportedException>(!longerCirc);
    InputFiles inFiles;
    OutputFile outFile;
    etilico::Command cmd;
    prepareData(longerSeq, shorterSeq, cmd, inFiles, outFile);
    etilico::runCommand(cmd);

    Fe freeEnergy;
    processingResult(outFile, freeEnergy);
    deleteObsoleteFiles(inFiles, outFile);
    return freeEnergy;
}
} //namespace fideo
