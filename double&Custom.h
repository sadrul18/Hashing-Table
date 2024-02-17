#ifndef DOUBLE_CUSTOM_H
#define DOUBLE_CUSTOM_H

#include<iostream>
#include "hash.h"
using namespace std;
class closedHash : public Hash
{
    Node *hashTable;
    long long functionFlag;
    long long probing;
    long long c1, c2;

    long long customProbing(string key, long long i)
    {
        long long index;
        if (functionFlag == 1)
        {
            index = (hashFunction1(key) + c1 * i * auxHash(key) + c2 * i * i) % N;
        }
        else if (functionFlag == 2)
        {
            index = (hashFunction2(key) + c1 * i * auxHash(key) + c2 * i * i) % N;
        }

        return index;
    }

    long long douobleHashing(string key, long long i)
    {
        long long index;
        if (functionFlag == 1)
        {
            index = (hashFunction1(key) + i * auxHash(key)) % N;
        }
        else if (functionFlag == 2)
        {
            index = (hashFunction2(key) + i * auxHash(key)) % N;
        }

        return index;
    }

public:
    closedHash(long long N, long long hashFunction, long long probing) : Hash(nextPrime(N))
    {
        N = nextPrime(N);
        functionFlag = hashFunction;
        this->probing = probing;
        c1 = 37;
        c2 = 41;
        hashTable = new Node[N];
    }

    ~closedHash()
    {
        delete[] hashTable;
    }

    void insert(string key, long long value)
    {
        if ((size / N > 0.2) || (size > 100 && probes / (0.1 * N) > 2))
        {
            reHash(1);
        }
        long long index, prev = 0;
        for (long long i = 0; i < N; i++)
        {
            prev = index;
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }

            if (index == prev)
            {
                continue;
            }

            if (hashTable[index].key == "")
            {
                hashTable[index] = Node(key, value);
                size++;
                break;
            }

            else if (hashTable[index].key != "")
            {
                collisions++;
            }
        } 
    }

    bool remove(string key)
    {
        long long index;
        for (long long i = 0; i < N; i++)
        {
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }

            if (hashTable[index].key == "")
            {
                break;
            }

            else if (hashTable[index].key == key)
            {
                hashTable[index] = Node("", -1);
                size--;
                return true;
            }
        }

        return false;
    }

    bool find(string key)
    {
        long long index;
        for (long long i = 0; i < N; i++)
        {
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }

            if (hashTable[index].key == "")
            {
                break;
            }

            else if (hashTable[index].key == key)
            {
                probes++;
                return true;
            }

            else
            {
                probes++;
            }
        }

        return false;
    }

    void reHash(long long n)
    {
        long long oldN = N;
        if (n == 1)
        {
            N = nextPrime(N * 2);
        }
        else if (n == 2)
        {
            nextPrime(N / 2);
        }
        Node *oldHashTable = hashTable;
        hashTable = new Node[N];
        size = 0;

        for (long long i = 0; i < oldN; i++)
        {
            if (oldHashTable[i].key != "")
                insert(oldHashTable[i].key, oldHashTable[i].value);
        }
        delete[] oldHashTable;
    }
};

#endif