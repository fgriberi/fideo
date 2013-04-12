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
#include <fideo/fideo.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(FideoConfigTestSuite, Test1)
{
    const std::string INTA_RNA = "runIntaRNA";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/IntaRNA/intarna-1.2.5/src";

    const std::string executablePath =  FideoConfig::getInstance()->getPath(INTA_RNA);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}

TEST(FideoConfigTestSuite, Test2)
{
    const std::string RUN_GENE_DESIGN = "runGD";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/geneDesign/GeneDesign/bin";

    const std::string executablePath =  FideoConfig::getInstance()->getPath(RUN_GENE_DESIGN);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}

TEST(FideoConfigTestSuite, Test3)
{
    const std::string RESULT_GENE_DESIGN = "resultGD";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/geneDesign/GeneDesign/bin/sequence_gdRT";

    const std::string executablePath =  FideoConfig::getInstance()->getPath(RESULT_GENE_DESIGN);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}
