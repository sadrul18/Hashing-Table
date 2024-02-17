#include <iostream>
#include <time.h>
#include "hash.h"
using namespace std;

class closedHash : public Hash
{
    Node **hashTable;
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
        hashTable = new Node *[N];
        for (long long i = 0; i < N; i++)
        {
            hashTable[i] = nullptr;
        }
    }

    ~closedHash()
    {
        Node *temp;
        for (int i = 0; i < N; i++)
        {
            temp = hashTable[i];
            while (temp != nullptr)
            {
                Node *temp2 = temp;
                temp = temp->right;
                delete temp2;
            }
        }
        delete[] hashTable;
    }

    void showHashTable()
    {
        for (int i = 0; i < N; i++)
        {
            if (hashTable[i] == nullptr)
            {
                continue;
            }
            cout << hashTable[i]->key << " " << hashTable[i]->value << endl;
        }
    }

    void insert(string key, long long value)
    {
        long long index;
        for (int i = 0; i < N; i++)
        {
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }
            if (hashTable[index] == nullptr)
            {
                hashTable[index] = new Node(key, value);
                size++;
                break;
            }
            else
            {
                continue;
            }
        }
    }

    bool remove(string key)
    {
        long long index;
        for (int i = 0; i < N; i++)
        {
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }

            if (hashTable[index] == nullptr)
            {
                return false;
            }

            else if (hashTable[index]->key == key)
            {
                Node *temp = hashTable[index];
                delete temp;
                hashTable[index] = nullptr;
                size--;
                return true;
            }
        }

        return false;
    }

    bool find(string key)
    {
        long long index;
        for (int i = 0; i < N; i++)
        {
            if (probing == 1) // probing =1 for custom probing
            {
                index = customProbing(key, i);
            }

            else if (probing == 2) // probing =2 for doubleHashing
            {
                index = douobleHashing(key, i);
            }

            if (hashTable[index] == nullptr)
            {
                return false;
            }

            else if (hashTable[index]->key == key)
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
};

string randomWordGenerator()
{
    string word;
    long long len = rand() % 6 + 5;
    for (long long i = 0; i < len; i++)
    {
        word += 'a' + rand() % 25;
    }

    return word;
}

int main()
{
    freopen("output3.txt", "w", stdout);
    srand(time(NULL));
    long long N = 10000;
    long long T = 0.1 * N;
    string words[N];
    for (long long i = 0; i < N; i++)
    {
        words[i] = randomWordGenerator();
    }

    // for (long long i = 1; i <= 2; i++)
    // {
    //     chaining *chain = new chaining(5000, i, 100);

    //     for (long long m = 0; m < N; m++)
    //     {
    //         if (chain->find(words[m]) == false)
    //         {
    //             chain->insert(words[m], chain->getSize() + 1);
    //         }
    //     }

    //     chain->resetProbes();

    //     for (long long m = 0; m < T; m++)
    //     {
    //         chain->find(words[m]);
    //     }
    //     // chain->showHashTable();
    //     cout << chain->getCollisions() << " " << (double)chain->getProbes() / T << endl;
    // }

    for (long long i = 1; i <= 2; i++) // loop for selecting probing type
    {
        for (long long j = 1; j <= 2; j++) // loop for selecting hash function
        {
            closedHash *c = new closedHash(5000, j, i);

            for (long long k = 0; k < N; k++)
            {
                if (c->find(words[k]) == false)
                {
                    c->insert(words[k], c->getSize() + 1);
                }
            }

             c->showHashTable();
        }
    }

    return 0;
}
