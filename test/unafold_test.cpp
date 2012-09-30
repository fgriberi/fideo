/*
    Copyright (C) 2012 Franco Riberi, FuDePAN

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



TEST(UnaFoldBackendTestSuite, BasicTest)
{
    biopp::NucSequence sec1 ("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure str1;

    IFold* p1 = mili::FactoryRegistry<IFold, std::string>::new_class("UNAFold");
    ASSERT_TRUE(p1 != NULL);

    EXPECT_NO_THROW(p1->fold(sec1,str1,true));

    ASSERT_EQ(32, str1.size());

    ASSERT_FALSE(std::ifstream("fold.in.ct"));
    ASSERT_FALSE(std::ifstream("fold.in.dG"));
    ASSERT_FALSE(std::ifstream("fold.in.h-num"));
    ASSERT_FALSE(std::ifstream("fold.in.log"));
    ASSERT_FALSE(std::ifstream("fold.in.plot"));
    ASSERT_FALSE(std::ifstream("fold.in.run"));
    ASSERT_FALSE(std::ifstream("fold.in.ss-count"));
    ASSERT_FALSE(std::ifstream("fold.in.ann"));
    ASSERT_FALSE(std::ifstream("fold.in.det"));

    delete(p1);
}
