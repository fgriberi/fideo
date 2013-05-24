/*
 * @file      DetFileTest.cpp
 * @brief     DetFileTest is a test file to det parse.
 *
 * @author    Franco Riberi
 * @email     fgriberi AT gmail.com
 *
 * Contents:  Source file.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date May 13, 2013
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define private public
#define protected public

#include <mili/mili.h>
#include <biopp/biopp.h>
#include <fideo/fideo.h>
#include <etilico/etilico.h>
#include <gtest/gtest.h>
#include "src/UNAFold.cpp" //Only include .cpp in tests file
#include "HelperTest.h"

using namespace fideo;
using namespace biopp;

TEST(DetFileParserTestSuite, RemoveConsecutiveWhiteSpacesTest)
{
    const std::string src1 = "Stack:           ddG =  -2.20 External closing pair is A(     2)-T(   204)";
    const std::string src2 = "Bulge    loop:      ddG  =  +1.70   External closing pair is C(    59)-G(   186)";
    std::string dest1;
    std::string dest2;
    UNAFold::DetFileParser temporalParser;
    temporalParser.removeConsecutiveWhiteSpaces(src1, dest1);
    temporalParser.removeConsecutiveWhiteSpaces(src2, dest2);

    const std::string expected1 = "Stack: ddG = -2.20 External closing pair is A( 2)-T( 204)";
    const std::string expected2 = "Bulge loop: ddG = +1.70 External closing pair is C( 59)-G( 186)";

    EXPECT_EQ(dest1, expected1);
    EXPECT_EQ(dest2, expected2);
}

TEST(DetFileParserTestSuite, parseMotifLineTest)
{

    Block temporalBlock;
    const std::string externalLoopLine = "External loop:   ddG =  -4.40 28 ss bases & 3 closing helices";
    const std::string dataExternalLoop = ":   ddG =  -4.40 28 ss bases & 3 closing helices";
    UNAFold::DetFileParser parser;
    parser.parseMotifLine(externalLoopLine, temporalBlock);
    EXPECT_EQ(temporalBlock.motifName, EXTERNAL_LOOP);
    EXPECT_EQ(temporalBlock.lines.front(), dataExternalLoop);
    temporalBlock.lines.clear();

    const std::string interiorMotif = "Interior loop:   ddG =  +2.70 External closing pair is U(   489)-G(   604)";
    const std::string dataInteriorLoop = ":   ddG =  +2.70 External closing pair is U(   489)-G(   604)";
    parser.parseMotifLine(interiorMotif, temporalBlock);
    EXPECT_EQ(temporalBlock.motifName, INTERIOR_LOOP);
    EXPECT_EQ(temporalBlock.lines.front(), dataInteriorLoop);
    temporalBlock.lines.clear();

    const std::string otherMotif = "Other loop   ddG =  +2.70 External closing pair is A(   41)-G(   222)";
    EXPECT_THROW(parser.parseMotifLine(otherMotif, temporalBlock), mili::StringNotFound);
}

TEST(DetFileParserTestSuite, parseStackLineTest)
{
    const std::string stackLine = "Stack: ddG =  -1.40 External closing pair is A(   488)-U(   605)";
    const std::string helixLine = "Helix: ddG = -15.60 8 base pairs.";
    const std::string line = "Helix ddG = -65.60 17 base pairs.";

    std::string nameMot;

    UNAFold::DetFileParser parser;
    parser.parseStackLine(stackLine, nameMot);
    EXPECT_EQ(nameMot, "Stack");

    nameMot.clear();
    parser.parseStackLine(helixLine, nameMot);
    EXPECT_EQ(nameMot, "Helix");

    EXPECT_THROW(parser.parseStackLine(line, nameMot), mili::StringNotFound);
}

TEST(DetFileParserTestSuite, getSubstrInPosTest)
{
    const std::string firstLine  = "External loop: ddG = -0.20 16 ss bases & 1 closing helices";
    const std::string secondLine = "Helix: ddG = -7.90 4 base pairs.";
    const std::string thirdLine  = "Stack: ddG = -2.10 External closing pair is C( 35)-G( 197)";

    std::string toFillFirstLine;
    std::string toFillSecondLine;
    std::string toFillThirdLine;

    UNAFold::DetFileParser::Rule* rule;

    rule->getSubstrInPos(firstLine, 5, toFillFirstLine);
    EXPECT_EQ(toFillFirstLine, "16");

    rule->getSubstrInPos(secondLine, 4, toFillSecondLine);
    EXPECT_EQ(toFillSecondLine, "4");

    EXPECT_THROW(rule->getSubstrInPos(thirdLine, 28, toFillThirdLine), IndexOutOfRange);
}

TEST(DetFileParserTestSuite, getInitAndEndPosOfNucleotidTest)
{
    const std::string interiorLoop  = ": ddG = +2.30 External closing pair is T( 97)-A( 116)";
    const std::string hairpinLoop   = ": ddG = +5.70 Closing pair is G( 16)-C( 20)";
    const std::string bulgeLoop     = ": ddG = +1.70 External closing pair is C( 59)-G( 186)";
    const std::string stack         = "Stack: ddG = -1.30 External closing pair is T( 70)-A( 174)";

    UNAFold::DetFileParser temporalParser;
    std::string tmp;
    temporalParser.removeConsecutiveWhiteSpaces(stack, tmp);

    UNAFold::DetFileParser::Rule* rule;
    const size_t initInteriorLoop = rule->getInitPosOfNucleotid(interiorLoop, INTERIOR_LOOP);
    EXPECT_EQ(initInteriorLoop, 97);
    const size_t endInteriorLoop = rule->getEndPosOfNucleotid(interiorLoop, INTERIOR_LOOP);
    EXPECT_EQ(endInteriorLoop, 116);

    const size_t initHairpinLoop = rule->getInitPosOfNucleotid(hairpinLoop, HAIRPIN_LOOP);
    EXPECT_EQ(initHairpinLoop, 16);
    const size_t endHairpinLoop = rule->getEndPosOfNucleotid(hairpinLoop, HAIRPIN_LOOP);
    EXPECT_EQ(endHairpinLoop, 20);

    const size_t initBulgeLoop = rule->getInitPosOfNucleotid(bulgeLoop, BULGE_LOOP);
    EXPECT_EQ(initBulgeLoop, 59);
    const size_t endBulgeLoop = rule->getEndPosOfNucleotid(bulgeLoop, BULGE_LOOP);
    EXPECT_EQ(endBulgeLoop, 186);

    const size_t initStack = rule->getInitPosOfNucleotid(stack, STACK);
    EXPECT_EQ(initStack, 70);
    const size_t endStack = rule->getEndPosOfNucleotid(stack, STACK);
    EXPECT_EQ(endStack, 174);
}

TEST(DetFileParserTestSuite, ExternalRuleCalcAttribTest)
{
    // build correct block
    Block externalLoopBlock;
    externalLoopBlock.motifName = "External loop";
    externalLoopBlock.lines.push_back(": ddG = -0.20 16 ss bases & 1 closing helices");
    externalLoopBlock.lines.push_back("Stack: ddG = -2.20 External closing pair is A( 2)-T( 204)");
    externalLoopBlock.lines.push_back("Stack: ddG = -3.30 External closing pair is C( 3)-G( 203)");
    externalLoopBlock.lines.push_back("Stack: ddG = -2.40 External closing pair is C( 4)-G( 202)");
    externalLoopBlock.lines.push_back("Helix: ddG = -7.90 4 base pairs.");

    const size_t attr = 16;
    const size_t stacks = 3;

    UNAFold::DetFileParser::ExternalRule externalRule;
    IMotifObserver::Motif motif;
    externalRule.calcAttrib(externalLoopBlock, motif);

    EXPECT_EQ(motif.nameMotif, EXTERNAL_LOOP);
    EXPECT_EQ(motif.attribute, attr);
    EXPECT_EQ(motif.stacks, stacks);

    // build incorrect block
    Block invalidBlock;
    invalidBlock.motifName = "External loops";
    invalidBlock.lines.push_back(": 16 ss bases & 1 closing helices");
    invalidBlock.lines.push_back("Stack: ddG = -2.20 External closing pair is A( 2)-T( 204)");
    invalidBlock.lines.push_back("Stack: ddG = -30 External closing pair is C( 3)-G( 203)");
    invalidBlock.lines.push_back("Stack: ddG = -2.40 External closing pair is C( 4)-G( 202)");
    invalidBlock.lines.push_back("Helix: ddG = -7.90 4 base pairs.");

    ASSERT_DEATH(externalRule.calcAttrib(invalidBlock, motif), "");
}

TEST(DetFileParserTestSuite, InteriorRuleCalcAttribTest)
{
    // build correct block
    Block interiorLoopBlock;
    interiorLoopBlock.motifName = "Interior loop";
    interiorLoopBlock.lines.push_back(": ddG = +2.30 External closing pair is T( 97)-A( 116)");
    interiorLoopBlock.lines.push_back("Stack: ddG = -2.40 External closing pair is G( 101)-C( 111)");
    interiorLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is A( 102)-T( 110)");
    interiorLoopBlock.lines.push_back("Helix: ddG = -3.30 3 base pairs.");

    const size_t attr = 9;
    const size_t stacks = 2;

    UNAFold::DetFileParser::InteriorRule interiorRule;
    IMotifObserver::Motif motif;
    interiorRule.calcAttrib(interiorLoopBlock, motif);

    EXPECT_EQ(motif.nameMotif, ASYMMETRIC);
    EXPECT_EQ(motif.attribute, attr);
    EXPECT_EQ(motif.stacks, stacks);

    // build incorrect block
    Block invalidBlock;
    invalidBlock.motifName = "Interior loop";
    invalidBlock.lines.push_back(": ");
    invalidBlock.lines.push_back(": ddG =  -2.20 External closing pair is A( 2)-T( 204)");
    invalidBlock.lines.push_back("Stack: ddG = -30 External closing is C( 3)-G( 203)");
    invalidBlock.lines.push_back("Stack: ddG = -2.40 External closing pair is C( 4)-G( 202)");
    invalidBlock.lines.push_back("Helix: ddG = -7.90 4 base pairs.");

    EXPECT_THROW(interiorRule.calcAttrib(invalidBlock, motif), IndexOutOfRange);
}

TEST(DetFileParserTestSuite, HairpinRuleCalcAttribTest)
{
    // build correct block
    Block hairpinLoopBlock;
    hairpinLoopBlock.motifName = "Hairpin loop";
    hairpinLoopBlock.lines.push_back(": ddG = +4.50 Closing pair is A( 42)-T( 47)");
    hairpinLoopBlock.lines.push_back("Stack: ddG = -2.10 External closing pair is A( 13)-T( 23)");
    hairpinLoopBlock.lines.push_back("Stack: ddG = -2.20 External closing pair is G( 14)-C( 22)");
    hairpinLoopBlock.lines.push_back("Stack: ddG = -2.10 External closing pair is T( 15)-A( 21)");
    hairpinLoopBlock.lines.push_back("Helix: ddG = -6.40 4 base pairs.");

    const size_t attr = 5;
    const size_t stacks = 3;

    UNAFold::DetFileParser::HairpinRule hairpinRule;
    IMotifObserver::Motif motif;
    hairpinRule.calcAttrib(hairpinLoopBlock, motif);

    EXPECT_EQ(motif.nameMotif, HAIRPIN_LOOP);
    EXPECT_EQ(motif.attribute, attr);
    EXPECT_EQ(motif.stacks, stacks);

    // build other block
    Block withoutStackBlock;
    withoutStackBlock.motifName = "Hairpin loop";
    withoutStackBlock.lines.push_back(": ddG = +5.70 Closing pair is G( 16)-C( 20)");

    const size_t numAttrib = 4;
    const size_t amountStacks = 0;

    hairpinRule.calcAttrib(withoutStackBlock, motif);

    EXPECT_EQ(motif.nameMotif, HAIRPIN_LOOP);
    EXPECT_EQ(motif.attribute, numAttrib);
    EXPECT_EQ(motif.stacks, amountStacks);
}

TEST(DetFileParserTestSuite, MultiLoopRuleCalcAttribTest)
{
    // build correct block
    Block multiLoopBlock;
    multiLoopBlock.motifName = "Multi-loop";
    multiLoopBlock.lines.push_back(": ddG = +1.90 External closing pair is C( 76)-G( 168)");
    multiLoopBlock.lines.push_back(" 20 ss bases & 3 closing helices.");
    multiLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is A( 125)-T( 167)");
    multiLoopBlock.lines.push_back("Stack: ddG = -0.60 External closing pair is A( 126)-T( 166)");
    multiLoopBlock.lines.push_back("Stack: ddG = -2.50 External closing pair is G( 127)-T( 165)");
    multiLoopBlock.lines.push_back("Stack: ddG = -2.10 External closing pair is C( 128)-G( 164)");
    multiLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is T( 129)-A( 163)");
    multiLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is T( 130)-A( 162)");
    multiLoopBlock.lines.push_back("Helix: ddG = -7.90 7 base pairs.");

    const size_t attr = 20;
    const size_t stacks = 6;

    UNAFold::DetFileParser::MultiRule multiRule;
    IMotifObserver::Motif motif;
    multiRule.calcAttrib(multiLoopBlock, motif);

    EXPECT_EQ(motif.nameMotif, MULTI_LOOP);
    EXPECT_EQ(motif.attribute, attr);
    EXPECT_EQ(motif.stacks, stacks);
}

TEST(DetFileParserTestSuite, BulgeRuleCalcAttribTest)
{
    // build correct block
    Block bulgeLoopBlock;
    bulgeLoopBlock.motifName = "Bulge loop";
    bulgeLoopBlock.lines.push_back(": ddG = +1.70 External closing pair is C( 59)-G( 186)");
    bulgeLoopBlock.lines.push_back("Stack: ddG = -1.30 External closing pair is T( 60)-A( 184)");
    bulgeLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is A( 61)-T( 183)");
    bulgeLoopBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is A( 62)-T( 182)");
    bulgeLoopBlock.lines.push_back("Stack: ddG = -1.10 External closing pair is A( 63)-T( 181)");
    bulgeLoopBlock.lines.push_back("Helix: ddG = -4.20 5 base pairs.");

    const size_t attr = 2;
    const size_t stacks = 4;

    UNAFold::DetFileParser::BulgeRule bulgeRule;
    IMotifObserver::Motif motif;
    bulgeRule.calcAttrib(bulgeLoopBlock, motif);

    EXPECT_EQ(motif.nameMotif, BULGE_LOOP);
    EXPECT_EQ(motif.attribute, attr);
    EXPECT_EQ(motif.stacks, stacks);
}

static const std::string DET_FILE_PATH = "/projects/fideo/tests/fileToParse.det";

void getDetFilePath(std::string& path)
{
    std::string temporalPath;
    etilico::getCurrentPath(temporalPath);
    path = temporalPath + DET_FILE_PATH;
}

TEST(DetFileParserTestSuite, builFirstBlockTest)
{
    std::string nameFile;
    getDetFilePath(nameFile);
    std::ifstream fileToParse;
    fileToParse.open(nameFile.c_str());
    if (!fileToParse)
    {
        throw FileNotExist();
    }
    UNAFold::DetFileParser parser;
    parser.goToBegin(fileToParse);
    Block block;
    parser.buildBlock(fileToParse, block);

    Block expectedBlock;
    expectedBlock.motifName = "External loop";
    expectedBlock.lines.push_back(": ddG = -0.20 16 ss bases & 1 closing helices");
    expectedBlock.lines.push_back("Stack: ddG = -2.20 External closing pair is A( 2)-T( 204)");
    expectedBlock.lines.push_back("Stack: ddG = -3.30 External closing pair is C( 3)-G( 203)");
    expectedBlock.lines.push_back("Stack: ddG = -2.40 External closing pair is C( 4)-G( 202)");
    expectedBlock.lines.push_back("Helix: ddG = -7.90 4 base pairs.");

    EXPECT_EQ(block.motifName, expectedBlock.motifName);
    ASSERT_TRUE(std::equal(expectedBlock.lines.begin(), expectedBlock.lines.end(), block.lines.begin()));
}

// TEST(DetFileParserTestSuite, builBlockAndParserBlockTest)
// {
//     std::string nameFile;
//     getDetFilePath(nameFile);
//     std::ifstream fileToParse;
//     fileToParse.open(nameFile.c_str());
//     if (!fileToParse)
//     {
//         throw FileNotExist();
//     }

//     UNAFold::DetFileParser parser;
//     parser.fillRules();
//     parser.goToBegin(fileToParse);
//     Block block;
//     parser.buildBlock(fileToParse, block);

//     IMotifObserver::Motif motif;
//     parser.parseBlock(block, motif);
//     EXPECT_EQ(motif.nameMotif, block.motifName);
//     EXPECT_EQ(motif.attribute, 16);
//     EXPECT_EQ(motif.stacks, 3);
//     block.lines.clear();

//     parser.buildBlock(fileToParse, block);
//     parser.parseBlock(block, motif);
//     EXPECT_EQ(motif.nameMotif, block.motifName);
//     EXPECT_EQ(motif.attribute, 17);
//     EXPECT_EQ(motif.stacks, 4);
//     block.lines.clear();

//     parser.buildBlock(fileToParse, block);
//     parser.parseBlock(block, motif);
//     EXPECT_EQ(motif.nameMotif, block.motifName);
//     EXPECT_EQ(motif.attribute, 3);
//     EXPECT_EQ(motif.stacks, 2);

//     Block expectedBlock;
//     expectedBlock.motifName = "Multi-loop";
//     expectedBlock.lines.push_back(": ddG = +2.40 External closing pair is A( 37)-T( 195)");
//     expectedBlock.lines.push_back(" 3 ss bases & 3 closing helices.");
//     expectedBlock.lines.push_back("Stack: ddG = -0.90 External closing pair is A( 52)-T( 193)");
//     expectedBlock.lines.push_back("Stack: ddG = -2.10 External closing pair is A( 53)-T( 192)");
//     expectedBlock.lines.push_back("Helix: ddG = -3.00 3 base pairs.");

//     EXPECT_EQ(block.motifName, expectedBlock.motifName);
//     ASSERT_TRUE(std::equal(expectedBlock.lines.begin(), expectedBlock.lines.end(), block.lines.begin()));
// }

TEST(DetFileParserTestSuite, parseDetFileNotExistTest)
{
    std::string nameFile;
    getDetFilePath(nameFile);
    const std::string fileInvalid = nameFile + ".det";
    UNAFold::DetFileParser parser;
    IMotifObserver* observer;

    EXPECT_THROW(parser.parseDet(fileInvalid, observer), FileNotExist);
}

static const size_t AMOUNT_OF_BLOCKS = 15;

// TEST(DetFileParserTestSuite, builBlockWithLastBlockTest)
// {
//     std::string nameFile;
//     getDetFilePath(nameFile);
//     std::ifstream fileToParse;
//     fileToParse.open(nameFile.c_str());
//     if (!fileToParse)
//     {
//         throw FileNotExist();
//     }

//     UNAFold::DetFileParser parser;
//     parser.fillRules();
//     parser.goToBegin(fileToParse);
//     Block block;
//     for (int i = 0; i < AMOUNT_OF_BLOCKS; ++i)
//     {
//         block.lines.clear();
//         parser.buildBlock(fileToParse, block);
//     }

//     Block expectedBlock;
//     expectedBlock.motifName = "Hairpin loop";
//     expectedBlock.lines.push_back(": ddG = +5.70 Closing pair is G( 16)-C( 20)");

//     EXPECT_EQ(block.motifName, expectedBlock.motifName);
//     ASSERT_TRUE(std::equal(expectedBlock.lines.begin(), expectedBlock.lines.end(), block.lines.begin()));
// }