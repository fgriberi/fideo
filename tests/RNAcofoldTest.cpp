/*
 * @file      RNAcofoldTest.cpp
 * @brief     RNAcofoldTest is a test file to RNAcofold backend.
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
#define RNA_COFOLD_H
#include "fideo/RNAcofold.h"
#undef RNA_COFOLD_H

using namespace fideo;

TEST(RNAcofoldBackendTestSuite1, BasicTest)
{
    const biopp::NucSequence seq1("GGAGUAGGUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCU");

    IHybridize* p = Hybridize::new_class("RNAcofold");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -10);
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAcofoldBackendTestSuite2, correctCommad)
{
    const biopp::NucSequence longer("AAAAAAAAGGGGGGGGCCCCCCCCTTTAAGGGGGGGGCCCCCCCCTTTTTTTT");
    const biopp::NucSequence shorter("AAGAUGUGGAAAAAUUGGAAUC");

    biopp::SecStructure secStructure;
    RNAcofold rnacofold;
    IHybridizeIntermediate::IntermediateFiles files;
    etilico::Command cmd;    
    rnacofold.prepareData(longer, shorter, cmd, files);
    
    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAcofold < "; 
    cmdExpected << files[IHybridizeIntermediate::FILE_1];
    cmdExpected << " > ";
    cmdExpected << files[IHybridizeIntermediate::FILE_2];
    EXPECT_EQ(cmdExpected.str(), cmd);
    unlink((files[IHybridizeIntermediate::FILE_1]).c_str());
    unlink((files[IHybridizeIntermediate::FILE_2]).c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t INVALID_COMMAND = 1;
TEST(RNAcofoldBackendTestSuite2, incorrectCommad)
{
   const etilico::Command cmd = "RNAcofold<inputTmpFile outputTmpFile";      
   EXPECT_EQ(etilico::runCommand(cmd), INVALID_COMMAND);
}

TEST(RNAcofoldBackendTestSuite2, FileNotExist)
{
    const std::string obsoleteFile = "/tmp/fideo-fileNotExist";  
    const std::string fileName = "/tmp/fideo-cofoldNotExist.test";        
    RNAcofold rnacofold;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnacofold.processingResult(files, freeEnergy), NotFoundFileException);  
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAcofoldBackendTestSuite2, InvalidFile)
{
    const std::string obsoleteFile = "/tmp/fideo-invalidFile"; 
    const std::string fileName = "/tmp/fideo-rnacofold.test";
    std::ofstream file(fileName.c_str());
    RNAcofold rnacofold;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnacofold.processingResult(files, freeEnergy), InvalidOutputRNACofold);    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAcofoldBackendTestSuite2, InvalidFile2)
{
    const std::string obsoleteFile1 = "/tmp/fideo-invalidFile2";    
    const std::string fileName = "/tmp/fideo-rnacofold.test";
    std::ofstream file(fileName.c_str());
    file << "UGUUGACUGUUUUUUACCUAAGGGGAAAUUAUCA&AGAGUGUGAUG \n";
    file << "(((())))(((((((((((((()))))))&)))))))(-9.50)";   
    file.close();

    RNAcofold rnacofold;
    IHybridizeIntermediate::IntermediateFiles files;       
    files.push_back(obsoleteFile1);    
    files.push_back(fileName);    
    Fe freeEnergy;

    EXPECT_THROW(rnacofold.processingResult(files, freeEnergy), InvalidOutputRNACofold);    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}