/*
 * @file      RNAHybridTest.cpp
 * @brief     RNAHybridTest is a test file to RNAHybrid backend.
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
#define RNA_HYBRID_H
#include "fideo/RNAHybrid.h"
#undef RNA_HYBRID_H

using namespace fideo;

TEST(RNAHybridBackendTestSuite1, BasicTest1)
{
    const std::string seq1 = "GTAGTGTACCCCACTTGAATACTTTGAAAATAAATTGTTGTTGACTGTTTTTTACCTAAGGGGAAATTA"
                             "TCAAGAGTGTGATGTCGGATTTGGTGTTTTATGATGTCGCATCAACAGGTGCAAATGGATTTGATCCTGA"
                             "TGCAGGGTATATGGACTTCTGTGTTAAAAATGCAGAATTACTCAACCTTGCTGCAGTTAGGATCTTCTTC"
                             "CTCAATGCCGCAAAGGCCAAGGCTGCTCTCTCGCGTAAGCCAGAGAGGAAGGCTAACCCTAAATTTGGAG"
                             "AGTGGCAGGTGGAGGTTATCAATAATCATT";

    const std::string seq2 = "AAGAUGUGGAAAAAUUGGAAUC";

    const biopp::NucSequence targetSequence(seq1);
    const biopp::NucSequence querySequence(seq2);

    IHybridize* const p = Hybridize::new_class("RNAHybrid");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(targetSequence, false, querySequence);
    EXPECT_DOUBLE_EQ(dG, -16.4);
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAHybridBackendTestSuite1, TestExampleInRNAhybridPackage)
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

    IHybridize* const p = Hybridize::new_class("RNAHybrid");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(targetSequence, false, querySequence);
    EXPECT_DOUBLE_EQ(dG, -28.2);
    delete p;
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAHybridBackendTestSuite1, InvalidBackend)
{
    IHybridize* const rnahybrid = Hybridize::new_class("RNAhybrid");
    ASSERT_TRUE(rnahybrid == NULL);
    delete rnahybrid;
}

TEST(RNAHybridBackendTestSuite2, correctCommad)
{
    const std::string seq1 = "AAAAAAAAGGGGGGGGCCCCCCCCTTTAAGGGGGGGGCCCCCCCCTTTTTTTT";
    const std::string seq2 = "AAGAUGUGGAAAAAUUGGAAUC";
    const biopp::NucSequence longer(seq1);
    const biopp::NucSequence shorter(seq2);

    biopp::SecStructure secStructure;
    RNAHybrid rnahybrid;
    IHybridizeIntermediate::InputFiles inFiles;
    IHybridizeIntermediate::OutputFile outFile;
    etilico::Command cmd;    
    rnahybrid.prepareData(longer, shorter, cmd, inFiles, outFile);
    
    EXPECT_TRUE(HelperTest::checkDirTmp());
    std::stringstream cmdExpected;
    cmdExpected << "RNAhybrid -s 3utr_human -t ";
    cmdExpected << inFiles[IHybridizeIntermediate::FILE_1];
    cmdExpected << " -q ";
    cmdExpected << inFiles[IHybridizeIntermediate::FILE_2];
    cmdExpected << " > ";
    cmdExpected << outFile;
    EXPECT_EQ(cmdExpected.str(), cmd);
    unlink((inFiles[IHybridizeIntermediate::FILE_1]).c_str());
    unlink((inFiles[IHybridizeIntermediate::FILE_2]).c_str());
    unlink(outFile.c_str());    
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

static const size_t ABORTING = 2;
TEST(RNAHybridBackendTestSuite2, incorrectCommad)
{
   const etilico::Command cmd = "RNAhybrid s 3utr_human -t -q filemiRNA > fileTmpOutput";   
   EXPECT_EQ(etilico::runCommand(cmd), ABORTING);
}

TEST(RNAHybridBackendTestSuite2, FileNotExist)
{    
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-rnahybrid.test";    
    RNAHybrid rnahybrid;
    Fe freeEnergy;
    EXPECT_THROW(rnahybrid.processingResult(outFile, freeEnergy), NotFoundFileException);  
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAHybridBackendTestSuite2, InvalidBodyFile)
{    
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-rnahybrid.test";
    std::ofstream file(outFile.c_str());
    file << "target: largerSequenceName\n\n";
    file << "length: 309\n";
    file << "miRNA : shorterSequenceName\n";
    file << "length: 22\n\n";
    file << "\n";
    file << "mfe: -34.3kcal/mol\n";
    file << "p-value:0.999995\n";
    file << "\n";
    file << "position  206\n";
    file << "target 5'  C    UCAAUG      A    3'\n";
    file << "            UUCC       CCGCA\n";
    file << "            AAGG       GGUGU\n";
    file << "miRNA  3' CU    UUAAAAA     AGAA 5'\n";
    file.close();
    RNAHybrid rnahybrid;    
    Fe freeEnergy;

    rnahybrid.processingResult(outFile, freeEnergy);
    EXPECT_EQ(freeEnergy, 1000);   //obsolete deltaG       
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(RNAHybridBackendTestSuite2, InvalidLineDG)
{
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-rnahybrid.test";
    std::ofstream file(outFile.c_str());
    file << "target: HeadToTargetSequence\n";
    file << "length: 309\n";
    file << "miRNA : shorterSequenceName\n";
    file << "length: 22\n";
    file << "\n";
    file << "mfe:-34.3kcal/mol\n";
    file << "p-value: 0.999995\n";
    file << "\n";
    file << "position  206\n";
    file << "target 5'  C    UCAAUG      A    3'\n";
    file << "            UUCC       CCGCA\n";
    file << "            AAGG       GGUGU\n";
    file << "miRNA  3' CU    UUAAAAA     AGAA 5'\n";
    file.close();
    RNAHybrid rnahybrid;
    Fe freeEnergy;
    rnahybrid.processingResult(outFile, freeEnergy);
    EXPECT_EQ(freeEnergy, 1000);   //obsolete deltaG   
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());
}