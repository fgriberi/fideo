#include <gmock/gmock.h>
#include <biopp/biopp.h>

#include "fideo/IFoldInverse.h"
#include "fideo/IStartProvider.h"
#include "fideo/FideoStructureParser.h"
#include "fideo/RNAFoldInverse.h"

using ::testing::Invoke;

struct StartProviderMock : public fideo::IStartProvider
{
    MOCK_METHOD1(get_partial_start, void(fideo::IFoldInverse* const));
    MOCK_METHOD1(get_complete_start, void(fideo::IFoldInverse* const));
};


class RNAinverseTest : public ::testing::Test
{
protected:
    biopp::SecStructure str;
    biopp::NucSequence s;
    void SetUp()
    {
        fideo::ViennaParser::parseStructure("(..((.....))..)", str);
    }
public:
    void fake_provider(fideo::IFoldInverse* const inv)
    {
        biopp::NucSequence s("GCACGCGTATGCCGC");
        inv->set_start(s);
    }

    void fake_bad1_provider(fideo::IFoldInverse* const inv)
    {
        biopp::NucSequence s("GCAA"); //<5
        EXPECT_THROW(inv->set_start(s), fideo::RNABackendException);
    }
    void fake_bad2_provider(fideo::IFoldInverse* const inv)
    {
        biopp::NucSequence s("GCACGCGTATG"); // >=5 && <15
        inv->set_start(s);
    }
    void fake_bad3_provider(fideo::IFoldInverse* const inv)
    {
        biopp::NucSequence s("GCACGCGTATGCCGCAAGGCCA"); // >15
        inv->set_start(s);
    }
};

TEST_F(RNAinverseTest, FoldInverse)
{
    fideo::IFoldInverse* const inverse = new fideo::RNAinverse(str, 4, 5, 10);

    StartProviderMock provider;
    EXPECT_CALL(provider, get_partial_start(inverse))
    .Times(1)
    .WillOnce(Invoke(this, &RNAinverseTest::fake_provider));


    inverse->query_start(&provider);

    const biopp::NucSequence expected("GTAGCTTTATGCCGC");

    biopp::NucSequence seq;
    inverse->fold_inverse(seq);
    for (size_t i = 0; i < str.size(); ++i)
    {
        EXPECT_EQ(seq[i], expected[i]);
    }

    delete inverse;
}

TEST_F(RNAinverseTest, BadStart1)
{
    fideo::IFoldInverse* const inverse = new fideo::RNAinverse(str, 4, 5, 10);

    StartProviderMock provider;
    EXPECT_CALL(provider, get_partial_start(inverse))
    .Times(1)
    .WillOnce(Invoke(this, &RNAinverseTest::fake_bad1_provider));


    EXPECT_THROW(inverse->query_start(&provider), fideo::RNABackendException);

    delete inverse;
}

TEST_F(RNAinverseTest, BadStart2)
{
    fideo::IFoldInverse* const inverse = new fideo::RNAinverse(str, 4, 5, 10);

    StartProviderMock provider;
    EXPECT_CALL(provider, get_partial_start(inverse))
    .Times(1)
    .WillOnce(Invoke(this, &RNAinverseTest::fake_bad2_provider));


    EXPECT_THROW(inverse->query_start(&provider), fideo::RNABackendException);

    delete inverse;
}

TEST_F(RNAinverseTest, BadStart3)
{
    fideo::IFoldInverse* const inverse = new fideo::RNAinverse(str, 4, 5, 10);

    StartProviderMock provider;
    EXPECT_CALL(provider, get_partial_start(inverse))
    .Times(1)
    .WillOnce(Invoke(this, &RNAinverseTest::fake_bad3_provider));


    EXPECT_THROW(inverse->query_start(&provider), fideo::RNABackendException);

    delete inverse;
}
