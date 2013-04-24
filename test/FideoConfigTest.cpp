/*
 * @file   FideoConfigTest.cpp
 * @brief  FideoConfigTest is a test file to FideoConfig class.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 02, 2012, 19:35 PM
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
#include <gtest/gtest.h>

TEST(FideoConfigTestSuite, Test1)
{
    const std::string INTA_RNA = "runIntaRNA";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/IntaRNA/intarna-1.2.5/src";
    std::string executablePath;
    fideo::FideoConfig::getInstance()->getPath(INTA_RNA, executablePath);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}

TEST(FideoConfigTestSuite, Test2)
{
    const std::string RUN_GENE_DESIGN = "runGD";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/geneDesign/GeneDesign/bin";

    std::string executablePath;
    fideo::FideoConfig::getInstance()->getPath(RUN_GENE_DESIGN, executablePath);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}

TEST(FideoConfigTestSuite, Test3)
{
    const std::string RESULT_GENE_DESIGN = "resultGD";
    const std::string PATH_EXECUTABLE = "/home/gringusi/Escritorio/geneDesign/GeneDesign/bin/sequence_gdRT";

    std::string executablePath;
    fideo::FideoConfig::getInstance()->getPath(RESULT_GENE_DESIGN, executablePath);

    ASSERT_EQ(executablePath, PATH_EXECUTABLE);
}