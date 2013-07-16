#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "fideo/FideoStructureParser.h"
#include "fideo/IStructureCmp.h"

TEST(RNAForesterTest, Compare)
{
    fideo::IStructureCmp* forester = fideo::IStructureCmp::Factory::new_class("RNAForester");
    biopp::SecStructure s1;
    biopp::SecStructure s2;

    fideo::ViennaParser::parseStructure("((....))", s1);
	fideo::ViennaParser::parseStructure("(...)...", s2);

    EXPECT_EQ(forester->compare(s1, s2), -1.4f);
    delete forester;
}