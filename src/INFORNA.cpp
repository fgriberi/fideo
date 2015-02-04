/*
 * File:   INFORNA.cpp
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on November 10, 2010, 4:26 PM
 *
 * Copyright (C) 2010  Santiago Videla, FuDePAN
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

#include <sstream>
#include <string>
#include <mili/mili.h>
#include <etilico/etilico.h>

#include "fideo/RNAStartInverse.h"
#include "fideo/FideoHelper.h"
#include "fideo/FideoStructureParser.h"

namespace fideo
{

class INFORNA : public RNAStartInverse
{
public:
    /**
     * @brief Constructor of class
     */
    INFORNA(const InverseFoldParams& params);

private:
    static const FilePath OUT;
    static const FileLineNo LINE_NO;

    size_t read_sequence(FileLine&, size_t, std::string&) const;
    size_t read_hamming_distance(FileLine&, size_t, Distance&) const;
    size_t read_structure_distance(FileLine&, size_t, Similitude&) const;

    virtual void execute(std::string&, Distance&, Similitude&, const Temperature);
    virtual void query_start(IStartProvider*);
};

REGISTER_FACTORIZABLE_CLASS_WITH_ARG(IFoldInverse, INFORNA, std::string, "INFORNA", const InverseFoldParams&);

const FilePath INFORNA::OUT = "inverse.out"; //TODO: rename
const FileLineNo INFORNA::LINE_NO = 13; //TODO: rename

INFORNA::INFORNA(const InverseFoldParams& params) :
    RNAStartInverse(params)
{}

void INFORNA::query_start(IStartProvider* provider)
{
    provider->get_partial_start(this);
    if (start.size() != structure.size())
        throw RNABackendException("Partial start and target structure must have the same length");
}

void INFORNA::execute(std::string& seq, Distance& hd, Similitude& sd, const Temperature /*temp*/)
{
    std::stringstream ss;
    const int repeat = max_structure_distance == 0 ? -1 : 1;
    std::string structure_str;
    ViennaParser::toString(structure, structure_str);    

    ss << "INFO-RNA-2.1.2 '" << structure_str << "'"
       << " -c '" << start << "'"
       << " -R " << repeat << " > " << OUT;

    const etilico::Command CMD  = ss.str();

    etilico::runCommand(CMD);

    FileLine aux;//TODO: rename
    fideo::helper::readLine(OUT, LINE_NO, aux);
    /* inverse.out looks like this:
     *
     * =========================
     * Initializing Step:
     * =========================
     * Wanted Structure    : (..((.....))..)
     * Sequence Constraints: NNNNNNNNNUGCCGC
     * Allowed Mismatches  : 000000000000000
     * Designed Sequence   : GUAGCUUUAUGCCGC
     * designed randomly   : 0
     *
     * =========================
     * Local Search Results:
     * =========================
     * MFE:    GUAGCUUUAUGCCGC    0  (-1.70)   d= 1
     * number of mismatches: 0
     *
     */
    //sequence found
    const size_t hd_offset = read_sequence(aux, 4, seq);
    //hamming distance from the start used
    const size_t sd_offset = read_hamming_distance(aux, hd_offset, hd);
    //structure distance from the sequence found to the target structure.
    read_structure_distance(aux, sd_offset, sd);
}

size_t INFORNA::read_sequence(FileLine& line, size_t offset, string& seq) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_not_of(" ", offset));
        const size_t to = mili::ensure_found(line.find_first_of(" ", from));
        fideo::helper::readValue(line, from, to - from, seq);

        for (size_t i = 0; i < seq.size(); ++i)
            seq[i] = tolower(seq[i]);

        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read sequence");
    }
}

size_t INFORNA::read_hamming_distance(FileLine& line, size_t offset, Distance& hd) const
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
}

size_t INFORNA::read_structure_distance(FileLine& line, size_t offset, Similitude& sd) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_of("d", offset)) + 3;
        const size_t to = line.size();
        fideo::helper::readValue(line, from, to - from, sd);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        sd = 0;
        return line.size();
    }
}

class INFORNATest : public INFORNA
{
public:
    /**
     * @brief Constructor of class
     */
    INFORNATest(const InverseFoldParams& params)
        : INFORNA(params)
    {}

protected:

    virtual void getProgram(std::string& executablePath) const
    {
        etilico::Config::getInstance()->getPath("INFORNAMock", executablePath);
    }
};

REGISTER_FACTORIZABLE_CLASS_WITH_ARG(IFoldInverse, INFORNATest, std::string, "INFORNATest", const InverseFoldParams&);

} //end namespace fideo
