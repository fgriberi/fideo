/*
 * @file   RNAFoldInverse.cpp
 * @brief  RNAFoldInverse is the implementation of IFoldInverse interface. It's a specific backend to folding.
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 *
 * Contents:  Source file for fideo providing backend RNAFoldInverse implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 10, 2010, 2012 4:26 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, Franco Riberi, FuDePAN
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

#include <string>
#include <biopp/biopp.h>
#include <etilico/etilico.h>
#include "fideo/FideoStructureParser.h"
#include "fideo/FideoHelper.h"
#include "fideo/RNAStartInverse.h"

namespace fideo
{

class RNAinverse : public RNAStartInverse
{
    static const FilePath IN;
    static const FilePath OUT;
    static const FileLineNo LINE_NO;

    size_t read_hamming_distance(FileLine&, size_t, Distance&) const;
    size_t read_structure_distance(FileLine&, size_t, Similitude&) const;

    virtual void execute(std::string&, Distance&, Similitude&);
    virtual void query_start(IStartProvider*);
protected:
    virtual void getProgram(std::string& name) const;
public:
    RNAinverse(const InverseFoldParams& params);
};

const FilePath RNAinverse::IN = "inverse.in";
const FilePath RNAinverse::OUT = "inverse.out";
const FileLineNo RNAinverse::LINE_NO = 0;

REGISTER_FACTORIZABLE_CLASS_WITH_ARG(IFoldInverse, RNAinverse, std::string, "RNAinverse", const InverseFoldParams&);

RNAinverse::RNAinverse(const InverseFoldParams& params) :
    RNAStartInverse(params)
{}

void RNAinverse::query_start(IStartProvider* provider)
{
    provider->get_partial_start(this);
    if (start.size() != structure.size())
        throw RNABackendException("Partial start and target structure must have the same length");
}

void RNAinverse::getProgram(std::string& executablePath) const
{
    etilico::Config::getInstance()->getPath("RNAinverse", executablePath);
}

void RNAinverse::execute(std::string& seq, Distance& hd, Similitude& sd)
{
    FileLinesCt lines;
    std::string structure_str;
    ViennaParser::toString(structure, structure_str);
    mili::insert_into(lines, structure_str);
    mili::insert_into(lines, start);
    fideo::helper::write(IN, lines);

    std::stringstream ss;
    const int repeat = (max_structure_distance == 0) ? -1 : 1;

    std::string executablePath;
    getProgram(executablePath);

    ss << executablePath << " -R " << repeat << " -a ATGC < " << IN << " > " << OUT;
    const etilico::Command CMD = ss.str();

    //CMD looks like "RNAinverse -R -1 -a ATGC < inverse.in > inverse.out"
    etilico::runCommand(CMD);

    //BUG: inverse.out looks like "TGCCTGTACTCATTAATGGAACTTCcuaccagucgcgau    6"
    FileLine aux;
    fideo::helper::readLine(OUT, LINE_NO, aux);

    // If the the search was unsuccessful, a structure distance to the target is appended.

    /* aux looks like this
     * accagggATCgcaggtaccccgcaGGcgcagAacccta   5 2   d= 2
     *
     * where uppercases were set by RNAinverse (dots in our start).
     * 5 it's the hamming distance from the random generated start and the
     * sequence found. And 2 it's the structure distance to the target structure.
     * If max_structure_distance=0 then, this will be also equal to 0.
     */
    fideo::helper::readValue(aux, 0, start.size(), seq);
    for (size_t i = 0; i < start.size(); ++i)
        seq[i] = tolower(seq[i]);

    //hamming distance from the start used
    const size_t sd_offset = read_hamming_distance(aux, start.size(), hd);
    //structure distance from the sequence found to the target structure.
    read_structure_distance(aux, sd_offset, sd);
}

size_t RNAinverse::read_hamming_distance(FileLine& line, size_t offset, Distance& hd) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_not_of(" ", offset));
        const size_t to = mili::ensure_found(line.find_first_of(" ", from));
        fideo::helper::readValue(line, from, to - from, hd);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read hamming distance");
    }
    catch (const fideo::NotReadValueUsingOffsetException& e)
    {
        throw RNABackendException("Could not read hamming distance");
    }
}

size_t RNAinverse::read_structure_distance(FileLine& line, size_t offset, Similitude& sd) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_not_of(" ", offset));
        const size_t to = mili::ensure_found(line.find_first_of(" ", from), line.size());
        fideo::helper::readValue(line, from, to - from, sd);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read structure distance");
    }
    catch (const fideo::NotReadValueUsingOffsetException& e)
    {
        throw RNABackendException("Could not read structure distance");
    }
}

class RNAinverseTest : public RNAinverse
{
protected:
    virtual void getProgram(std::string& executablePath) const
    {
        etilico::Config::getInstance()->getPath("RNAinverseMock", executablePath);
    }
public:
    RNAinverseTest(const InverseFoldParams& params)
        : RNAinverse(params)
    {}
};

REGISTER_FACTORIZABLE_CLASS_WITH_ARG(IFoldInverse, RNAinverseTest, std::string, "RNAinverseTest", const InverseFoldParams&);


} //end namespace fideo
