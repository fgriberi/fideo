/*
 * File:   UNAFold.cpp
 * Author: Franco Riberi <fgriberi at gmail.com>
 *
 * Created on November 10, 2010, 4:26 PM
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vac-o.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <sstream>
#include <vector>
#include <mili/mili.h>
#include "errno.h"
#include "fideo/IFold.h"
#include "fideo/RNABackendProxy.h"
#include "fideo/RNABackendsConfig.h"
#include "fideo/rna_backends_types.h"

using std::stringstream;
using namespace mili;

//UNAFold package
class UNAFold : public IFold
{
    std::ifstream file_in;
    static void delete_all_files();
    virtual Fe fold(const biopp::NucSequence& sequence, biopp::SecStructure& structure, bool circ) const;

    class HeaderLine
    {
        enum Columns
        {
            ColNumberOfBases,
            ColDeltaGStr,
            ColEqualSymbol,
            ColDeltaG,
            ColSeqName,
            NumberOfColumns
        };
    public:
        biopp::SeqIndex number_of_bases;
        Fe delta_G;
        string sequence_name;

        //Archivo parado en la primera linea.
        void parse(std::ifstream& file)
        {
            std::vector<string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                    throw RNABackendException("Invalid Header.");
                convert_from_string(aux[ColNumberOfBases], number_of_bases);
                convert_from_string(aux[ColDeltaG], delta_G);
                sequence_name = aux[ColSeqName];
            }
            else
                throw RNABackendException("Failured operation >>.");
        }
    };

    class BodyLine
    {
        enum Columns
        {
            ColNucleotideNumber,
            ColNucl,
            ColPrevious,
            ColNext,
            ColPairedWith,
            ColNumberOfBases2,
            Col7,  // not used
            Col8,  // not used
            NumberOfColumns
        };

    public:
        char nuc;
        biopp::SeqIndex nucNumber;  // starts at 1!
        biopp::SeqIndex pairedNuc; // starts at 1! 0 means unpaired.

        bool parse(std::ifstream& file)
        {
            std::vector<string> aux;
            const bool ret = (file >> aux);

            if (ret)
            {
                if (aux.size() != NumberOfColumns)
                    throw RNABackendException("Invalid BodyLine.");
                convert_from_string(aux[ColNucl], nuc);
                convert_from_string(aux[ColNucleotideNumber], nucNumber);
                convert_from_string(aux[ColPairedWith], pairedNuc);
            }
            return ret;
        }
    };

    static void fillStructure(const BodyLine& bodyLine, biopp::SecStructure& secStructure);

};

REGISTER_FACTORIZABLE_CLASS(IFold, UNAFold, std::string, "UNAFold");

void UNAFold::delete_all_files()
{
    remove_file((get_input_file_name() + ".ct").c_str());
    remove_file((get_input_file_name() + ".dG").c_str());
    remove_file((get_input_file_name() + ".h-num").c_str());
    remove_file((get_input_file_name() + ".log").c_str());
    remove_file((get_input_file_name() + ".plot").c_str());
    remove_file((get_input_file_name() + ".run").c_str());
    remove_file((get_input_file_name() + ".ss-count").c_str());
    remove_file((get_input_file_name() + ".ann").c_str());
    remove_file((get_input_file_name() + ".det").c_str());
}


void UNAFold::fillStructure(const BodyLine& bodyLine, biopp::SecStructure& secStructure)
{
    if (bodyLine.pairedNuc == 0)
        secStructure.unpair(bodyLine.nucNumber - 1);
    else
        secStructure.pair(bodyLine.nucNumber - 1, bodyLine.pairedNuc - 1);
}

Fe UNAFold::fold(const biopp::NucSequence& sequence, biopp::SecStructure& structure, bool circ) const
{
    structure.clear();
    FileLine sseq;
    for (size_t i = 0; i < sequence.length(); ++i)
        sseq += sequence[i].as_char();
    write(get_input_file_name(), sseq);
    stringstream ss;
    ss << "UNAFold.pl --max=1 ";
    if (circ)
        ss << "--circular ";
    ss << get_input_file_name();

    const Command CMD = ss.str();
    runCommand(CMD);

    /* fold.in.ct look like this:
     * amountOfNucleotids dG = 'value'  nameSequence
     * nucleotideNumber  nucleotide previus next pairedWith nucleotideNumber   -    -
     *       1                A        0     2       0             1           0    0
     *       .                .        .     .       .             .           .    .
    */

    std::ifstream file_in((get_input_file_name() + ".ct").c_str());
    if (!file_in)
        throw RNABackendException("output file not found.");
    HeaderLine headerLine;
    headerLine.parse(file_in);
    structure.set_sequence_size(headerLine.number_of_bases);

    BodyLine bodyLine;
    while (bodyLine.parse(file_in))
    {
        fillStructure(bodyLine, structure);
    }
    structure.set_circular(circ);
    file_in.close();
    delete_all_files();
    return 0;
}
