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
#include <list>
#include <new>
#include <stdexcept>

namespace homework
{
const int DEFAULT_SIZE = 49999;

enum
{
    HASH_EXIST=1,
    HASH_NOEXIST,
    HASH_OVERWRITE,
    HASH_INC_SUCC,
};

template<class T, class V>
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
    int set(const T& key, const V& value)
    {
        int ret = HASH_INC_SUCC;
        pthread_rwlock_wrlock(&_rwlock); 
        typename ConType::iterator it = _vec.begin();
        for (; it != _vec.end(); ++it)
        {
            if (key == it->first)
                break;
        }
        if (it == _vec.end())
        {
            _vec.push_back(std::make_pair(key, value));
        }
        else
        {
            it->second = value;
            ret = HASH_EXIST;
        }
        pthread_rwlock_unlock(&_rwlock); 
        return ret;
    }
    int get(const T& key, V* value = NULL) 
    {
        int ret = HASH_NOEXIST;
        pthread_rwlock_rdlock(&_rwlock);
        typename ConType::iterator it = _vec.begin();
        for (; it != _vec.end(); ++it)
        {
            if (key == it->first)
                break;
        }
        if (it != _vec.end())
        {
            if (NULL != value)
                *value = it->second;
            ret = HASH_EXIST; 
        }
        pthread_rwlock_unlock(&_rwlock);
        return ret;
    }
private:
    pthread_rwlock_t _rwlock;
    typedef std::list<std::pair<T, V> > ConType;
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
        unsigned int hash = 0;
        const char *str = value.c_str();
        while (*str)
        {
            hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        }
        return (hash & 0x7FFFFFFF);
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

template<class T, class V, size_t num =  DEFAULT_SIZE, class HashFun=Hasher<T>, class Container=LockContainer<T, V>, 
    class Allocator=New<LockContainer<T, V> >, class DeAllocator=Del<LockContainer<T, V> > >
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
    int set(const T& key, const V& value)
    {
        unsigned int index = _hasher.hash(key) % num;
        if (NULL == _pContainer)
            return -1;
        return _pContainer[index].set(key, value);
    }
    int get(const T& key, V* value = NULL)
    {
        unsigned int index = _hasher.hash(key) % num;
        if (NULL == _pContainer)
            return -1;
        return _pContainer[index].get(key, value);
    }
private:
    HashFun _hasher;
    Allocator _allocator;
    DeAllocator _dallocator;
    Container* _pContainer;

    void Destory()
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
