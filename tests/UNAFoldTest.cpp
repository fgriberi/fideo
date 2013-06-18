/*
 * @file      unafold_test.cpp
 * @brief     unafold_test is a test file to UNAFold backend.
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

#include <string>
#include <fstream>
#include <iostream>
#include <fideo/fideo.h>
#include <etilico/etilico.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include <gtest/gtest.h>
#define UNA_FOLD_H
#include "fideo/UNAFold.h"
#undef UNA_FOLD_H
#include "HelperTest.h"

using namespace fideo;
using namespace etilico;

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

TEST(UnaFoldBackendTestSuite2, correctCommad1)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;
    UNAFold unafold;
    Command cmd;
    unafold.prepareFileToFold(seq, true, cmd);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 --circular ";
    cmdExpected << unafold.temporalFileName;

    EXPECT_EQ(cmdExpected.str(), cmd);
    etilico::runCommand(cmd);
}

TEST(UnaFoldBackendTestSuite2, correctCommad2)
{
    const biopp::NucSequence seq("GGGGAAAAAAAAGGGGCCCCCCCCTTTTCCCCCCCTTTTT");
    biopp::SecStructure secStructure;
    UNAFold unafold;
    etilico::Command cmd;
    unafold.prepareFileToFold(seq, false, cmd);

    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "UNAFold.pl --max=1 ";
    cmdExpected << unafold.temporalFileName;

    EXPECT_EQ(cmdExpected.str(), cmd);
    etilico::runCommand(cmd);
}

TEST(UnaFoldBackendTestSuite2, InvalidFileToParse)
{
    const std::string fileTest = "fileTest";
    const std::string fileName = "fileTest.ct";
    std::ofstream file(fileName.c_str());
    file << "32      dG = -15.1      fideo-9I1tef \n";
    file << "1       A       32      2       0       1       0       0 \n";
    file << "2       A       1       3       0       2       0       0 \n";
    file << "3       A       2       4       30      3       0       \n";
    file.close();
    UNAFold unafold;
    unafold.temporalFileName = fileTest;
    biopp::SecStructure secStructure;
    Fe freeEnergy;

    EXPECT_THROW(unafold.parseCTFile(true, secStructure, freeEnergy), InvalidBodyLine);

    //To avoid "Error unlink" when calling the destructor
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    secStructure.clear();
    unafold.fold(seq, true, secStructure);
}
