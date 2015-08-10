/**
 * @Author: WangKun <wangkun207989@sogou-inc.com>
 * @Created Time : Thu 25 Jun 2015 10:15:23 AM CST
 *
 * @File Name: compare.cpp
 * @brief:
*/
#include "ThreadSafeHash.h"
#include <sys/times.h>
#include <gsl/Hash/PHashMap.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include <google/profiler.h>


void gen_rnd_string(std::vector<std::string>& out, int count = 200000, int max_len = 40);

template <class T, class V, template <class T, class V> class Hash >
class Thread_Fun
{
public:
    /**
     * @brief           : Thread main function that loop through a Container to lookup whether each element is in the HashMap
     *
     * @tparam T
     * @param param
     *
     * @return 
     */
    static void* Hash_Reader(void* param)
    {
        typedef Hash<T, V> HashType;
        typedef std::vector<T> ConType;
        typedef std::pair<HashType*, ConType*> ValType;
        ValType* pValue = reinterpret_cast<ValType*>(param);
        HashType* pHash = pValue->first;
        ConType* pCon= pValue->second;

        // read, ignore the get result
        for (typename ConType::iterator it = pCon->begin(); it != pCon->end(); ++it)
        {
            pHash->get(*it);
        }
    }
    /**
     * @brief 	        : Thread main function that loop through a Container to insert each element in the HashMap
     *
     * @tparam T
     * @param param
     *
     * @return 
     */
    static void* Hash_Writer(void* param)
    {
        typedef Hash<T, V> HashType;
        typedef std::vector<T> ConType;
        typedef std::pair<HashType*, ConType*> ValType;
        ValType* pValue = reinterpret_cast<ValType*>(param);
        HashType* pHash = pValue->first;
        ConType* pCon= pValue->second;

        // set, ignore the get result
        for (typename ConType::iterator it = pCon->begin(); it != pCon->end(); ++it)
        {
            pHash->set(*it, *it);
        }
    }
};

int main()
{
    // create gsl PHashMap and ThreadSafeHash
    typedef homework::ThreadSafeHash<std::string, std::string> MyHashType;
    typedef gsl::PHashMap<std::string, std::string> GSLHashType;
    MyHashType my_hash;
    GSLHashType gsl_hash;
    gsl_hash.create(homework::DEFAULT_SIZE);

    // Generate a collection of strings randomly
    std::vector<std::string> content;
    time_t start = time(NULL);
    srand(time(NULL));
    gen_rnd_string(content);
    time_t end = time(NULL);
    std::cout << "Spent " << end - start << " seconds to generate a collection of strings" << std::endl;
    std::pair<MyHashType*, std::vector<std::string>* > my_hash_data = std::make_pair(&my_hash, &content);
    std::pair<GSLHashType*, std::vector<std::string>* > gsl_hash_data = std::make_pair(&gsl_hash, &content);

    //  use multi threads to insert a gsl PHashMap
    int ret = 0;
    const int cnt_writer = 30;
    pthread_t gsl_hash_writer[cnt_writer];
    start = time(NULL);
    for (int i = 0; i < cnt_writer; ++i)
    {
        ret = pthread_create(&gsl_hash_writer[i], NULL, Thread_Fun<std::string, std::string, gsl::PHashMap>::Hash_Writer, (void*)(&gsl_hash_data));
        if (0 != ret)
        {
            fprintf(stderr, "fail to create gsl_hash_writer thread %d", i);
            return ret;
        }
    }

    for (int i = 0; i < cnt_writer; ++i)
    {
        ret = pthread_join(gsl_hash_writer[i],NULL);
        if (0 != ret)
        {
            fprintf(stderr, "fail to join gsl_hash_writer thread %d", i);
            return ret;
        }
    }
    end = time(NULL);
    std::cout << "GSL Hash spent " << end - start << " seconds to insert the Hash" << std::endl;

    //  use multi threads to insert a ThreadSafeHash
    pthread_t my_hash_writer[cnt_writer];
    start = time(NULL);
    for (int i = 0; i < cnt_writer; ++i)
    {
        ret = pthread_create(&my_hash_writer[i], NULL, Thread_Fun<std::string, std::string, homework::ThreadSafeHash>::Hash_Writer,
               reinterpret_cast<void*>(&my_hash_data));
        if (0 != ret)
        {
            fprintf(stderr, "fail to create my_hash_writer thread %d", i);
            return ret;
        }
    }

    for (int i = 0; i < cnt_writer; ++i)
    {
        ret = pthread_join(my_hash_writer[i],NULL);
        if (0 != ret)
        {
            fprintf(stderr, "fail to join my_hash_writer thread %d", i);
            return ret;
        }
    }
    end = time(NULL);
    std::cout << "My Hash spent " << end - start << " seconds to insert the Hash" << std::endl;

    // use multi threads to lookup a gsl PHashMap
    const int cnt_reader = 30;
    pthread_t gsl_hash_reader[cnt_reader];
    start = time(NULL);
    for (int i = 0; i < cnt_reader; ++i)
    {
        ret = pthread_create(&gsl_hash_reader[i], NULL, Thread_Fun<std::string, std::string, gsl::PHashMap>::Hash_Reader,
                reinterpret_cast<void*>(&gsl_hash_data));
        if (0 != ret)
        {
            fprintf(stderr, "fail to create gsl_hash_reader thread %d", i);
            return ret;
        }
    }

    for (int i = 0; i < cnt_reader; ++i)
    {
        ret = pthread_join(gsl_hash_reader[i],NULL);
        if (0 != ret)
        {
            fprintf(stderr, "fail to join gsl_hash_reader thread %d", i);
            return ret;
        }
    }
    end = time(NULL);
    std::cout << "GSL Hash spent " << end - start << " seconds to lookup the Hash" << std::endl;

    // use multi threads to lookup a ThreadSafeHashMap
    pthread_t my_hash_reader[cnt_reader];
    start = time(NULL);
    for (int i = 0; i < cnt_reader; ++i)
    {
        ret = pthread_create(&my_hash_reader[i], NULL, Thread_Fun<std::string, std::string, homework::ThreadSafeHash>::Hash_Reader,
               reinterpret_cast<void*>(&my_hash_data));
        if (0 != ret)
        {
            fprintf(stderr, "fail to create my_hash_reader thread %d", i);
            return ret;
        }
    }

    for (int i = 0; i < cnt_reader; ++i)
    {
        ret = pthread_join(my_hash_reader[i],NULL);
        if (0 != ret)
        {
            fprintf(stderr, "fail to join my_hash_reader thread %d", i);
            return ret;
        }
    }
    end = time(NULL);
    std::cout << "My Hash spent " << end - start << " seconds to lookup the Hash" << std::endl;

    return 0;
}

void gen_rnd_string(std::vector<std::string>& out, int count, int max_len)
{
    out.clear();
    if (max_len <= 0)
        return;
    const int ASCII_MAX = 128;
    std::vector<std::string>::iterator it;
    for(int i = 0; i < count; ++i)
    {
        int len = rand() % max_len;
        std::string str;
        str.reserve(len + 1);
        for (int j = 0; j < len; ++j)
        {
            str.push_back((char)(rand() % ASCII_MAX));
        }
        str.push_back('\0');
        out.push_back(str);
    }
}

