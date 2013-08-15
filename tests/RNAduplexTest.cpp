/*
 * @file      RNAduplexTest.cpp
 * @brief     RNARNAduplexTest is a test file to RNAduplex backend.
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
#include <biopp/biopp.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HelperTest.h"
#define RNA_DUPLEX_H
#include "fideo/RNAduplex.h"
#undef RNA_DUPLEX_H

using namespace fideo;

TEST(RNAduplexBackendTestSuite1, BasicTest)
{
    const biopp::NucSequence seq1("GGAGUGGAGUAGGGGCCGCAAUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCUUUGC");

    IHybridize* p = Hybridize::new_class("RNAduplex");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -8.30);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAduplexBackendTestSuite2, correctCommad)
{	
    const biopp::NucSequence longer("AAAAAAAAGGGGGGGGCCCCCCCCTTTAAGGGGGGGGCCCCCCCCTTTTTTTT");
    const biopp::NucSequence shorter("AAGAUGUGGAAAAAUUGGAAUC");

    biopp::SecStructure secStructure;
    RNAduplex rnaduplex;
    IHybridizeIntermediate::IntermediateFiles files;
    etilico::Command cmd;    
    rnaduplex.prepareData(longer, shorter, cmd, files);
    
    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAduplex < "; 
    cmdExpected << files[IHybridizeIntermediate::FILE_1];
    cmdExpected << " > ";
    cmdExpected << files[IHybridizeIntermediate::FILE_2];
    EXPECT_EQ(cmdExpected.str(), cmd);

    unlink((files[IHybridizeIntermediate::FILE_1]).c_str());
    unlink((files[IHybridizeIntermediate::FILE_2]).c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t INVALID_COMMAND = 1;
TEST(RNAduplexBackendTestSuite2, incorrectCommad)
{
   const etilico::Command cmd = "RNAcofold<inputTmpFile outputTmpFile";      
   EXPECT_EQ(etilico::runCommand(cmd), INVALID_COMMAND);
}

TEST(RNAduplexBackendTestSuite2, FileNotExist)
{
    const std::string obsoleteFile = "/tmp/fideo-fileNotExist";
    const std::string fileName = "/tmp/fideo-rnaduplex.test";        
    RNAduplex rnaduplex;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnaduplex.processingResult(files, freeEnergy), NotFoundFileException);  
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(RNAduplexBackendTestSuite2, InvalidFile)
{
    const std::string obsoleteFile = "/tmp/fideo-fileNotExist";
    const std::string fileName = "/tmp/fideo-rnaduplex.test";        
    std::ofstream file(fileName.c_str());
    RNAduplex rnaduplex;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnaduplex.processingResult(files, freeEnergy), InvalidOutputRNADuplex);    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAduplexBackendTestSuite2, InvalidFile2)
{
    const std::string obsoleteFile = "/tmp/fideo-fileNotExist2";
    const std::string fileName = "/tmp/fideo-rnaduplex.test";        
    std::ofstream file(fileName.c_str());
    file << ".(((((((.&))))))).18,26:1,8(-5.10)\n";   
    file.close();

    RNAduplex rnaduplex;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnaduplex.processingResult(files, freeEnergy), InvalidOutputRNADuplex);    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}