/*
 * @file      IntaRNATest.cpp
 * @brief     IntaRNATest is a test file to IntaRNA backend.
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

#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <gtest/gtest.h>
#include "HelperTest.h"

using namespace fideo;

//temporal
int linkFictitious();

TEST(IntaRNABackendTestSuite, BasicTest1)
{
    linkFictitious();
    const biopp::NucSequence seq1("GGAGUGGAGUAGGGGCCGCAAUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCUUUGC");

    IHybridize* p = Hybridize::new_class("IntaRNA");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -5.23621);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(IntaRNABackendTestSuite, BasicTest2)
{
    const std::string sequence1 = "AAGGTCAGTTCCACATTGGCATCTGAACTCGGTATCACGGCCGAGGATGCAAGGCTTGTTTCAGAGATTG"
                                  "CAATGCATACTACTGAGGACAGGATCAGTAGAGCGGTTGGACCCAGGCAAGCCCAAGTGTCATTTCTACA"
                                  "CGGTGATCAAAGTGAGAATGAGCTACCGGGATTGGGAGGTAAGGAAGATAAGAGAGTCAAACAGAGTCGA"
                                  "GGAGAAGCCAGGGAGAGCTATAGAGAAACTGGGCACAGCAGAGCAAATGATGCGAGAGCTGCTGACCTTC"
                                  "CAACCGGCACACCCCTAGACATTGACACTGCATCGGAGTTCAGCCAAGACCCACAGGACAGTCGAAGGTC"
                                  "AGCTGACGCCCTGCTCAGGCTGCAAGCCATGGCAGGGATCCCGGAAGAACAAGGCTCAGACATGGACACC"
                                  "CCTAGAGTGTACAATGACAGAGATCTTCTAGACTAG";
    const std::string sequence2 = "AAGAUGUGGAAAAAUUGGAAUC";

    const biopp::NucSequence seq1(sequence1);
    const biopp::NucSequence seq2(sequence2);

    IHybridize* p = Hybridize::new_class("IntaRNA");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -7.34977);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}
