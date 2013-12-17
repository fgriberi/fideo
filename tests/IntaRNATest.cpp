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
#define private public

#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <etilico/etilico.h>
#include <gtest/gtest.h>
#include "HelperTest.h"
#define INTA_RNA_H
#include "fideo/IntaRNA.h"
#undef INTA_RNA_H

using namespace fideo;

TEST(IntaRNABackendTestSuite, BasicTest1)
{
    const biopp::NucSequence seq1("GGAGUGGAGUAGGGGCCGCAAUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCUUUGC");

    IHybridize* const p = Hybridize::new_class("IntaRNA");
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

    IHybridize* const p = Hybridize::new_class("IntaRNA");
    ASSERT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -7.34977);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
}

TEST(IntaRNABackendTestSuite1, InvalidBackend)
{
    IHybridize* const intarna = Hybridize::new_class("intaRNA");
    ASSERT_TRUE(intarna == NULL);
    delete intarna;
}

TEST(IntaRNABackendTestSuite2, correctCommad)
{
    const std::string seq1 = "AAAAAAAAGGGGGGGGCCCCCCCCUUUAAGGGGGGGGCCCCCCCCCUUUUUUUU";
    const std::string seq2 = "AAGAUGUGGAAAAAUUGGAAUC";
    const biopp::NucSequence longer(seq1);
    const biopp::NucSequence shorter(seq2);
    biopp::SecStructure secStructure;         
    IntaRNA intarna;
    IHybridizeIntermediate::InputFiles inFiles;
    IHybridizeIntermediate::OutputFile outFile;
    etilico::Command cmd;    
    intarna.prepareData(longer, shorter, cmd, inFiles, outFile);    
    
    EXPECT_TRUE(HelperTest::checkDirTmp());    
    std::stringstream cmdExpected;
    cmdExpected << "./IntaRNA ";
    cmdExpected << seq1;
    cmdExpected << " ";
    cmdExpected << seq2;
    cmdExpected << " > ";
    cmdExpected << outFile;
    EXPECT_EQ(cmdExpected.str(), cmd);    
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

static const size_t SECOND_SEQ_IS_MISSING = 1;
TEST(IntaRNABackendTestSuite2, incorrectCommad)
{
    const etilico::Command cmd = "./IntaRNA AAGAUGUGGAAAAAUUGGAAUC > outputFile";    
    EXPECT_EQ(etilico::runCommand(cmd), SECOND_SEQ_IS_MISSING);
}

TEST(IntaRNABackendTestSuite2, FileNotExist)
{
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-fileNotExist";      
    IntaRNA intarna;
    biopp::SecStructure secStructure;
    Fe freeEnergy;
    EXPECT_THROW(intarna.processingResult(outFile, freeEnergy), NotFoundFileException);      
    EXPECT_FALSE(HelperTest::checkDirTmp());   
}

TEST(IntaRNABackendTestSuite2, InvalidFile)
{    
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-intarna.test";
    std::ofstream file(outFile.c_str());
    file << ">target RNA \n";
    file << ">ncRNA \n";
    file << "\n";
    file << "5'-AAGGGGCAUCUGAACUCGGUAUCACGGCCGAGGAUGCAAGGCUUGUUUCAGAGAUUGCAA$ \n";
    file << "       UCAGUUCCACAUU \n";
    file << "       GGUUAAGGUGUAG\n";
    file << "3'-CUAAAAAAA-5' \n";
    file << "energy:-7.67526/mol \n";
    file.close();
    IntaRNA intarna;
    Fe freeEnergy;
    intarna.processingResult(outFile, freeEnergy);
    EXPECT_EQ(freeEnergy, 1000);   //obsolete deltaG       
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());      
}

TEST(IntaRNABackendTestSuite2, InvalidFile2)
{    
    const IHybridizeIntermediate::OutputFile outFile = "/tmp/fideo-intarna.test";
    std::ofstream file(outFile.c_str());    
    IntaRNA intarna;
    Fe freeEnergy;
    intarna.processingResult(outFile, freeEnergy);
    EXPECT_EQ(freeEnergy, 1000);   //obsolete deltaG   
    unlink(outFile.c_str());
    EXPECT_FALSE(HelperTest::checkDirTmp());      
}