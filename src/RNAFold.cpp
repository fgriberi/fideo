/*
 * File:   RNAFold.cpp
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <sstream>
#include <stack>
#include <mili/mili.h>
#include "fideo/IFold.h"
#include "fideo/RNABackendProxy.h"
#include "fideo/RNABackendsConfig.h"
#include "fideo/rna_backends_types.h"

using std::stack;
using std::stringstream;

static const FilePath OUT;

//Vienna package
class RNAFold : public IFold
{
    static const FileLineNo LINE_NO;
    static const char OPEN_PAIR = '(';
    static const char CLOSE_PAIR = ')';
    static const char UNPAIR = '.';
    size_t read_free_energy(FileLine& file, size_t offset, Fe& energy) const;
    static void parse_structure(std::string& str, biopp::SecStructure& secStructure);
    virtual Fe fold(const biopp::NucSequence& seqRNAm, biopp::SecStructure& structureRNAm, bool isCircRNAm) const;
};

const FileLineNo RNAFold::LINE_NO = 1;

REGISTER_FACTORIZABLE_CLASS(IFold, RNAFold, std::string, "RNAFold");

Fe RNAFold::fold(const biopp::NucSequence& seqRNAm, biopp::SecStructure& structureRNAm, bool isCircRNAm) const
{
    structureRNAm.clear();
    structureRNAm.set_circular(isCircRNAm);
    FileLine sseq = seqRNAm.getString();
    write(get_input_file_name(), sseq);
    stringstream ss;
    ss << "RNAfold" << " --noPS ";
    if (isCircRNAm)
        ss << "-circ ";
    ss << "< " << get_input_file_name() << " > " << get_output_file_name();
    const Command CMD = ss.str();
    runCommand(CMD);

    /* fold.out look like this:
     * CGCAGGGAUCGCAGGUACCCCGCAGGCGCAGAUACCCUA
     * ...(((((((....(..((.....))..).))).)))). (-10.80)
    */
    FileLine aux;
    read_line(OUT, LINE_NO, aux);

    string str;
    read_value(aux, 0, seqRNAm.length(), str);
    parse_structure(str, structureRNAm);

    Fe energy;
    read_free_energy(aux, seqRNAm.length(), energy);
    return energy;
}

size_t RNAFold::read_free_energy(FileLine& line, size_t offset, Fe& energy) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_of("(", offset)) + 1;
        const size_t to = mili::ensure_found(line.find_first_of(")", from)) - 1;
        read_value(line, from, to - from, energy);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read free energy");
    }
}

void RNAFold::parse_structure(std::string& str, biopp::SecStructure& secStructure)
{
    secStructure.set_sequence_size(str.length());
    stack<biopp::SeqIndex> s;
    for (size_t i = 0; i < str.length(); ++i)
    {
        biopp::SeqIndex open;
        switch (str[i])
        {
            case UNPAIR:
                secStructure.unpair(i);
                break;
            case OPEN_PAIR:
                s.push(i);
                break;
            case CLOSE_PAIR:
                if (!s.empty())
                {
                    open = s.top();
                    secStructure.pair(open, i);
                    s.pop();
                }
                else
                    throw(InvalidStructureException(" Unexpected closing pair"));
                break;
            default:
                throw(InvalidStructureException(" Unexpected symbol: " + secStructure.paired_with(i)));
                break;
        }
    }
    if (!s.empty())
        throw(InvalidStructureException(" Pairs pending to close"));
}
