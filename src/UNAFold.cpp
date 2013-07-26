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

UNAFold::~UNAFold()
{
    deleteAllFiles();
}

void UNAFold::HeaderParser::parse(File& file)
{
    ResultLine aux;
    if (file >> aux)
    {
        if (aux.size() != NumberOfColumns)
        {
            throw InvalidaHeader();
        }
        helper::convertFromString(aux[ColNumberOfBases], numberOfBases);
        helper::convertFromString(aux[ColDeltaG], deltaG);
        sequenceName = aux[ColSeqName];
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
        if (aux.size() != NumberOfColumns)
        {
            throw InvalidBodyLine();
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
    mili::assert_throw<UnlinkException>(unlink(temporalFileName.c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".ct").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + "_1.ct").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".dG").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".h-num").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".rnaml").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".plot").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".run").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".ss-count").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".ann").c_str()) == 0);
    mili::assert_throw<UnlinkException>(unlink((temporalFileName + ".det").c_str()) == 0);
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

void UNAFold::prepareData(const biopp::NucSequence& sequence, const bool isCirc, etilico::Command& command, IntermediateFiles& outputFiles)
{
    FileLine sseq = sequence.getString();
    const std::string path = "/tmp/";
    std::string prefix = "fideo-XXXXXX";
    std::string temporalFile;
    etilico::createTemporaryFile(temporalFile, path, prefix);
    temporalFileName = temporalFile;
    outputFiles.push_back(temporalFile);

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

void UNAFold::processingResult(const bool isCirc, biopp::SecStructure& structureRNAm, size_t sizeSequence, const IntermediateFiles& inputFiles, Fe& freeEnergy)
{
    File fileIn((inputFiles[INPUT_FILE] + ".ct").c_str());
    if (!fileIn)
    {
        throw NotFoundFileException();
    }
    HeaderParser headerLine;
    headerLine.parse(fileIn);
    structureRNAm.set_sequence_size(headerLine.numberOfBases);

    BodyLineParser bodyLine;
    while (bodyLine.parse(fileIn))
    {
        fillStructure(bodyLine, structureRNAm);
    }
    structureRNAm.set_circular(isCirc);
    freeEnergy = headerLine.deltaG;
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
    Rule* rule = availableRules[block.motifName];
    rule->calculateAttrib(block, motif);
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
    mili::delete_container(availableRules);
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
    observer->start();
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
    if (pos > result.size())
    {
        throw IndexOutOfRange();
    }
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
Fe UNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    const Fe freeEnergy = IFoldIntermediate::fold(seqRNAm, isCircRNAm, structureRNAm);
    DetFileParser parser;
    const std::string detFile = temporalFileName + ".det";
    parser.parseDet(detFile, motifObserver);
    return freeEnergy;
    return 0;
}
} // namespace fideo