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
    virtual Fe fold(const biopp::NucSequence& seqRNAm, bool isCircRNAm, biopp::SecStructure& structureRNAm) const;

<<<<<<< local
    class HeaderParser
    {       
    public:
        //Archivo parado en la primera linea.
        void parse(std::ifstream& file)
        {
            std::vector<string> aux;
            if (file >> aux)
            {
                if (aux.size() != NumberOfColumns)
                {
                    throw RNABackendException("Invalid Header.");
                }
                helper::convert_from_string(aux[ColNumberOfBases], number_of_bases);
                helper::convert_from_string(aux[ColDeltaG], delta_G);
                sequence_name = aux[ColSeqName];
            }
            else
            {
                throw RNABackendException("Failured operation >>.");
            }
        }

        biopp::SeqIndex number_of_bases;
        Fe delta_G;
        string sequence_name;

    private:
=======
    ///Class that allows parsing the header of a file
    class HeaderLine
    {
>>>>>>> other
        enum Columns
        {
            ColNumberOfBases,
            ColDeltaGStr,
            ColEqualSymbol,
            ColDeltaG,
            ColSeqName,
            NumberOfColumns
        };
    };

    class BodyLineParser
    {       
    public:      

        bool parse(std::ifstream& file)
        {
            std::vector<string> aux;
            const bool ret = (file >> aux);

            if (ret)
            {
                if (aux.size() != NumberOfColumns)
                {
                    throw RNABackendException("Invalid BodyLine.");
                }
                helper::convert_from_string(aux[ColNucl], nuc);
                helper::convert_from_string(aux[ColNucleotideNumber], nucNumber);
                helper::convert_from_string(aux[ColPairedWith], pairedNuc);
            }
            return ret;
        }

<<<<<<< local
        char nuc;
        biopp::SeqIndex nucNumber;  // starts at 1!
        biopp::SeqIndex pairedNuc; // starts at 1! 0 means unpaired.

    private:
=======
    ///Class that allows parsing the body of a file
    class BodyLine
    {
>>>>>>> other
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
    };   

<<<<<<< local
    static void fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure);
=======
    public:
        char nuc;                   /// a nucleotid
        biopp::SeqIndex nucNumber;  /// starts at 1!. Number of nucleotid in sequence
        biopp::SeqIndex pairedNuc;  /// starts at 1!. Nucleotid paired. 

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

    /** @brief fill structure
     * 
     * @param bodyLine: to parse. 
     * @param secStructure: structure to fill
     * @return void
     */
    static void fillStructure(const BodyLine& bodyLine, biopp::SecStructure& secStructure);

>>>>>>> other
};

REGISTER_FACTORIZABLE_CLASS(IFold, UNAFold, std::string, "UNAFold");

static const string PATH_TMP = "/tmp/";

void UNAFold::delete_all_files(const string& nameFile)
{    
    helper::removeFile(nameFile);

    ///remove others files generated by UNAFold
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

void UNAFold::fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure)
{
<<<<<<< local
    if (bodyLine.pairedNuc == 0)
    {
=======
    if (bodyLine.pairedNuc == 0) ///means unpaired
>>>>>>> other
        secStructure.unpair(bodyLine.nucNumber - 1);
    }
    else
    {
        secStructure.pair(bodyLine.nucNumber - 1, bodyLine.pairedNuc - 1);
    }
}

Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, bool isCircRNAm, biopp::SecStructure& structureRNAm) const
{
    structureRNAm.clear();
    FileLine sseq = seqRNAm.getString();

    string temporalFile;
    helper::createTmpFile(temporalFile);

    helper::write(temporalFile, sseq);
    stringstream ss;
    ss << "UNAFold.pl --max=1 ";
    if (isCircRNAm)
    {
        ss << "--circular ";
    }
    ss << temporalFile;

    if (chdir(PATH_TMP.c_str()) != 0)
    {
        throw RNABackendException("Invalid path of temp files.");
<<<<<<< local
    }
    const Command cmd = ss.str();
=======
    const Command cmd = ss.str();  /// UNAFold.pl --max=1 ("" | --circular) temporalFile
>>>>>>> other
    helper::runCommand(cmd);

    /* file output look like this:
     * amountOfNucleotids dG = 'value'  nameSequence
     * nucleotideNumber  nucleotide previus next pairedWith nucleotideNumber   -    -
     *       1                A        0     2       0             1           0    0
     *       .                .        .     .       .             .           .    .
    */

    /// temporalFile.ct is the file to parse
    std::ifstream file_in((temporalFile + ".ct").c_str());
    if (!file_in)
<<<<<<< local
    {
        throw RNABackendException("output file not found.");
    }
    HeaderParser headerLine;
=======
        throw NotFoundFileException();
    HeaderLine headerLine;
>>>>>>> other
    headerLine.parse(file_in);
    structureRNAm.set_sequence_size(headerLine.number_of_bases);

    BodyLineParser bodyLine;
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
