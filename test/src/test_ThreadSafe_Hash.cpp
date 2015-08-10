/**
 * @Author: WangKun <wangkun207989@sogou-inc.com>
 * @Created Time : Fri 17 Jul 2015 03:18:42 PM CST
 *
 * @File Name: test_ThreadSafe_Hash.cpp
 * @brief:
*/
#include <gtest/gtest.h>
#include "ThreadSafeHash.h"

class ThreadSafeHashTest : public ::testing::Test
{
    protected:
        ThreadSafeHashTest()
        {
        }

        virtual ~ThreadSafeHashTest()
        {
        }

        static void TearDownTestCase()
        {
        }

        static void SetUpTestCase()
        {
        }

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }
};



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(ThreadSafeHashTest, test_insert)
{
    homework::ThreadSafeHash<std::string, std::string> hash;
    const std::string str("test");
    ASSERT_EQ(homework::HASH_INC_SUCC, hash.set(str, str));
    ASSERT_EQ(homework::HASH_EXIST, hash.set(str, str));
}

TEST_F(ThreadSafeHashTest, test_GET)
{
    homework::ThreadSafeHash<std::string, std::string> hash;
    const std::string str("test");
    hash.set(str, str);
    std::string value;
    ASSERT_EQ(homework::HASH_EXIST, hash.get(str, &value));
    ASSERT_EQ(str, value);
    ASSERT_EQ(homework::HASH_NOEXIST, hash.get("another"));
}
