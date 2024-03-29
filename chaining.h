#ifndef CHAINING_H
#define CHAINING_H

#include <iostream>
#include <time.h>
#include "hash.h"
class chaining : public Hash
{
    Node **hashTable;
    long long functionFlag;
    long long C;
    long long insertCount;
    long long deleteCount;
    long long initialN;

public:
    chaining(long long N, long long functionFlag, long long C) : Hash(nextPrime(N))
    {
        this->C = C;
        insertCount = 0;
        deleteCount = 0;
        N = nextPrime(N);
        initialN = N;
        this->functionFlag = functionFlag;
        hashTable = new Node *[N];
        for (long long i = 0; i < N; i++)
        {
            hashTable[i] = nullptr;
        }
        // cout<<"Constructor called"<<endl;
    }

    ~chaining()
    {
        for (long long i = 0; i < N; i++)
        {
            Node *temp = hashTable[i];
            while (temp != nullptr)
            {
                Node *temp2 = temp;
                temp = temp->right;
                delete temp2;
            }
        }
        delete[] hashTable;
    }

    // show hashTable for debug perpose
    void showHashTable()
    {
        for (long long i = 0; i < N; i++)
        {
            Node *temp = hashTable[i];
            if (temp == nullptr)
            {
                continue;
            }
            else
            {
                cout << temp->value << " :";
                while (temp != nullptr)
                {
                    cout << temp->key << " ";
                    temp = temp->right;
                }
                cout << endl;
            }
        }
    }

    void reHash(char ch)
    {
        cout<<"Load-factor: "<<(double)size/N<<endl;
        cout<<"Average probe count: "<<(double)probes/size<<endl;
        cout<<"Max chain length before rehashing: "<<maxChainLen()<<endl;
        long long oldN = N;
        if (ch == 'D') // D for doubling the size
        {
            N = nextPrime(2 * N);
        }
        else if (ch == 'H') // H for the half of the size
        {
            N = nextPrime(N / 2);
        }
        Node **tempHashTable = new Node *[oldN];
        tempHashTable = hashTable;
        hashTable = new Node *[N];
        for (long long i = 0; i < N; i++)
        {
            hashTable[i] = nullptr;
        }

        Node *temp;
        for (long long i = 0; i < oldN; i++)
        {
            temp = tempHashTable[i];
            while (temp != nullptr)
            {
                insert(temp->key, temp->value);
                temp = temp->right;
            }
        }

        for (long long i = 0; i < oldN; i++)
        {
            Node *temp2 = tempHashTable[i];
            while (temp2 != nullptr)
            {
                Node *temp3 = temp2;
                temp2 = temp2->right;
                delete temp3;
            }
        }
        delete[] tempHashTable;
        cout<<"Max chain length after rehashing: "<<maxChainLen()<<endl<<endl;

    }

    void insert(string key, long long value)
    {
        // condition should be checked later if reHashing should be done or not
        if (insertCount >= 100)
        {
            insertCount = 0;
            if (maxChainLen() > C)
            {
                reHash('D'); // D for the indication of doubling the size
            }
        }
        long long index;
        if (functionFlag == 1)
        {
            index = hashFunction1(key);
        }
        else
        {
            index = hashFunction2(key);
        }

        if (hashTable[index] == nullptr)
        {
            Node *newNode = new Node(key, value);
            hashTable[index] = newNode;
        }
        else
        {
            collisions++;
            Node *temp = hashTable[index];
            while (temp->right != nullptr)
            {
                temp = temp->right;
            }
            
            temp->right = new Node(key, value);
        }
        size++;
        insertCount++;
    }

    long long maxChainLen()
    {
        long long chainLen = 0;
        long long count;
        Node *temp;
        for (long long i = 0; i < N; i++)
        {
            count = 0;
            temp = hashTable[i];
            while (temp != nullptr)
            {
                temp = temp->right;
                count++;
            }
            chainLen = max(chainLen, count);
        }

        return chainLen;
    }

    bool find(string key)
    {
        //cout << "Find called" << endl;
        if (size == 0)
        {
            return false;
        }
        long long index;
        if (functionFlag == 1)
        {
            index = hashFunction1(key);
        }
        else
        {
            index = hashFunction2(key);
        }

        if (hashTable[index] == nullptr)
        {
            return false;
        }
        else
        {
            Node *temp = hashTable[index];
            probes++;
            while (temp != nullptr)
            {
                if (temp->key == key)
                {
                    return true;
                }
                temp = temp->right;
                probes++;
            }
            return false;
        }
    }

    bool remove(string key)
    {
        // give a check whether rehashing is a must or not
        if (deleteCount >= 100)
        {
            deleteCount = 0;
            if (maxChainLen() < 0.8 * C && !nextPrime(N / 2) < initialN)
            {
                reHash('H'); // H for half the size
            }
        }
        long long index;
        if (functionFlag == 1)
        {
            index = hashFunction1(key);
        }
        else
        {
            index = hashFunction2(key);
        }

        if (hashTable[index] == nullptr)
        {
            return false;
        }

        else
        {
            Node *temp = hashTable[index];
            if (temp->key == key)
            {
                delete temp;
                hashTable[index] = nullptr;
                size--;
                deleteCount++;
                return true;
            }
            while (temp->right->key != key)
            {
                temp = temp->right;
            }
            Node *temp2 = temp->right;
            temp->right = temp->right->right;
            delete temp2;
            size--;
            deleteCount++;
            return true;
        }
    }
};

#endif