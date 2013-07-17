/*
 * @file      UNAFoldTest.cpp
 * @brief     UNAFoldTest is a test file to UNAFold backend.
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
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define private public

#include <unistd.h>
#include <fideo/fideo.h>
#include <etilico/etilico.h>
#include <mili/mili.h>
#include <biopp/biopp.h>
#include "HelperTest.h"
#include <gtest/gtest.h> 
#define UNA_FOLD_H
#include "fideo/UNAFold.h"
#undef UNA_FOLD_H

using namespace fideo;

TEST(UnaFoldBackendTestSuite1, BasicTest)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* p = Fold::new_class("UNAFold");
    ASSERT_TRUE(p != NULL);

    EXPECT_NO_THROW(p->fold(seq, true, secStructure));
    delete p;

    EXPECT_EQ(32, secStructure.size());
    EXPECT_TRUE(secStructure.is_circular());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(UnaFoldBackendTestSuite1, InvalidBackend)
{
    IFold* unafold = Fold::new_class("UNAFOLD");    
    ASSERT_TRUE(unafold == NULL);
}

TEST(UnaFoldBackendTestSuite2, correctCommad1)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;
    
    IFoldIntermediate *unafold = new UNAFold();
    IntermediateFiles files;
    etilico::Command cmd;
    unafold->prepareData(seq, true, cmd, files);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 --circular ";
    cmdExpected << files[IFoldIntermediate::INPUT_FILE];
    EXPECT_EQ(cmdExpected.str(), cmd);    
    
    etilico::runCommand(cmd);
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(UnaFoldBackendTestSuite2, correctCommad2)
{
    const biopp::NucSequence seq("GGGGAAAAAAAAGGGGCCCCCCCCTTTTCCCCCCCTTTTT");
    biopp::SecStructure secStructure;
    IFoldIntermediate *unafold = new UNAFold();
    IntermediateFiles files;
    etilico::Command cmd;
    unafold->prepareData(seq, false, cmd, files);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 ";
    cmdExpected << files[IFoldIntermediate::INPUT_FILE];
    EXPECT_EQ(cmdExpected.str(), cmd);    
    etilico::runCommand(cmd);
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t COMMAND_NOT_FOUND = 127;

TEST(UnaFoldBackendTestSuite2, incorrectCommad)
{
    const etilico::Command cmd = "UNAFold.pl--max=1 file.in";
    EXPECT_EQ(etilico::runCommand(cmd), COMMAND_NOT_FOUND    );
}

TEST(UnaFoldBackendTestSuite2, InvalidHeaderParse)
{
    const std::string fileTest = "/tmp/fideo-fileTest";
    const std::string fileName = "/tmp/fideo-fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "32      dG=-15.1      fideo-9I1tef \n";
    file << "1       A       1       3       0       2       0       0 \n";
    file << "2       A       32      2       0       1       0       0 \n";
    file.close();
    IFoldIntermediate *unafold = new UNAFold();
    IntermediateFiles files;   
    files.push_back(fileTest);     
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(true, secStructure, 0, files, freeEnergy), InvalidaHeader);
    //To avoid "Error unlink" when calling the destructor
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    secStructure.clear();
    unafold->fold(seq, true, secStructure);    
    unlink(fileName.c_str());
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(UnaFoldBackendTestSuite2, InvalidBodyLineParse)
{
    const std::string fileTest = "/tmp/fideo-fileTest";
    const std::string fileName = "/tmp/fideo-fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "32      dG = -15.1      fideo-9I1tef \n";
    file << "1       A       1       3       0       2       0       0 \n";
    file << "2       A       32      2       0       1       0       0 \n";
    file << "2       A       32      2       0       1       0       \n";
    file.close();
    IFoldIntermediate *unafold = new UNAFold();
    IntermediateFiles files;   
    files.push_back(fileTest);     
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(true, secStructure, 0, files, freeEnergy), InvalidBodyLine);
    //To avoid "Error unlink" when calling the destructor
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    secStructure.clear();
    unafold->fold(seq, true, secStructure);
    unlink(fileName.c_str());
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(UnaFoldBackendTestSuite2, FileNotExist)
{
    const std::string fileTest = "/tmp/fideo-fileNotExist";
    const std::string fileName = "/tmp/fideo-fileNotExist.ct";    
    IFoldIntermediate *unafold = new UNAFold();
    IntermediateFiles files;   
    files.push_back(fileTest);     
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(true, secStructure, 0, files, freeEnergy), NotFoundFileException);
    //To avoid "Error unlink" when calling the destructor
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    secStructure.clear();
    unafold->fold(seq, true, secStructure);
    unlink(fileName.c_str());
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}