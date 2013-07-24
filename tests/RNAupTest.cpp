/*
 * @file      RNAupTest.cpp
 * @brief     RNAupTest is a test file to RNAup backend.
 *
 * @author    Franco Riberi
 * @email     fgriberi AT gmail.com
 *
 * Contents:  Source file.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date 2013
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

#include <string>
#include <fstream>
#include <fideo/fideo.h>
#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include <gtest/gtest.h>
#include "HelperTest.h"
#define RNA_UP_H
#include "fideo/RNAup.h"
#undef RNA_UP_H

using namespace fideo;

static const std::string FILE_NAME = "RNA_w25_u2.out";

TEST(RNAupBackendTestSuite1, BasicTest)
{
    const biopp::NucSequence seq1("GGAGUAGGUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCU");

    IHybridize* p = Hybridize::new_class("RNAup");
    EXPECT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -6.72);
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
    EXPECT_FALSE(std::ifstream(FILE_NAME.c_str()));
}

TEST(RNAupBackendTestSuite1, InvalidBackend)
{
    IHybridize* rnaup = Hybridize::new_class("RNAUP");
    ASSERT_TRUE(rnaup == NULL);
}

TEST(RNAupBackendTestSuite2, correctCommad)
{
    const std::string seq1 = "AAAAAAAAGGGGGGGGCCCCCCCCTTTAAGGGGGGGGCCCCCCCCTTTTTTTT";
    const std::string seq2 = "AAGAUGUGGAAAAAUUGGAAUC";
    const biopp::NucSequence longer(seq1);
    const biopp::NucSequence shorter(seq2);

    biopp::SecStructure secStructure;
    RNAup rnaup;
    IHybridizeIntermediate::IntermediateFiles files;
    etilico::Command cmd;    
    rnaup.prepareData(longer, shorter, cmd, files);
    
    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAup -u 3,4 -c SH < ";
    cmdExpected << files[IHybridizeIntermediate::FILE_1];
    cmdExpected << " > ";
    cmdExpected << files[IHybridizeIntermediate::FILE_2];
    EXPECT_EQ(cmdExpected.str(), cmd);
    unlink((files[IHybridizeIntermediate::FILE_1]).c_str());
    unlink((files[IHybridizeIntermediate::FILE_2]).c_str());    
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t INVALID_COMMAND = 1;
TEST(RNAupBackendTestSuite2, incorrectCommad)
{
   const etilico::Command cmd = "RNAup -u -c SH < inputTmpFile > outputTmpFile";      
   EXPECT_EQ(etilico::runCommand(cmd), INVALID_COMMAND);
}

TEST(RNAupBackendTestSuite2, FileNotExist)
{
    const std::string obsoleteFile = "/tmp/fideo-fileNotExist";    
    const std::string fileName = "/tmp/fideo-rnaup.test";
    RNAup rnaup;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);        
    files.push_back(fileName);    
    Fe freeEnergy;
    EXPECT_THROW(rnaup.processingResult(files, freeEnergy), NotFoundFileException);  
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(RNAupBackendTestSuite1, InvalidFile)
{
    const std::string obsoleteFile = "/tmp/fideo-invalidFile";    
    const std::string fileName = "/tmp/fideo-rnaup.test";
    std::ofstream file(fileName.c_str());
    file << "((((((.&.))))))19,25:1,7   (-4.08 = -5.10 + 1.02) \n";
    file << "AUACUUU&AGAGUGU\n";
    file << "RNAup output in file: RNA_w25_u2.out\n";    
    file.close();

    RNAup rnaup;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnaup.processingResult(files, freeEnergy), RNABackendException);    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}