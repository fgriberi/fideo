/*
    Copyright (C) 2013 Franco Riberi, FuDePAN

    This file is part of the Fideo Library.

    Fideo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fideo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fideo.  If not, see <http://www.gnu.org/licenses/>.

    This is a test file.
*/
#include <string>
#include <fstream>
#include <fideo/fideo.h>
#include <biopp/biopp.h>
#include <mili/mili.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(RNAFoldBackendTestSuite, BasicTest)
{
    const biopp::NucSequence seq("AATTAAAAAAGGGGGGGTTGCAACCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* p = mili::FactoryRegistry<IFold, std::string>::new_class("RNAFold");
    ASSERT_TRUE(p != NULL);

    Fe result = p->fold(seq, secStructure, true);
    delete p;

    EXPECT_DOUBLE_EQ(result, -18.70);
    EXPECT_TRUE(secStructure.is_circular());

    const std::string prefixTmpFile = TmpFile::getTmpName();

    EXPECT_FALSE(std::ifstream(prefixTmpFile.c_str()));
    EXPECT_FALSE(std::ifstream((prefixTmpFile + ".out").c_str()));
}
