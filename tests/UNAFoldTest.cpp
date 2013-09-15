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

TEST(UnaFoldBackendTestSuite1, FoldTest)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* const p = Fold::new_class("UNAFold");
    ASSERT_TRUE(p != NULL);

    EXPECT_NO_THROW(p->fold(seq, true, secStructure));

    EXPECT_EQ(32, secStructure.size());
    EXPECT_TRUE(secStructure.is_circular());
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(UnaFoldBackendTestSuite1, FoldToTest)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* const p = Fold::new_class("UNAFold");
    ASSERT_TRUE(p != NULL);

    std::string filePath = "/tmp/fideo-UnafoldTo";
    EXPECT_NO_THROW(p->foldTo(seq, true, secStructure, filePath));
    EXPECT_TRUE(HelperTest::checkDirTmp());    
    delete p;
    unlink(filePath.c_str());
    unlink((filePath + ".det").c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());    
}

TEST(UnaFoldBackendTestSuite1, FoldFromTest)
{
    const FilePath fileName = "/tmp/fideo-fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "15  dG = 5.2    fideo-9RR4Qw \n";
    file << "1   A   15  2   0   1   0   0 \n";
    file << "2   U   1   3   0   2   0   0 \n";
    file << "3   A   2   4   11  3   0   4 \n";
    file << "4   G   3   5   10  4   3   0 \n";
    file << "5   G   4   6   0   5   0   0 \n";
    file << "6   C   5   7   0   6   0   0 \n";
    file << "7   C   6   8   0   7   0   0 \n";
    file << "8   C   7   9   0   8   0   0 \n";
    file << "9   C   8   10  0   9   0   0 \n";
    file << "10  C   9   11  4   10  0   11 \n";
    file << "11  U   10  12  3   11  10  0 \n";
    file << "12  U   11  13  0   12  0   0 \n";
    file << "13  U   12  14  0   13  0   0 \n";
    file << "14  U   13  15  0   14  0   0 \n";
    file << "15  U   14  1   0   15  0   0";
    file.close();

    IFold* const p = Fold::new_class("UNAFold");
    ASSERT_TRUE(p != NULL);
    biopp::SecStructure structure;
    const Fe freeEnergy = p->foldFrom(fileName, structure);    
    EXPECT_EQ(freeEnergy, 5.2);
    EXPECT_TRUE(HelperTest::checkDirTmp());    
    delete p;
    unlink(fileName.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(UnaFoldBackendTestSuite1, InvalidBackend)
{
    IFold* const unafold = Fold::new_class("UNAFOLD");    
    ASSERT_TRUE(unafold == NULL);
    delete unafold;
}

TEST(UnaFoldBackendTestSuite2, correctCommad1)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;
    IFoldIntermediate *unafold = new UNAFold();
    InputFile inFile;
    OutputFile outFile;
    etilico::Command cmd;
    unafold->prepareData(seq, true, cmd, inFile, outFile);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 --circular ";
    cmdExpected << inFile;
    EXPECT_EQ(cmdExpected.str(), cmd);    
    etilico::runCommand(cmd);    
    Fe freeEnergy;
    unafold->processingResult(secStructure, outFile, freeEnergy);
    delete unafold;    
    etilico::runCommand("rm /tmp/fideo-*");
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(UnaFoldBackendTestSuite2, correctCommad2)
{
    const biopp::NucSequence seq("GGGGAAAAAAAAGGGGCCCCCCCCTTTTCCCCCCCTTTTT");
    biopp::SecStructure secStructure;
    IFoldIntermediate *unafold = new UNAFold();
    InputFile inFile;
    OutputFile outFile;
    etilico::Command cmd;
    unafold->prepareData(seq, false, cmd, inFile, outFile);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 ";
    cmdExpected << inFile;
    EXPECT_EQ(cmdExpected.str(), cmd);    
    etilico::runCommand(cmd);
    Fe freeEnergy;
    unafold->processingResult(secStructure, outFile, freeEnergy);
    delete unafold;
    etilico::runCommand("rm /tmp/fideo-*");
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t COMMAND_NOT_FOUND = 127;

TEST(UnaFoldBackendTestSuite2, incorrectCommad)
{
    const etilico::Command cmd = "UNAFold.pl--max=1 file.in";
    EXPECT_EQ(etilico::runCommand(cmd), COMMAND_NOT_FOUND);
}

TEST(UnaFoldBackendTestSuite2, InvalidHeaderParse)
{    
    const std::string fileName = "/tmp/fideo-fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "32      dG=-15.1      fideo-9I1tef \n";
    file << "1       A       1       3       0       2       0       0 \n";
    file << "2       A       32      2       0       1       0       0 \n";
    file.close();
    IFoldIntermediate* const unafold = new UNAFold();
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(secStructure, fileName, freeEnergy), InvalidaHeader);
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
    const std::string fileName = "/tmp/fideo-fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "32      dG = -15.1      fideo-9I1tef \n";
    file << "1       A       1       3       0       2       0       0 \n";
    file << "2       A       32      2       0       1       0       0 \n";
    file << "2       A       32      2       0       1       0       \n";
    file.close();
    IFoldIntermediate* const unafold = new UNAFold();
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(secStructure, fileName, freeEnergy), InvalidBodyLine);
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
    const std::string fileName = "/tmp/fideo-fileNotExist.ct";    
    IFoldIntermediate* const unafold = new UNAFold(); 
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold->processingResult(secStructure, fileName, freeEnergy), NotFoundFileException);
    //To avoid "Error unlink" when calling the destructor
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    secStructure.clear();
    unafold->fold(seq, true, secStructure);
    unlink(fileName.c_str());
    delete unafold;
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}