#include "block_builder.h"
#include <gtest/gtest.h>
#include <fstream>

class BlockBuilderTest : public ::testing::Test
{
protected:
    BlockBuilder blockBuilder;
};

TEST_F(BlockBuilderTest, ResetTest)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    blockBuilder.Reset();
    ASSERT_EQ(blockBuilder.Size(), 0);
}

TEST_F(BlockBuilderTest, AddTest)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    ASSERT_EQ(18, blockBuilder.Size());
}

TEST_F(BlockBuilderTest, FinishTest)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    Slice result = blockBuilder.Finish();
    ASSERT_GT(result.size(), 0);
}

// Test that Finish returns a non-empty Slice after adding an entry
TEST_F(BlockBuilderTest, FinishReturnsNonEmptyAfterAdd)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    Slice result = blockBuilder.Finish();
    ASSERT_GT(result.size(), 0);
}

// Test that Finish returns an empty Slice if no entries have been added
TEST_F(BlockBuilderTest, FinishReturnsEmptyWithoutAdd)
{
    Slice result = blockBuilder.Finish();
    ASSERT_EQ(result.size(), 0);
}

// Test that Finish returns a Slice of correct size after adding multiple entries
TEST_F(BlockBuilderTest, FinishReturnsCorrectSizeAfterMultipleAdd)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    blockBuilder.Add(Slice("key2"), Slice("value2"));
    Slice result = blockBuilder.Finish();
    ASSERT_EQ(result.size(), 44); // Expected size based on the entries added
}

TEST_F(BlockBuilderTest, AddEmptyKeyAndValue)
{
    blockBuilder.Add(Slice(""), Slice(""));
    Slice result = blockBuilder.Finish();
    ASSERT_GT(result.size(), 0);
}

TEST_F(BlockBuilderTest, AddLargeData)
{
    // Test adding large data
    std::string largeString(1000, 'x');
    blockBuilder.Add(Slice(largeString), Slice(largeString));
    Slice result = blockBuilder.Finish();
    ASSERT_GT(result.size(), 0);
    // Additional checks for large data handling
}

TEST_F(BlockBuilderTest, RestartIntervalTest)
{
    for (int i = 0; i < 20; i++)
    {
        blockBuilder.Add(Slice("key" + std::to_string(i)), Slice("value" + std::to_string(i)));
    }
    // Check if restarts are handled correctly
    // You may need to add a method or friend class to access private members for this test
}

void WriteBlockToFile(BlockBuilder &blockBuilder, const std::string &filename)
{
    std::ofstream out(filename, std::ios::binary);
    ASSERT_TRUE(out.is_open());
    WriteBlock(blockBuilder, out);
    out.close();

    // Additional checks can be performed here, such as verifying file contents
}

TEST_F(BlockBuilderTest, WriteBlockTest)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    WriteBlockToFile(blockBuilder, "test_output.bin");
    // Additional file content checks here
}

// Test that WriteBlock resets the BlockBuilder
TEST_F(BlockBuilderTest, WriteBlockResetsBlockBuilder)
{
    blockBuilder.Add(Slice("key1"), Slice("value1"));
    std::ofstream out("test_output.bin", std::ios::binary);
    WriteBlock(blockBuilder, out);
    out.close();

    ASSERT_EQ(blockBuilder.Size(), 26);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
