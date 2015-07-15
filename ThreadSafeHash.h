/**
 * @Author: WangKun <wangkun207989@sogou-inc.com>
 * @Created Time : Tue 23 Jun 2015 07:03:32 PM CST
 *
 * @File Name: ThreadSafeHash.h
 * @brief:
*/
#ifndef _THREAD_SAFE_HASH_
#define _THREAD_SAFE_HASH_

#include <string>
#include <pthread.h>
#include <vector>
#include <list>
#include <new>
#include <stdexcept>

namespace homework
{
static const int DEFAULT_SIZE = 100000;

enum
{
    HASH_EXIST=1,
    HASH_NOEXIST,
    HASH_OVERWRITE,
    HASH_INC_SUCC,
};

template<class T>
class LockContainer
{
public:
    LockContainer()
    {
        if (0 != pthread_rwlock_init(&_rwlock, NULL))
        {
            throw std::runtime_error("can't init rwlock");
        }
    }
    ~LockContainer()
    {
        pthread_rwlock_destroy(&_rwlock);
    }
    int Push(const T& value)
    {
        int ret = HASH_INC_SUCC;
        pthread_rwlock_wrlock(&_rwlock); 
        typename ConType::iterator it = _vec.begin();
        for (; it != _vec.end(); ++it)
        {
            if (value == *it)
                break;
        }
        if (it == _vec.end())
            _vec.push_back(value);
        else
            ret = HASH_EXIST;
        pthread_rwlock_unlock(&_rwlock); 
        return ret;
    }
    int Get(const T& value) 
    {
        int ret = HASH_NOEXIST;
        pthread_rwlock_rdlock(&_rwlock);
        typename ConType::iterator it = _vec.begin();
        for (; it != _vec.end(); ++it)
        {
            if (value == *it)
                break;
        }
        if (it != _vec.end())
            ret = HASH_EXIST; 
        pthread_rwlock_unlock(&_rwlock);
        return ret;
    }
private:
    pthread_rwlock_t _rwlock;
    typedef std::list<T> ConType;
    ConType _vec;
};

template<class T>
class Hasher
{
public:
    unsigned int hash(const T& value);
};

template<>
class Hasher<std::string>
{
public:
    unsigned int hash(const std::string& value)
    {
        const char* _s = value.c_str();
        if (*_s) return 0;
        unsigned long n = 0;
        for (; *_s; ++_s)
        {
            n = n * 5 + *_s;
        }
        return static_cast<unsigned int>(n);
    }
};

template<>
class Hasher<long long>
{
public:
    unsigned int hash(const long long& value)
    {
        return static_cast<unsigned int>(value);
    }
};

template<class T>
class New
{
public:
    T* create(int size)
    {
        return new(std::nothrow) T[size];
    }
};

template<class T>
class Del
{
public:
    void clear(T* pointer)
    {
        delete[] pointer;
    }
};

template<class T, size_t num =  DEFAULT_SIZE, class HashFun=Hasher<T>, class Container=LockContainer<T>, 
    class Allocator=New<LockContainer<T> >, class DeAllocator=Del<LockContainer<T> > >
class ThreadSafeHash
{
public:
    ThreadSafeHash() : _pContainer(NULL)
    {
        if (!Create())
        {
            throw std::bad_alloc();
        }
    }
    ~ThreadSafeHash()
    {
        Destory();
    }
    int Push(const T& value)
    {
        unsigned int index = _hasher.hash(value) % num;
        if (NULL == _pContainer)
            return -1;
        return _pContainer[index].Push(value);
    }
    int Get(const T& value)
    {
        unsigned int index = _hasher.hash(value) % num;
        if (NULL == _pContainer)
            return -1;
        return _pContainer[index].Get(value);
    }
private:
    HashFun _hasher;
    Allocator _allocator;
    DeAllocator _dallocator;
    Container* _pContainer;

    int Destory()
    {
        _dallocator.clear(_pContainer);
        _pContainer = NULL;
    }

    int Create()
    {
        _pContainer = _allocator.create(num);
        if (NULL == _pContainer)
        {
            return 0;
        }
        return 1;
    }
};
}
#endif
