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

#include <unistd.h>
#include <map>
#include <etilico/etilico.h>
#include "fideo/IFold.h"

namespace fideo
{

using namespace mili;

//UNAFold package
class UNAFold : public IFold
{
private:

    /** @brief Delete all files generated
     *
     * @param nameFile: file name to delete
     * @return void
     */
    void deleteAllFiles();

    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm);
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver);

    /** @brief Destructor of class
     *
     */
    virtual ~UNAFold();

    /** @brief Class that allows parsing the header of a file
    *
    */
    class HeaderParser
    {
    public:

        void parse(File& file);

        biopp::SeqIndex numberOfBases;
        Fe deltaG;
        std::string sequenceName;

    private:

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

    /** @brief Class that allows parsing the body of a file
    *
    */
    class BodyLineParser
    {
    public:

        bool parse(File& file);

        char nuc;                  /// a nucleotid
        biopp::SeqIndex nucNumber; /// starts at 1!. Number of nucleotid in sequence
        biopp::SeqIndex pairedNuc; /// starts at 1!. Nucleotid paired.

    private:

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

    class DetFileParser;

    /** @brief fill structure
    *
    * @param bodyLine: to parse.
    * @param secStructure: structure to fill
    * @return void
    */
    void fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure);

    /** @brief To store the temporal file name generated
     *
     */
    std::string temporalFileName;
};

UNAFold::~UNAFold()
{
    deleteAllFiles();
}

void UNAFold::HeaderParser::parse(File& file)
{
    std::vector<std::string> aux;
    if (file >> aux)
    {
        if (aux.size() != NumberOfColumns)
        {
            throw RNABackendException("Invalid Header.");
        }
        helper::convertFromString(aux[ColNumberOfBases], numberOfBases);
        helper::convertFromString(aux[ColDeltaG], deltaG);
        sequenceName = aux[ColSeqName];
    }
    else
    {
        throw RNABackendException("Failured operation >>.");
    }
}

bool UNAFold::BodyLineParser::parse(File& file)
{
    std::vector<std::string> aux;
    const bool ret = (file >> aux);

    if (ret)
    {
        if (aux.size() != NumberOfColumns)
        {
            throw RNABackendException("Invalid BodyLine.");
        }
        helper::convertFromString(aux[ColNucl], nuc);
        helper::convertFromString(aux[ColNucleotideNumber], nucNumber);
        helper::convertFromString(aux[ColPairedWith], pairedNuc);
    }
    return ret;
}

REGISTER_FACTORIZABLE_CLASS(IFold, UNAFold, std::string, "UNAFold");

static const std::string PATH_TMP = "/tmp/";

void UNAFold::deleteAllFiles()
{
    mili::assert_throw<ExceptionUnlink>(unlink(temporalFileName.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".ct").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + "_1.ct").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".dG").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".h-num").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".rnaml").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".plot").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".run").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".ss-count").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".ann").c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink((temporalFileName + ".det").c_str()) == 0);
}

void UNAFold::fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure)
{
    if (bodyLine.pairedNuc == 0) ///means unpaired
    {
        secStructure.unpair(bodyLine.nucNumber - 1);
    }
    else
    {
        secStructure.pair(bodyLine.nucNumber - 1, bodyLine.pairedNuc - 1);
    }
}

Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm)
{
    structureRNAm.clear();
    FileLine sseq = seqRNAm.getString();

    std::string temporalFile;
    etilico::createTemporaryFilename(temporalFile);
    temporalFileName = temporalFile;

    helper::write(temporalFile, sseq);
    std::stringstream ss;
    ss << "UNAFold.pl --max=1 ";
    if (isCircRNAm)
    {
        ss << "--circular ";
    }
    ss << temporalFile;

    if (chdir(PATH_TMP.c_str()) != 0)
    {
        throw RNABackendException("Invalid path of temp files.");
    }
    const etilico::Command cmd = ss.str();  /// UNAFold.pl --max=1 ("" | --circular) temporalFile
    etilico::runCommand(cmd);

    /* file output look like this:
     * amountOfNucleotids dG = 'value'  nameSequence
     * nucleotideNumber  nucleotide previus next pairedWith nucleotideNumber   -    -
     *       1                A        0     2       0             1           0    0
     *       .                .        .     .       .             .           .    .
    */

    /// temporalFile.ct is the file to parse
    File fileIn((temporalFile + ".ct").c_str());
    if (!fileIn)
    {
        throw RNABackendException("output file not found.");
    }
    HeaderParser headerLine;
    headerLine.parse(fileIn);
    structureRNAm.set_sequence_size(headerLine.numberOfBases);

    BodyLineParser bodyLine;
    while (bodyLine.parse(fileIn))
    {
        fillStructure(bodyLine, structureRNAm);
    }
    structureRNAm.set_circular(isCircRNAm);
    return headerLine.deltaG;
}

//------------------------------------- DetFileParser --------------------------------------

#define DET_FILE_PARSER_H
#include "fideo/DetFileParser.h"
#undef DET_FILE_PARSER_H

/** @brief constant that represents motif name
*
*/
static const std::string HELIX         = "Helix";
static const std::string EXTERNAL_LOOP = "External loop";
static const std::string INTERIOR_LOOP = "Interior loop";
static const std::string HAIRPIN_LOOP  = "Hairpin loop";
static const std::string MULTI_LOOP    = "Multi-loop";
static const std::string BULGE_LOOP    = "Bulge loop";
static const std::string STACK         = "Stack";


void UNAFold::DetFileParser::goToBegin(File& file) const
{
    std::string temporalLine;
    getline(file, temporalLine); //structure data line
    getline(file, temporalLine); //obsolete line
}

void UNAFold::DetFileParser::buildBlock(File& file, Block& block) const
{
    std::string currentLine;
    getline(file, currentLine);
    std::string temporalLine;
    removeConsecutiveWhiteSpaces(currentLine, temporalLine);
    parseMotifLine(temporalLine, block);
    std::string currentMotif = block.motifName;
    while (currentMotif != HELIX && getline(file, currentLine))
    {
        removeConsecutiveWhiteSpaces(currentLine, temporalLine);
        block.lines.push_back(temporalLine);
        if (temporalLine.find_first_of(":") != std::string::npos)
        {
            parseStackLine(temporalLine, currentMotif);
        }
    }
}

void UNAFold::DetFileParser::removeConsecutiveWhiteSpaces(const std::string& src, std::string& dest) const
{
    bool previousWhite = false;
    std::string current = "";
    for (size_t i = 0; i < src.size(); ++i)
    {
        if (src[i] != ' ')
        {
            current += src[i];
            previousWhite = false;
        }
        else if (!previousWhite)
        {
            current += src[i];
            previousWhite = true;
        }
    }
    dest = current;
}

void UNAFold::DetFileParser::parseMotifLine(const std::string& line, Block& block) const
{
    const size_t to = mili::ensure_found(line.find_first_of(":"));
    block.motifName = line.substr(0, to);
    const std::string data = line.substr(to, line.size() - 1);
    block.lines.push_back(data);
}

void UNAFold::DetFileParser::parseStackLine(const std::string& line, std::string& nameToFill) const
{
    const size_t to = mili::ensure_found(line.find_first_of(":"));
    nameToFill = line.substr(0, to);
}

void UNAFold::DetFileParser::parseBlock(const Block& block, IMotifObserver::Motif& motif)
{
    Rule* rule = availableRules[block.motifName];
    rule->calcAttrib(block, motif);
}

void UNAFold::DetFileParser::fillRules() 
{
    availableRules[EXTERNAL_LOOP] = new ExternalRule();
    availableRules[INTERIOR_LOOP] = new InteriorRule();
    availableRules[HAIRPIN_LOOP]  = new HairpinRule();
    availableRules[MULTI_LOOP]    = new MultiRule();
    availableRules[BULGE_LOOP]    = new BulgeRule();
}

UNAFold::DetFileParser::~DetFileParser()
{
    freeRules();
}

void UNAFold::DetFileParser::freeRules()
{
    delete availableRules[EXTERNAL_LOOP];
    delete availableRules[INTERIOR_LOOP];
    delete availableRules[HAIRPIN_LOOP];
    delete availableRules[MULTI_LOOP];
    delete availableRules[BULGE_LOOP];
}

void UNAFold::DetFileParser::parseDet(const std::string& file, IMotifObserver* observer)
{
    File fileToParse;
    fileToParse.open(file.c_str());
    if (!fileToParse)
    {
        throw FileNotExist();
    }
    fillRules();
    goToBegin(fileToParse);
    Block currentBlock;
    IMotifObserver::Motif motif;
    while (!fileToParse.eof())
    {

        buildBlock(fileToParse, currentBlock);
        parseBlock(currentBlock, motif);
        observer->processMotif(motif);
        currentBlock.lines.clear();
    }    
}

//--------------------------------------------------- Rule ----------------------------------------------------

/** @brief constant that represents specific interior loop
*
*/
static const std::string ASYMMETRIC = "Interior Asymmetric";
static const std::string SYMMETRIC = "Interior Symmetric";

void UNAFold::DetFileParser::Rule::getSubstrInPos(const std::string& lineInput, size_t pos, std::string& res)
{
    std::stringstream ss(lineInput);
    std::vector<std::string> result;
    ss >> mili::Separator(result, ' ');
    if (pos > result.size())
    {
        throw IndexOutOfRange();
    }
    res = result[pos];
}

static const size_t INIT_STA_INT_BUL_LOOP = 9;
static const size_t INIT_HAIRPIN_LOOP = 8;

size_t UNAFold::DetFileParser::Rule::getInitPosOfNucleotid(const std::string line, const std::string motifName)
{
    size_t position;
    if (motifName == HAIRPIN_LOOP)
    {
        position = INIT_HAIRPIN_LOOP;
    }
    else if ((motifName == STACK) || (motifName == INTERIOR_LOOP) || (motifName == BULGE_LOOP))
    {
        position = INIT_STA_INT_BUL_LOOP;
    }
    else
    {
        throw InvalidMotif();
    }
    std::string initialStrPos;
    getSubstrInPos(line, position, initialStrPos);
    const size_t firstParenthesis = mili::ensure_found(initialStrPos.find_first_of(")"));
    size_t result;
    helper::convertFromString(initialStrPos.substr(0, firstParenthesis), result);
    return result;
}

static const size_t END_STA_INT_BUL_LOOP = 10;
static const size_t END_HAIRPIN_LOOP = 9;

size_t UNAFold::DetFileParser::Rule::getEndPosOfNucleotid(const std::string line, const std::string motifName)
{
    size_t position;
    if (motifName == HAIRPIN_LOOP)
    {
        position = END_HAIRPIN_LOOP;
    }
    else if ((motifName == STACK) || (motifName == INTERIOR_LOOP) || (motifName == BULGE_LOOP))
    {
        position = END_STA_INT_BUL_LOOP;
    }
    else
    {
        throw InvalidMotif();
    }
    std::string endStrPos;
    getSubstrInPos(line, position, endStrPos);
    size_t result;
    helper::convertFromString(endStrPos.substr(0, endStrPos.size() - 1), result);
    return result;
}

void UNAFold::DetFileParser::Rule::getSecondElement(const Body& list, std::string& element)
{
    Body::const_iterator it = list.begin();
    size_t i = 0;
    std::string currentLine;
    while ((it != list.end()) && (i != 2))
    {
        currentLine = *it;
        ++it;
        ++i;
    }
    element = currentLine;
}

//--------------------------------------------------- Specific Rules ----------------------------------------------------

static const size_t SS_EXTERNAL = 4;

void UNAFold::DetFileParser::ExternalRule::calcAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == EXTERNAL_LOOP);
    const std::string currentLine = block.lines.front();
    std::string temporalValue;
    getSubstrInPos(currentLine, SS_EXTERNAL, temporalValue);
    motif.nameMotif = block.motifName;
    helper::convertFromString(temporalValue, motif.attribute);
    motif.stacks = block.lines.size() - 2; //one by concreteMotif and one by helix line
}

void UNAFold::DetFileParser::InteriorRule::calcAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == INTERIOR_LOOP);
    const std::string currentLine = block.lines.front();
    const size_t initNucleotidOfInteriorLoop = getInitPosOfNucleotid(currentLine, INTERIOR_LOOP);
    const size_t endNucleotidOfInteriorLoop =  getEndPosOfNucleotid(currentLine, INTERIOR_LOOP);
    std::string stackLine;
    getSecondElement(block.lines, stackLine);
    const size_t initNucleotidOfStack = getInitPosOfNucleotid(stackLine, STACK);
    const size_t endNucleotidOfStack = getEndPosOfNucleotid(stackLine, STACK);

    const size_t firstTerm = initNucleotidOfStack - initNucleotidOfInteriorLoop;
    const size_t secondTerm = endNucleotidOfInteriorLoop - endNucleotidOfStack;

    if (firstTerm != secondTerm)
    {
        motif.nameMotif = ASYMMETRIC;
    }
    else
    {
        motif.nameMotif = SYMMETRIC;
    }
    motif.attribute = firstTerm + secondTerm;
    motif.stacks = block.lines.size() - 2;
}

void UNAFold::DetFileParser::HairpinRule::calcAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == HAIRPIN_LOOP);
    const std::string currentLine = block.lines.front();

    const size_t initNucleotid = getInitPosOfNucleotid(currentLine, HAIRPIN_LOOP);
    const size_t endNucleotid =  getEndPosOfNucleotid(currentLine, HAIRPIN_LOOP);

    motif.nameMotif = block.motifName;
    motif.attribute = std::abs(endNucleotid - initNucleotid);
    if (block.lines.size() == 1)
    {
        motif.stacks = 0;
    }
    else
    {
        motif.stacks = block.lines.size() - 2;
    }
}

static const size_t SS_MULTI = 1;

void UNAFold::DetFileParser::MultiRule::calcAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == MULTI_LOOP);
    std::string currentLine;
    getSecondElement(block.lines, currentLine);
    std::string temporalValue;
    getSubstrInPos(currentLine, SS_MULTI, temporalValue);
    helper::convertFromString(temporalValue, motif.attribute);
    motif.nameMotif = block.motifName;
    motif.stacks = block.lines.size() - 3; // the information of multi-loop motif is in 2 lines
}

void UNAFold::DetFileParser::BulgeRule::calcAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == BULGE_LOOP);

    const std::string bulgeLine = block.lines.front();
    const size_t initNucleotidOfBulge = getInitPosOfNucleotid(bulgeLine, BULGE_LOOP);
    const size_t endNucleotidOfBulge =  getEndPosOfNucleotid(bulgeLine, BULGE_LOOP);
    std::string stackLine;
    getSecondElement(block.lines, stackLine);
    const size_t initNucleotidOfStack = getInitPosOfNucleotid(stackLine, STACK);
    const size_t endNucleotidOfStack = getEndPosOfNucleotid(stackLine, STACK);

    const size_t initDif = initNucleotidOfStack - initNucleotidOfBulge;
    const size_t endDif = endNucleotidOfBulge - endNucleotidOfStack;

    if (initDif == EXPECTED_DIFFERENCE)  //The bulge is on the other side
    {
        motif.attribute = endDif;
    }
    else
    {
        motif.attribute = initDif;
    }
    motif.nameMotif = BULGE_LOOP;
    motif.stacks = block.lines.size() - 2;
}

//----------------------------------- Fold with observer --------------------------------------
Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    const Fe freeEnergy = fold(seqRNAm, isCircRNAm, structureRNAm);
    DetFileParser parser;
    const std::string detFile = temporalFileName + ".det";
    parser.parseDet(detFile, motifObserver);
    return freeEnergy;
}
} // namespace fideo