/*
 * @file      RNAFoldTest.cpp
 * @brief     RNAFoldTest is a test file to RNAFold backend.
 *
 * @author    Franco Riberi
 * @email     fgriberi AT gmail.com
 *
 * Contents:  Source file.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 2012
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.+
 *
 */

#define private public

#include <fideo/fideo.h>
#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include <gtest/gtest.h>
#include "HelperTest.h"
#define RNA_FOLD_H
#include "fideo/RNAFold.h"
#undef RNA_FOLD_H

using namespace fideo;

TEST(RNAFoldBackendTestSuite1, FoldTest)
{
    const biopp::NucSequence seq("AATTAAAAAAGGGGGGGTTGCAACCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* p = Fold::new_class("RNAFold");
    ASSERT_TRUE(p != NULL);

    Fe result = p->fold(seq, true, secStructure);
    delete p;

    EXPECT_DOUBLE_EQ(result, -18.70);
    EXPECT_TRUE(secStructure.is_circular());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAFoldBackendTestSuite1, FoldToTest)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* p = Fold::new_class("RNAFold");
    ASSERT_TRUE(p != NULL);

    std::string filePath = "/tmp/new-fideoRnafold";
    EXPECT_NO_THROW(p->foldTo(seq, true, secStructure, filePath));
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
    unlink(filePath.c_str());    
}

TEST(RNAFoldBackendTestSuite1, FoldFromTest)
{    
    const FilePath fileName = "/tmp/fideo-output";
    std::ofstream file(fileName.c_str());
    file << "AAAAAAAAGGGGGGGGCCCCCCCCUUUUUUUU\n";
    file << "..((((((((((((....)))))))))))).. (-13.60)\n";    
    file.close();

    IFold* p = Fold::new_class("RNAFold");
    ASSERT_TRUE(p != NULL);
    biopp::SecStructure structure;
    const Fe freeEnergy = p->foldFrom(fileName, structure);
    
    delete p;
    EXPECT_EQ(freeEnergy, -13.6);
    EXPECT_TRUE(HelperTest::checkDirTmp());    
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());    
}

TEST(RNAFoldBackendTestSuite1, InvalidBackend)
{
    IFold* rnafold = Fold::new_class("RNAfold");    
    ASSERT_TRUE(rnafold == NULL);
}

TEST(RNAFoldBackendTestSuite1, getSizeOfSequence)
{
    const biopp::NucSequence seq("AATTAAAAAAGGGGGGGTTGCAACCCCCCCTTTTTTTTCCCCCCCCTCCATTTTTTTTT");
    biopp::SecStructure secStructure;
    RNAFold rnafold;
    IntermediateFiles files;
    etilico::Command cmd;
    rnafold.prepareData(seq, true, cmd, files);   
    etilico::runCommand(cmd);

    EXPECT_EQ(rnafold.getSizeOfSequence(files[IFoldIntermediate::OUTPUT_FILE]), seq.length());
    unlink((files[IFoldIntermediate::INPUT_FILE]).c_str());
    unlink((files[IFoldIntermediate::OUTPUT_FILE]).c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());    
}


TEST(RNAFoldBackendTestSuite2, correctCommad1)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;
    RNAFold rnafold;
    IntermediateFiles files;
    etilico::Command cmd;
    rnafold.prepareData(seq, true, cmd, files);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAfold --noPS --circ < ";
    cmdExpected << files[IFoldIntermediate::INPUT_FILE]; 
    cmdExpected << " > ";
    cmdExpected << files[IFoldIntermediate::OUTPUT_FILE];     

    EXPECT_EQ(cmdExpected.str(), cmd);    
    unlink((files[IFoldIntermediate::INPUT_FILE]).c_str());
    unlink((files[IFoldIntermediate::OUTPUT_FILE]).c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAFoldBackendTestSuite2, correctCommad2)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;
    RNAFold rnafold;
    IntermediateFiles files; 
    etilico::Command cmd;
    rnafold.prepareData(seq, false, cmd, files);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAfold --noPS < ";
    cmdExpected << files[IFoldIntermediate::INPUT_FILE]; 
    cmdExpected << " > ";
    cmdExpected << files[IFoldIntermediate::OUTPUT_FILE];     
    EXPECT_EQ(cmdExpected.str(), cmd);
    unlink((files[IFoldIntermediate::INPUT_FILE]).c_str());
    unlink((files[IFoldIntermediate::OUTPUT_FILE]).c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

static const size_t UNRECOGNIZED_OPTION = 1;

TEST(RNAFoldBackendTestSuite2, incorrectCommad)
{
    const etilico::Command cmd = "RNAfold -nPS -c < fileInput > fileOutput";    
    EXPECT_EQ(etilico::runCommand(cmd), UNRECOGNIZED_OPTION);
}

TEST(RNAFoldBackendTestSuite2, FileNotExist)
{
    const std::string fileTest1 = "/tmp/fideo-FileNotExist1";
    const std::string fileTest2 = "/tmp/fideo-FileNotExist2";    
    RNAFold rnafold;
    IntermediateFiles files;   
    files.push_back(fileTest1);     
    files.push_back(fileTest2);     
    biopp::SecStructure secStructure;
    Fe freeEnergy;
    EXPECT_THROW(rnafold.processingResult(secStructure, files, true, freeEnergy), NotFoundFileException);    
    EXPECT_FALSE(HelperTest::checkDirTmp());      
}

TEST(RNAFoldBackendTestSuite2, InvalidFile)
{
    const std::string obsoleteInputFile = "/tmp/fideo-invalidFile";
    const std::string fileName = "/tmp/fideo-rnafold.test";
    std::ofstream file(fileName.c_str());
    file << "AAUUAAAAAAGGGGGGGUUGCAACCCCCCCUUUUUUUU \n";    
    file.close();
    RNAFold rnafold;
    IntermediateFiles files;   
    files.push_back(obsoleteInputFile);
    files.push_back(fileName);     
    biopp::SecStructure secStructure;
    Fe freeEnergy;    
    EXPECT_THROW(rnafold.processingResult(secStructure, files, true, freeEnergy), RNABackendException);           
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());         
}

TEST(RNAFoldBackendTestSuite2, InvalidFile2)
{
    const std::string obsoleteInputFile = "/tmp/fideo-invalidFile2";
    const std::string fileName = "/tmp/fideo-rnafold.test";
    std::ofstream file(fileName.c_str());
    file << "AAUUAAAAAAGGGGGGGUUGCAACCCCCCCUUUUUUUU \n";    
    file << "\n";
    file.close();
    RNAFold rnafold;
    IntermediateFiles files;   
    files.push_back(obsoleteInputFile);
    files.push_back(fileName);     
    biopp::SecStructure secStructure;
    fideo::Fe freeEnergy;
    EXPECT_THROW(rnafold.processingResult(secStructure, files, true, freeEnergy), RNABackendException);           
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());            
}