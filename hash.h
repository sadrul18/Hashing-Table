#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <time.h>

using namespace std;
bool isPrime(long long num);
long long nextPrime(long long num);

class Hash
{
protected:
    long long N, size, collisions, probes;
    class Node
    {

    public:
        string key;
        long long value;
        Node *right;
        Node(string k = "", long long val = -1, Node *r = nullptr)
        {
            key = k;
            value = val;
            right = r;
        }

        void setRight(Node *r)
        {
            right = r;
        }
    };

    long long hashFunction1(string k)
    {
        char *key = (char *)k.c_str();
        unsigned int *lkey = (unsigned int *)key;
        long long intlength = k.size() / 4;
        unsigned int sum = 0;

        for (long long i = 0; i < intlength; i++)
            sum += lkey[i];

        long long extra = k.size() - intlength * 4;
        char temp[4];
        lkey = (unsigned int *)temp;
        lkey[0] = 0;

        for (long long i = 0; i < extra; i++)
            temp[i] = key[intlength * 4 + i];

        sum += lkey[0];

        return sum % N;
    }

    long long hashFunction2(string key)
    {
        long long hashVal = 0;

        for (long long i = 0; i < key.length(); i++)
            hashVal = 37 * hashVal + key[i];

        hashVal %= N;

        if (hashVal < 0)
            hashVal += N;

        return hashVal;
    }

    long long auxHash(string key)
    {
        long long i, sum;
        for (i = 0, sum = 0; i < key.length(); i++)
            sum += key[i];
        return sum % N;
    }

public:
    Hash(long long N)
    {
        this->N = N;
        size = 0;
        collisions = 0;
        probes = 1;
    }

    void resetCollisions()
    {
        this->collisions = 0;
    }

    void resetProbes()
    {
        this->probes = 1;
    }

    long long getCollisions()
    {
        return collisions;
    }

    long long getProbes()
    {
        return probes;
    }

    long long getSize()
    {
        return size;
    }

    virtual void insert(string key, long long value) = 0;
    virtual bool remove(string key) = 0;
    virtual bool find(string key) = 0;
};

#endif