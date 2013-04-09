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

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

bool isMyTmpFile (const std::string& fileTmpName)  
{
	bool ret = false;
	stringstream ss(fileTmpName);
    vector<string> result;  //typedef vector<string> Result
    ss >> mili::Separator(result, '-');
    if (result.size() == 2 && result[0] == "myTmpFile")
		ret = true;
    return ret; 
}

static const std::string DIRECTORY_PATH = "/tmp";

bool existFiles()
{
	bool ret = false;
	DIR *dir;
	struct dirent *ent;
	dir = opendir(DIRECTORY_PATH.c_str());
	if (dir == NULL) 
  		std::cout << "Can not open directory " << DIRECTORY_PATH << std::endl; 
	while ((ent = readdir (dir)) != NULL && !ret) 
	{
		ret = isMyTmpFile(ent->d_name);
	}
	closedir (dir);
	return ret;
}

TEST(UnaFoldBackendTestSuite, BasicTest)
{
    const biopp::NucSequence seq("AAAAAAAAGGGGGGGGCCCCCCCCTTTTTTTT");
    biopp::SecStructure secStructure;

    IFold* p = mili::FactoryRegistry<IFold, std::string>::new_class("UNAFold");
    ASSERT_TRUE(p != NULL);

    EXPECT_NO_THROW(p->fold(seq, secStructure, true));
    delete p;

    EXPECT_EQ(32, secStructure.size());
    EXPECT_TRUE(secStructure.is_circular());

	EXPECT_FALSE(existFiles());
}
