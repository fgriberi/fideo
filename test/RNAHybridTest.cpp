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
#include "HelperTest.h"

TEST(RNAHybridBackendTestSuite, BasicTest1)
{
    const std::string seq1 = "GTAGTGTACCCCACTTGAATACTTTGAAAATAAATTGTTGTTGACTGTTTTTTACCTAAGGGGAAATTA"
                             "TCAAGAGTGTGATGTCGGATTTGGTGTTTTATGATGTCGCATCAACAGGTGCAAATGGATTTGATCCTGA"
                             "TGCAGGGTATATGGACTTCTGTGTTAAAAATGCAGAATTACTCAACCTTGCTGCAGTTAGGATCTTCTTC"
                             "CTCAATGCCGCAAAGGCCAAGGCTGCTCTCTCGCGTAAGCCAGAGAGGAAGGCTAACCCTAAATTTGGAG"
                             "AGTGGCAGGTGGAGGTTATCAATAATCATT";
    const std::string seq2 = "AAGAUGUGGAAAAAUUGGAAUC";

    const biopp::NucSequence targetSequence(seq1);
    const biopp::NucSequence querySequence(seq2);

    fideo::IHybridize* p = mili::FactoryRegistry<fideo::IHybridize, std::string>::new_class("RNAHybrid");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(targetSequence, querySequence, false);
    EXPECT_DOUBLE_EQ(dG, -16.4);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAHybridBackendTestSuite, TestExampleInRNAhybridPackage)
{
    const std::string seq1 = "TGAGGACGTCCTCGTTAAGGAAACACTTCCCATAGCCCCTTACCCTCGTCTAGTGACCCATTCTGAAATCGGCAGATAGA"
                             "CTTTCATGTAGCTGGTTTAAGTTTTCTCTTCATTTTCTTTAACTTATCAATGTTCGGGCGTTACCACTTTTCTAATCATG"
                             "GCCAGTTTCTTGCACGTACACCTCAACCGTCCATTTCCATGAAATCTCATACTTGTTACTGTTTTCTTTTACCTCTGATG"
                             "TCACATTTTCTCTCTGTCTCACTTTCTACCTCCAAACATATTTTACTACTTGTATTGAATGCCAAAAAATACCATATTTA"
                             "TTAAGGAGCATTGTTCATTTACAGTTTTGTACTCTCAGAGCGTGTTATTATCTAGAAGCAATTGTATACTGTTCTCAGTA"
                             "CATGTAGTACCTCCCCCAGAATACTTGTTTCTGTTACTATGTACCCCTCTTATTAACTTCGGGATATGAAACTTTTTATG"
                             "TTTCATTTTCTATTGATTTCATTTGTTTGTCATTTTCAAGCTCCTCTTTCCACATAAGCTTTAACTGCATGTCTTTCATT"
                             "TTTATTTATTTCTATTTGCCAATTGTTTAACTATGCACACATTTGTTTCATGTTTCTCCAGAGATAACTTTCCCAAATTC"
                             "AAGTTTGCGCCAACTCGTGCTGCTCTTTTATTGTACGGTTTTATAACGTTTCCGTCTTGAAATCAGAGATTGTAGCCGTT"
                             "TTTTGAAAAGGATATGCCAAAGAATCGTCCCCCACCCTCTAGTTGTCATTTGTTAAATAGCCGAAGTGACCCAACAACCC"
                             "GCTTTTGTCCCCTCTACTAATAACCGTTTTATTATTATTATCACTCAATATTTATCTTTTTATGTACTTCTTTCACTGCT"
                             "CCCATGTCGTGATTTCTGATTTCACATTTTCCAGACTATCTCGCACTTTCATTCTACCTCAATACATCCCAGCTTTTTTG"
                             "CCATACATTCTCCGATTCGAATTCATGTGTGCTCGTTTAACTATTATTACCTGTATCCACCGATTACTTTTTTGTTTATT"
                             "CGCTCCCTTTTTTCTATCTCAGGAATGATTTATAGTTTTCAATTTGTCTTCTCACAACTCATCTAAACTACTTGTCCGCT"
                             "ACCTTATGTACCTCATTGACTCATTTTGCCATCACCCAATACAATTTATACCTCAATACTGTCTCTTACCTGTATAATGC"
                             "CTTCTACCTCCAATTTTTACCATCTATTCTAGTTAATTACCATTTTCTACCTCAACCCATTTTCTATTATACAACCGTTC"
                             "CACCTCAAACTTCAGTGCGTTCTTCTGTCATCATGTACAATTTTCTTTCTTCGAATTTTGATTCGAATGTCAATTTATCA"
                             "ATTTATAAAAACTCCAATAAAAAAGCATCTTGAAGCATCTTGTTTTACCACATATATCAAAACTTCAAAGTACACAATTA"
                             "ATCGGATCATCAGAAAAA";
    const std::string seq2 = "UGAGGUAGUAGGUUGUAUAGUU";

    const biopp::NucSequence targetSequence(seq1);
    const biopp::NucSequence querySequence(seq2);

    fideo::IHybridize* p = mili::FactoryRegistry<fideo::IHybridize, std::string>::new_class("RNAHybrid");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(targetSequence, querySequence, false);
    EXPECT_DOUBLE_EQ(dG, -28.2);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}
