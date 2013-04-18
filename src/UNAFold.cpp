/*
 * @file   UNAFold.cpp
 * @brief  UNAFold is the implementation of IFold interface. It's a specific backend to folding. 
 *
 * @author Franco Riberi 
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend UNAFold implementation.
 * 
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 10, 2010, 4:26 PM
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

#include "fideo/IFold.h"

using std::stringstream;
using namespace mili;

namespace fideo
{
//UNAFold package
class UNAFold : public IFold
{
    std::ifstream file_in;
    static void delete_all_files(const string& nameFile);
    virtual Fe fold(const biopp::NucSequence& seqRNAm, biopp::SecStructure& structureRNAm, bool isCircRNAm) const;

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
                helper::convert_from_string(aux[ColNumberOfBases], number_of_bases);
                helper::convert_from_string(aux[ColDeltaG], delta_G);
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
                helper::convert_from_string(aux[ColNucl], nuc);
                helper::convert_from_string(aux[ColNucleotideNumber], nucNumber);
                helper::convert_from_string(aux[ColPairedWith], pairedNuc);
            }
            return ret;
        }
    };

    static void fillStructure(const BodyLine& bodyLine, biopp::SecStructure& secStructure);

};

REGISTER_FACTORIZABLE_CLASS(IFold, UNAFold, std::string, "UNAFold");

static const string PATH_TMP = "/tmp/";

void UNAFold::delete_all_files(const string& nameFile)
{
    helper::removeFile(nameFile);
    helper::removeFile(nameFile + ".ct");
    helper::removeFile(nameFile + "_1.ct");
    helper::removeFile(nameFile + ".dG");
    helper::removeFile(nameFile + ".h-num");
    helper::removeFile(nameFile + ".rnaml");
    helper::removeFile(nameFile + ".plot");
    helper::removeFile(nameFile + ".run");
    helper::removeFile(nameFile + ".ss-count");
    helper::removeFile(nameFile + ".ann");
    helper::removeFile(nameFile + ".det");
}

void UNAFold::fillStructure(const BodyLine& bodyLine, biopp::SecStructure& secStructure)
{
    if (bodyLine.pairedNuc == 0)
        secStructure.unpair(bodyLine.nucNumber - 1);
    else
        secStructure.pair(bodyLine.nucNumber - 1, bodyLine.pairedNuc - 1);
}

Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, biopp::SecStructure& structureRNAm, bool isCircRNAm) const
{
    structureRNAm.clear();
    FileLine sseq = seqRNAm.getString();

    string temporalFile;
    helper::createTmpFile(temporalFile);

    helper::write(temporalFile, sseq);
    stringstream ss;
    ss << "UNAFold.pl --max=1 ";
    if (isCircRNAm)
        ss << "--circular ";
    ss << temporalFile;

    if (chdir(PATH_TMP.c_str()) != 0)
        throw RNABackendException("Invalid path of temp files.");
    const Command cmd = ss.str();
    helper::runCommand(cmd);

    /* fold.in.ct look like this:
     * amountOfNucleotids dG = 'value'  nameSequence
     * nucleotideNumber  nucleotide previus next pairedWith nucleotideNumber   -    -
     *       1                A        0     2       0             1           0    0
     *       .                .        .     .       .             .           .    .
    */

    std::ifstream file_in((temporalFile + ".ct").c_str());
    if (!file_in)
        throw RNABackendException("output file not found.");
    HeaderLine headerLine;
    headerLine.parse(file_in);
    structureRNAm.set_sequence_size(headerLine.number_of_bases);

    BodyLine bodyLine;
    while (bodyLine.parse(file_in))
    {
        fillStructure(bodyLine, structureRNAm);
    }
    structureRNAm.set_circular(isCircRNAm);
    file_in.close();
    delete_all_files(temporalFile);
    return 0;
}
}
