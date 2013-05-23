/*
 * @file      RNAupTest.cpp
 * @brief     RNAupTest is a test file to RNAup backend.
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

#include <string>
#include <fstream>
#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <gtest/gtest.h>
#include "HelperTest.h"

using namespace fideo;

static const std::string FILE_NAME = "RNA_w25_u3_4_up.out";
TEST(RNAupBackendTestSuite, BasicTest)
{
    const biopp::NucSequence seq1("GGAGUAGGUUAUCCUCUGUU");
    const biopp::NucSequence seq2("AGGACAACCU");

    IHybridize* p = Hybridizer::new_class("RNAup");
    EXPECT_TRUE(p != NULL);

    double dG = p->hybridize(seq1, false, seq2);
    EXPECT_DOUBLE_EQ(dG, -6.66);
    delete p;

    EXPECT_FALSE(HelperTest::checkDirTmp());
    EXPECT_FALSE(std::ifstream(FILE_NAME.c_str()));
}
