/*
 * @file   RNAFoldTest.cpp
 * @brief  RNAFoldTest is a test file to RNAFold backend.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
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

#include <string>
#include <fstream>
#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include <gtest/gtest.h>
#include "HelperTest.h"

TEST(RNAFoldBackendTestSuite, BasicTest)
{
    const biopp::NucSequence seq("AATTAAAAAAGGGGGGGTTGCAACCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    fideo::IFold* p = mili::FactoryRegistry<fideo::IFold, std::string>::new_class("RNAFold");
    ASSERT_TRUE(p != NULL);

    fideo::Fe result = p->fold(seq, true, secStructure);
    delete p;

    EXPECT_DOUBLE_EQ(result, -18.70);
    EXPECT_TRUE(secStructure.is_circular());

    EXPECT_FALSE(fideo::HelperTest::checkDirTmp());
}
