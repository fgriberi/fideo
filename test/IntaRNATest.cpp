/*
    Copyright (C) 2012 Franco Riberi, FuDePAN

    This file is part of the Fideo Library.

    Fideo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fideo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fideo. If not, see <http://www.gnu.org/licenses/>.

    This is a test file.
*/

#include <string>
#include <fstream>
#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(IntaRNABackendTestSuite, BasicTest1)
{
    const biopp::NucSequence seq1("GGAGUGGAGUAGGGGCCGCAAUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCUUUGC");

    IHybridize* p = mili::FactoryRegistry<IHybridize, std::string>::new_class("IntaRNA");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, seq2, false);
    ASSERT_DOUBLE_EQ(dG, -5.23621);
    delete p;

    EXPECT_FALSE(std::ifstream("outputIntaRNA.out"));

}

TEST(RNAupBackendTestSuite, BasicTest2)
{
    const std::string sequence1 = "AAGGTCAGTTCCACATTGGCATCTGAACTCGGTATCACGGCCGAGGATGCAAGGCTTGTTTCAGAGATTG"
    "CAATGCATACTACTGAGGACAGGATCAGTAGAGCGGTTGGACCCAGGCAAGCCCAAGTGTCATTTCTACACGGTGATCAAAGTGAGAATGAGCTACCGG"
    "GATTGGGAGGTAAGGAAGATAAGAGAGTCAAACAGAGTCGAGGAGAAGCCAGGGAGAGCTATAGAGAAACTGGGCACAGCAGAGCAAATGATGCGAGAG"
    "CTGCTGACCTTCCAACCGGCACACCCCTAGACATTGACACTGCATCGGAGTTCAGCCAAGACCCACAGGACAGTCGAAGGTCAGCTGACGCCCTGCTCA"
    "GGCTGCAAGCCATGGCAGGGATCCCGGAAGAACAAGGCTCAGACATGGACACCCCTAGAGTGTACAATGACAGAGATCTTCTAGACTAG";
    const std::string sequence2 = "AAGAUGUGGAAAAAUUGGAAUC";

    const biopp::NucSequence seq1(sequence1);
    const biopp::NucSequence seq2(sequence2);

    IHybridize* p = mili::FactoryRegistry<IHybridize, std::string>::new_class("IntaRNA");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, seq2, false);
    ASSERT_DOUBLE_EQ(dG, -7.34977);
    delete p;

    EXPECT_FALSE(std::ifstream("outputIntaRNA.out"));
}
