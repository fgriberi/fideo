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

    IHybridize* const p = Hybridize::new_class("RNAcofold");
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
    IHybridizeIntermediate::InputFiles inFiles;
    IHybridizeIntermediate::OutputFile outFile;
    etilico::Command cmd;    
    rnacofold.prepareData(longer, shorter, cmd, inFiles, outFile);
    
    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAcofold --temp=37 < "; 
    cmdExpected << inFiles[IHybridizeIntermediate::FILE_1];
    cmdExpected << " > ";
    cmdExpected << outFile;
    EXPECT_EQ(cmdExpected.str(), cmd);
    unlink((inFiles[IHybridizeIntermediate::FILE_1]).c_str());
    unlink(outFile.c_str());
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
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-cofoldNotExist.test";        
    RNAcofold rnacofold;
    Fe freeEnergy;
    EXPECT_THROW(rnacofold.processingResult(outFile, freeEnergy), NotFoundFileException);  
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAcofoldBackendTestSuite2, InvalidFile)
{        
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-rnacofold.test";
    std::ofstream file(outFile.c_str());
    RNAcofold rnacofold;
    Fe freeEnergy;

    EXPECT_THROW(rnacofold.processingResult(outFile, freeEnergy), InvalidOutputRNACofold);    
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAcofoldBackendTestSuite2, InvalidFile2)
{
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-rnacofold.test";
    std::ofstream file(outFile.c_str());
    file << "UGUUGACUGUUUUUUACCUAAGGGGAAAUUAUCA&AGAGUGUGAUG \n";
    file << "(((())))(((((((((((((()))))))&)))))))(-9.50)";   
    file.close();
    RNAcofold rnacofold;
    Fe freeEnergy;

    EXPECT_THROW(rnacofold.processingResult(outFile, freeEnergy), InvalidOutputRNACofold);    
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}