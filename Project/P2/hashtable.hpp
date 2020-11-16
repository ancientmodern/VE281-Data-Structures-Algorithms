#include "hash_prime.hpp"

#include <exception>
#include <stdexcept>
#include <functional>
#include <vector>
#include <forward_list>
#include <cmath>
#include <algorithm>

/**
 * The Hashtable class
 * The time complexity of functions are based on n and k
 * n is the size of the hashtable
 * k is the length of Key
 * @tparam Key          key type
 * @tparam Value        data type
 * @tparam Hash         function object, return the hash value of a key
 * @tparam KeyEqual     function object, return whether two keys are the same
 */
template <
    typename Key, typename Value,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>>
class HashTable
{
public:
    typedef std::pair<const Key, Value> HashNode;
    typedef std::forward_list<HashNode> HashNodeList;
    typedef std::vector<HashNodeList> HashTableData;

    /**
     * A single directional iterator for the hashtable
     * ! DO NOT NEED TO MODIFY THIS !
     */
    class Iterator
    {
    private:
        typedef typename HashTableData::iterator VectorIterator;
        typedef typename HashNodeList::iterator ListIterator;

        const HashTable *hashTable;
        VectorIterator bucketIt;   // an iterator of the buckets
        ListIterator listItBefore; // a before iterator of the list, here we use "before" for quick erase and insert
        bool endFlag = false;      // whether it is an end iterator

        /**
         * Increment the iterator
         * Time complexity: Amortized O(1)
         */
        void increment()
        {
            if (bucketIt == hashTable->buckets.end())
            {
                endFlag = true;
                return;
            }
            auto newListItBefore = listItBefore;
            ++newListItBefore;
            if (newListItBefore != bucketIt->end())
            {
                if (++newListItBefore != bucketIt->end())
                {
                    // use the next element in the current forward_list
                    ++listItBefore;
                    return;
                }
            }
            while (++bucketIt != hashTable->buckets.end())
            {
                if (!bucketIt->empty())
                {
                    // use the first element in a new forward_list
                    listItBefore = bucketIt->before_begin();
                    return;
                }
            }
            endFlag = true;
        }

        explicit Iterator(HashTable *hashTable) : hashTable(hashTable)
        {
            bucketIt = hashTable->buckets.begin();
            listItBefore = bucketIt->before_begin();
            endFlag = bucketIt == hashTable->buckets.end();
        }

        Iterator(HashTable *hashTable, VectorIterator vectorIt, ListIterator listItBefore) : hashTable(hashTable), bucketIt(vectorIt), listItBefore(listItBefore)
        {
            endFlag = bucketIt == hashTable->buckets.end();
        }

    public:
        friend class HashTable;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++()
        {
            increment();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            increment();
            return temp;
        }

        bool operator==(const Iterator &that) const
        {
            if (endFlag && that.endFlag)
                return true;
            if (bucketIt != that.bucketIt)
                return false;
            return listItBefore == that.listItBefore;
        }

        bool operator!=(const Iterator &that) const
        {
            if (endFlag && that.endFlag)
                return false;
            if (bucketIt != that.bucketIt)
                return true;
            return listItBefore != that.listItBefore;
        }

        HashNode *operator->()
        {
            auto listIt = listItBefore;
            ++listIt;
            return &(*listIt);
        }

        HashNode &operator*()
        {
            auto listIt = listItBefore;
            ++listIt;
            return *listIt;
        }
    };

protected:                                                                 // DO NOT USE private HERE!
    static constexpr double DEFAULT_LOAD_FACTOR = 0.5;                     // default maximum load factor is 0.5
    static constexpr size_t DEFAULT_BUCKET_SIZE = HashPrime::g_a_sizes[0]; // default number of buckets is 5

    HashTableData buckets;                          // buckets, of singly linked lists
    typename HashTableData::iterator firstBucketIt; // help get begin iterator in O(1) time

    size_t tableSize;     // number of elements
    double maxLoadFactor; // maximum load factor
    Hash hash;            // hash function instance
    KeyEqual keyEqual;    // key equal function instance

    /**
     * Time Complexity: O(k)
     * @param key
     * @param bucketSize
     * @return the hash value of key with a new bucket size
     */
    inline size_t hashKey(const Key &key, size_t bucketSize) const
    {
        return hash(key) % bucketSize;
    }

    /**
     * Time Complexity: O(k)
     * @param key
     * @return the hash value of key with current bucket size
     */
    inline size_t hashKey(const Key &key) const
    {
        return hash(key) % buckets.size();
    }

    /**
     * Find the minimum bucket size for the hashtable
     * The minimum bucket size must satisfy all of the following requirements:
     * - It is not less than (i.e. greater or equal to) the parameter bucketSize
     * - It is greater than floor(tableSize / maxLoadFactor)
     * - It is a (prime) number defined in HashPrime (hash_prime.hpp)
     * - It is minimum if satisfying all other requirements
     * Time Complexity: O(1)
     * @throw std::range_error if no such bucket size can be found
     * @param bucketSize lower bound of the new number of buckets
     */
    size_t findMinimumBucketSize(size_t bucketSize) const
    {
        // TODO: implement this function
        if ((double)tableSize / (double)buckets.size() >= maxLoadFactor)
        {
            const size_t *first = HashPrime::g_a_sizes;
            const size_t *last = HashPrime::g_a_sizes + HashPrime::num_distinct_sizes;
            const size_t *pos = std::lower_bound(first, last, floor((double)tableSize / maxLoadFactor));
            if (pos == last)
            {
                throw std::range_error("range error");
            }
            else
            {
                return *pos;
            }
        }
        return bucketSize;
    }

    // TODO: define your helper functions here if necessary
    void copy_from(const HashTable &ht)
    {
        buckets.clear();
        buckets.resize(ht.buckets.size());
        for (size_t i = 0; i < ht.buckets.size(); ++i)
        {
            buckets[i] = ht.buckets[i];
        }
        tableSize = ht.tableSize;
        maxLoadFactor = ht.maxLoadFactor;
        hash = ht.hash;
        keyEqual = ht.keyEqual;
        firstBucketIt = ht.firstBucketIt;
    }

public:
    HashTable() : buckets(DEFAULT_BUCKET_SIZE), tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
                  hash(Hash()), keyEqual(KeyEqual())
    {
        firstBucketIt = buckets.end();
    }

    explicit HashTable(size_t bucketSize) : tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
                                            hash(Hash()), keyEqual(KeyEqual())
    {
        bucketSize = findMinimumBucketSize(bucketSize);
        buckets.resize(bucketSize);
        firstBucketIt = buckets.end();
    }

    HashTable(const HashTable &that)
    {
        // TODO: implement this function
        if (&that != this)
        {
            copy_from(that);
        }
    }

    HashTable &operator=(const HashTable &that)
    {
        // TODO: implement this function
        if (&that != this)
        {
            copy_from(that);
        }
        return *this;
    };

    ~HashTable() = default;

    Iterator begin()
    {
        if (firstBucketIt != buckets.end())
        {
            return Iterator(this, firstBucketIt, firstBucketIt->before_begin());
        }
        return end();
    }

    Iterator end()
    {
        return Iterator(this, buckets.end(), buckets.begin()->before_begin());
    }

    /**
     * Find whether the key exists in the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists in the hashtable
     */
    bool contains(const Key &key)
    {
        return find(key) != end();
    }

    /**
     * Find the value in hashtable by key
     * If the key exists, iterator points to the corresponding value, and it.endFlag = false
     * Otherwise, iterator points to the place that the key were to be inserted, and it.endFlag = true
     * Time Complexity: Amortized O(k)
     * @param key
     * @return a pair (success, iterator of the value)
     */
    Iterator find(const Key &key)
    {
        // TODO: implement this function
        size_t n = hashKey(key);
        auto ptr = buckets[n].before_begin();
        for (auto ptr_front = buckets[n].begin(); ptr_front != buckets[n].end(); ++ptr_front, ++ptr)
        {
            if ((*ptr_front).first == key)
            {
                Iterator it(this, buckets.begin() + n, ptr);
                it.endFlag = false;
                return it;
            }
        }
        Iterator it(this, buckets.begin() + n, (buckets.begin() + n)->before_begin());
        it.endFlag = true;
        return it;
    }

    /**
     * Insert value into the hashtable according to an iterator returned by find
     * the function can be only be called if no other write actions are done to the hashtable after the find
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: O(k)
     * @param it an iterator returned by find
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(Iterator &it, const Key &key, const Value &value) // TODO: should be const Iterator &it
    {
        // TODO: implement this function
        if (!it.endFlag)
        {
            it->second = value;
            return false;
        }
        ++tableSize;
        HashNode p = {key, value};
        it.bucketIt->emplace_after(it.listItBefore, p);
        firstBucketIt = min(firstBucketIt, it.bucketIt);
        it.endFlag = false;
        if ((double)tableSize / (double)buckets.size() >= maxLoadFactor)
        {
            rehash(buckets.size());
            it = find(key);
        }
        return true;
    }

    /**
     * Insert <key, value> into the hashtable
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Key &key, const Value &value)
    {
        // TODO: implement this function
        auto it = find(key);
        return insert(it, key, value);
    }

    /**
     * Erase the key if it exists in the hashtable, otherwise, do nothing
     * DO NOT rehash in this function
     * firstBucketIt should be updated
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists
     */
    bool erase(const Key &key)
    {
        // TODO: implement this function
        auto it = find(key);
        if (it.endFlag)
        {
            return false;
        }
        else
        {
            erase(it);
            return true;
        }
    }

    /**
     * Erase the key at the input iterator
     * If the input iterator is the end iterator, do nothing and return the input iterator directly
     * firstBucketIt should be updated
     * Time Complexity: O(1)
     * @param it
     * @return the iterator after the input iterator before the erase
     */
    Iterator erase(const Iterator &it) // TODO: should be const Iterator &it
    {
        // TODO: implement this function
        if (it.endFlag)
        {
            return it;
        }
        else
        {
            it.bucketIt->erase_after(it.listItBefore);
            --tableSize;
            if (firstBucketIt >= it.bucketIt)
            {
                firstBucketIt = buckets.end();
                for (auto my_it = it.bucketIt; my_it != buckets.end(); ++my_it)
                {
                    if (!(*my_it).empty())
                    {
                        firstBucketIt = my_it;
                        break;
                    }
                }
            }
            return it;
        }
    }

    /**
     * Get the reference of value by key in the hashtable
     * If the key doesn't exist, create it first (use default constructor of Value)
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return reference of value
     */
    Value &operator[](const Key &key)
    {
        // TODO: implement this function
        auto it = find(key);
        if (it.endFlag)
        {
            insert(it, key, Value());
        }
        return it->second;
    }

    /**
     * Rehash the hashtable according to the (hinted) number of buckets
     * The bucket size after rehash need not be same as the parameter bucketSize
     * Instead, findMinimumBucketSize is called to get the correct number
     * firstBucketIt should be updated
     * Do nothing if the bucketSize doesn't change
     * Time Complexity: O(nk)
     * @param bucketSize lower bound of the new number of buckets
     */
    void rehash(size_t bucketSize)
    {
        bucketSize = findMinimumBucketSize(bucketSize);
        if (bucketSize == buckets.size())
            return;

        // TODO: implement this function
        HashTable big_table(bucketSize);
        auto temp_size = tableSize;
        for (auto &y : buckets)
        {
            for (auto &[key, value] : y)
            {
                big_table.insert(key, value);
            }
        }
        copy_from(big_table);
        tableSize = temp_size;
    }

    /**
     * @return the number of elements in the hashtable
     */
    size_t size() const { return tableSize; }

    /**
     * @return the number of buckets in the hashtable
     */
    size_t bucketSize() const { return buckets.size(); }

    /**
     * @return the current load factor of the hashtable
     */
    double loadFactor() const { return (double)tableSize / (double)buckets.size(); }

    /**
     * @return the maximum load factor of the hashtable
     */
    double getMaxLoadFactor() const { return maxLoadFactor; }

    /**
     * Set the max load factor
     * @throw std::range_error if the load factor is too small
     * @param loadFactor
     */
    void setMaxLoadFactor(double loadFactor)
    {
        if (loadFactor <= 1e-9)
        {
            throw std::range_error("invalid load factor!");
        }
        maxLoadFactor = loadFactor;
        rehash(buckets.size());
    }
};
