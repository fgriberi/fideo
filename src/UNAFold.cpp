/*
 * @file     UNAFold.cpp
 * @brief    UNAFold is an implementation of IFold interface. It's a specific backend to folding.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Source file for fideo providing backend UNAFold implementation.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
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
 * along with fideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define UNA_FOLD_H
#include "fideo/UNAFold.h"
#undef UNA_FOLD_H

namespace fideo
{

using mili::operator>>;

const std::string UNAFold::_det = ".det";

UNAFold::~UNAFold()
{
    File file(_temporalFileName.c_str());
    if (file)
    {
        deleteObsoleteFiles(_temporalFileName);
    }
    File detFile((_temporalFileName + _det).c_str());
    if (detFile)
    {
        mili::assert_throw<UnlinkException>(unlink((_temporalFileName + _det).c_str()) == 0);
    }
}

void UNAFold::HeaderParser::parse(File& file)
{
    ResultLine aux;
    if (file >> aux)
    {
        mili::assert_throw<InvalidaHeader>(aux.size() == NumberOfColumns);
        helper::convertFromString(aux[ColNumberOfBases], _numberOfBases);
        helper::convertFromString(aux[ColDeltaG], _deltaG);
        _sequenceName = aux[ColSeqName];
    }
    else
    {
        throw FailOperation();
    }
}

bool UNAFold::BodyLineParser::parse(File& file)
{
    ResultLine aux;
    const bool ret = (file >> aux);

    if (ret)
    {
        mili::assert_throw<InvalidBodyLine>(aux.size() == NumberOfColumns);
        helper::convertFromString(aux[ColNucl], _nuc);
        helper::convertFromString(aux[ColNucleotideNumber], _nucNumber);
        helper::convertFromString(aux[ColPairedWith], _pairedNuc);
    }
    return ret;
}

REGISTER_FACTORIZABLE_CLASS(IFold, UNAFold, std::string, "UNAFold");

static const std::string PATH_TMP = "/tmp/";

void UNAFold::fillStructure(const BodyLineParser& bodyLine, biopp::SecStructure& secStructure)
{
    if (bodyLine._pairedNuc == 0) ///means unpaired
    {
        secStructure.unpair(bodyLine._nucNumber - 1);
    }
    else
    {
        secStructure.pair(bodyLine._nucNumber - 1, bodyLine._pairedNuc - 1);
    }
}

void UNAFold::deleteObsoleteFiles(const InputFile& inFile)
{
    mili::assert_throw<UnlinkException>(unlink(inFile.c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + "_1.ct").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".dG").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".h-num").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".rnaml").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".plot").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".run").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".ss-count").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((inFile + ".ann").c_str()) == 0);
}

static const size_t NAME_FILE = 0;
void UNAFold::renameNecessaryFiles(const std::string& fileToRename, const std::string& newNameFile)
{
    const etilico::Command renameCtFile = "mv " + fileToRename + " " + newNameFile;
    etilico::runCommand(renameCtFile);

    std::stringstream ss(fileToRename);
    ResultLine result;
    ss >> mili::Separator(result, '.');
    mili::assert_throw<InvalidName>(result.size() == 2);

    const etilico::Command renameDetFile = "mv " + result[NAME_FILE] + _det + " " + newNameFile + _det;
    etilico::runCommand(renameDetFile);
}

void UNAFold::prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, InputFile& inputFile, OutputFile& outputFile)
{
    FileLine sseq = sequence.getString();
    std::string prefix = "fideo-XXXXXX";
    std::string temporalFile;
    etilico::createTemporaryFile(temporalFile, PATH_TMP, prefix);
    _temporalFileName = temporalFile;
    inputFile = temporalFile;
    outputFile = temporalFile + ".ct";
    helper::write(temporalFile, sseq);
    std::stringstream ss;
    ss << "UNAFold.pl --max=1 ";
    if (isCirc)
    {
        ss << "--circular ";
    }
    ss << temporalFile;

    if (chdir(PATH_TMP.c_str()) != 0)
    {
        throw RNABackendException("Invalid path of temp files.");
    }
    command = ss.str(); /// UNAFold.pl --max=1 ("" | --circular) temporalFile
}

void UNAFold::processingResult(biopp::SecStructure& structureRNAm, const InputFile& inputFile, Fe& freeEnergy)
{
    File fileIn(inputFile.c_str());
    mili::assert_throw<NotFoundFileException>(fileIn);
    HeaderParser headerLine;
    headerLine.parse(fileIn);
    structureRNAm.set_sequence_size(headerLine._numberOfBases);

    BodyLineParser bodyLine;
    while (bodyLine.parse(fileIn))
    {
        fillStructure(bodyLine, structureRNAm);
    }
    freeEnergy = headerLine._deltaG;
}

void UNAFold::deleteAllFilesAfterProcessing(const InputFile& inFile, const OutputFile& outFile)
{
    deleteObsoleteFiles(inFile);
    mili::assert_throw<UnlinkException>(unlink(outFile.c_str()) == 0); //.ct file
}

//------------------------------------- DetFileParser --------------------------------------

void UNAFold::DetFileParser::goToBegin(File& file) const
{
    std::string temporalLine;
    getline(file, temporalLine); //structure data line
    getline(file, temporalLine); //obsolete line
}

void UNAFold::DetFileParser::removeConsecutiveWhiteSpaces(const std::string& src, std::string& dest) const
{
    dest = "";
    bool previousWhite = false;
    for (std::string::const_iterator it = src.begin(); it != src.end(); ++it)
    {
        if (*it != ' ')
        {
            dest += *it;
            previousWhite = false;
        }
        else if (!previousWhite)
        {
            dest += *it;
            previousWhite = true;
        }
    }
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

bool UNAFold::DetFileParser::containMotif(const std::string& line) const
{
    bool ret = false;
    if (line.find_first_of(":") == std::string::npos)
    {
        ret = false;
    }
    else
    {
        const size_t to = mili::ensure_found(line.find_first_of(":"));
        const std::string nameMotif = line.substr(0, to);
        if (nameMotif == INTERIOR_LOOP || nameMotif == HAIRPIN_LOOP || nameMotif == MULTI_LOOP || nameMotif == BULGE_LOOP)
        {
            ret = true;
        }
    }
    return ret;
}

void UNAFold::DetFileParser::buildBlock(File& file, Block& block) const
{
    block.lines.clear();
    std::string currentLine;
    getline(file, currentLine);
    std::string temporalLine;
    removeConsecutiveWhiteSpaces(currentLine, temporalLine);
    parseMotifLine(temporalLine, block);
    std::string currentMotif = block.motifName;
    while (currentMotif != HELIX && getline(file, currentLine))
    {
        removeConsecutiveWhiteSpaces(currentLine, temporalLine);
        if (containMotif(temporalLine))
        {
            block.lines.clear();
            parseMotifLine(temporalLine, block);
        }
        else
        {
            block.lines.push_back(temporalLine);
            if (temporalLine.find_first_of(":") != std::string::npos)
            {
                parseStackLine(temporalLine, currentMotif);
            }
        }
    }
}

void UNAFold::DetFileParser::parseBlock(const Block& block, IMotifObserver::Motif& motif)
{
    Rule* rule = _availableRules[block.motifName];
    rule->calculateAttrib(block, motif);
}

void UNAFold::DetFileParser::fillRules()
{
    _availableRules[EXTERNAL_LOOP] = new ExternalRule();
    _availableRules[INTERIOR_LOOP] = new InteriorRule();
    _availableRules[HAIRPIN_LOOP]  = new HairpinRule();
    _availableRules[MULTI_LOOP]    = new MultiRule();
    _availableRules[BULGE_LOOP]    = new BulgeRule();
}

UNAFold::DetFileParser::~DetFileParser()
{
    mili::delete_container(_availableRules);
}

void UNAFold::DetFileParser::parseDet(const std::string& file, IMotifObserver* observer)
{
    File fileToParse;
    fileToParse.open(file.c_str());
    mili::assert_throw<FileNotExist>(fileToParse);
    fillRules();
    goToBegin(fileToParse);
    Block currentBlock;
    IMotifObserver::Motif motif;
    while (!fileToParse.eof())
    {
        buildBlock(fileToParse, currentBlock);
        parseBlock(currentBlock, motif);
        observer->processMotif(motif);
    }
    observer->finalize();
}

//--------------------------------------------------- Rule ----------------------------------------------------

void UNAFold::DetFileParser::Rule::getSubstrInPos(const std::string& lineInput, const size_t pos, std::string& res)
{
    std::stringstream ss(lineInput);
    ResultLine result;
    ss >> mili::Separator(result, ' ');
    mili::assert_throw<IndexOutOfRange>(pos <= result.size());
    res = result[pos];
}

static const size_t INIT_STA_INT_BUL_LOOP = 9;
static const size_t INIT_HAIRPIN_LOOP = 8;

size_t UNAFold::DetFileParser::Rule::getInitPosOfNucleotid(const std::string& line, const std::string& motifName)
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

size_t UNAFold::DetFileParser::Rule::getEndPosOfNucleotid(const std::string& line, const std::string& motifName)
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

void UNAFold::DetFileParser::ExternalRule::calculateAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == EXTERNAL_LOOP);
    const std::string currentLine = block.lines.front();
    std::string temporalValue;
    getSubstrInPos(currentLine, SS_EXTERNAL, temporalValue);
    motif.nameMotif = block.motifName;
    helper::convertFromString(temporalValue, motif.attribute);
    motif.amountStacks = block.lines.size() - 2; //one by concreteMotif and one by helix line
}

void UNAFold::DetFileParser::InteriorRule::calculateAttrib(const Block& block, IMotifObserver::Motif& motif) const
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
        if (firstTerm >= secondTerm)
        {
            motif.attribute = firstTerm;
        }
        else
        {
            motif.attribute = secondTerm;
        }
    }
    else
    {
        motif.nameMotif = SYMMETRIC;
        motif.attribute = firstTerm;
    }
    motif.amountStacks = block.lines.size() - 2;
}

void UNAFold::DetFileParser::HairpinRule::calculateAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == HAIRPIN_LOOP);
    const std::string currentLine = block.lines.front();

    const size_t initNucleotid = getInitPosOfNucleotid(currentLine, HAIRPIN_LOOP);
    const size_t endNucleotid =  getEndPosOfNucleotid(currentLine, HAIRPIN_LOOP);

    motif.nameMotif = block.motifName;
    motif.attribute = std::abs(endNucleotid - initNucleotid);
    if (block.lines.size() == 1)
    {
        motif.amountStacks = 0;
    }
    else
    {
        motif.amountStacks = block.lines.size() - 2;
    }
}

static const size_t SS_MULTI = 1;

void UNAFold::DetFileParser::MultiRule::calculateAttrib(const Block& block, IMotifObserver::Motif& motif) const
{
    assert(block.motifName == MULTI_LOOP);
    std::string currentLine;
    getSecondElement(block.lines, currentLine);
    std::string temporalValue;
    getSubstrInPos(currentLine, SS_MULTI, temporalValue);
    helper::convertFromString(temporalValue, motif.attribute);
    motif.nameMotif = block.motifName;
    motif.amountStacks = block.lines.size() - 3; // the information of multi-loop motif is in 2 lines
}

void UNAFold::DetFileParser::BulgeRule::calculateAttrib(const Block& block, IMotifObserver::Motif& motif) const
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
    motif.amountStacks = block.lines.size() - 2;
}

//----------------------------------- Fold with observer --------------------------------------

void UNAFold::commonParse(const FilePath& file, IMotifObserver* observer)
{
    DetFileParser parser;
    parser.parseDet(file, observer);
}

Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, const Temperature temp, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    const Fe freeEnergy = IFoldIntermediate::fold(seqRNAm, isCircRNAm, temp, structureRNAm);
    commonParse(_temporalFileName + _det, motifObserver);
    mili::assert_throw<UnlinkException>(unlink((_temporalFileName + _det).c_str()) == 0);
    return freeEnergy;
}

void UNAFold::foldTo(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, const Temperature temp, biopp::SecStructure& structureRNAm, const FilePath& outputFile, IMotifObserver* motifObserver)
{
    IFoldIntermediate::foldTo(seqRNAm, isCircRNAm, temp, structureRNAm, outputFile);
    commonParse(outputFile + _det, motifObserver);
}

Fe UNAFold::foldFrom(const FilePath& inputFile, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    const Fe freeEnergy = IFoldIntermediate::foldFrom(inputFile, structureRNAm);
    commonParse(inputFile + _det, motifObserver);
    return freeEnergy;
}

} // namespace fideo
