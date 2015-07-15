/**
 * @Author: WangKun <wangkun207989@sogou-inc.com>
 * @Created Time : Thu 25 Jun 2015 10:15:23 AM CST
 *
 * @File Name: test.cpp
 * @brief:
*/
#include "ThreadSafeHash.h"
#include <assert.h>

int main()
{
    homework::ThreadSafeHash<std::string> hash;
    const std::string str("test");
    assert(homework::HASH_INC_SUCC == hash.Push(str));
    assert(homework::HASH_EXIST == hash.Push(str));
    assert(homework::HASH_EXIST == hash.Get(str));
    assert(homework::HASH_NOEXIST == hash.Get("another"));
    return 0;
}
