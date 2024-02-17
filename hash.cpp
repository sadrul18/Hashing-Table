#include <iostream>
#include <time.h>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "hash.h"
using namespace std;

class closedHash : public Hash
{
    Node *hashTable;
    long long functionFlag;
    long long probing;
    long long c1, c2;
    long long insertCount;

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
        insertCount = 0;
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

    // void showHashTable()
    // {
    //     for (int i = 0; i < N; i++)
    //     {
    //         if (hashTable[i] == nullptr)
    //         {
    //             continue;
    //         }
    //         cout << hashTable[i]->key << " " << hashTable[i]->value << endl;
    //     }
    // }

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
                insertCount++;
                break;
            }

            else if (hashTable[index].key != "")
            {
                collisions++;
            }
        }

        // long long i = 0, h, prev, offset = 0;

        // if (probing == 1)
        // {
        //     h = customProbing(key, i);
        // }
        // else if (probing == 2)
        // {
        //     h = douobleHashing(key, i);
        // }

        // while (hashTable[h].key != "" && i < N)
        // {
        //     prev = h;

        //     if (probing == 1)
        //     {
        //         h = (offset + customProbing(key, ++i)) % N;
        //     }

        //     else if (probing == 2)
        //     {
        //         h = (offset + douobleHashing(key, ++i)) % N;
        //     }

        //     if (h == prev)
        //     {
        //         offset++;
        //         i = 0;
        //     }

        //     collisions++;
        // }

        // hashTable[h] = Node(key, value);

        // size++;
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
    //freopen("output3.txt", "w", stdout);
    srand(time(NULL));
    long long N = 10000;
    long long T = 0.1 * N;
    string words[N];
    long long size[3] = {5000, 10000, 20000};
    double report [3][3][2][2];

    for (long long i = 0; i < N; i++)
    {
        words[i] = randomWordGenerator();
    }

    long long n=0;

    for (long long s = 0; s < 3; s++)
    {
        for (long long i = 1; i <= 2; i++)
        {
            chaining *chain = new chaining(size[s], i, 100);

            for (long long m = 0; m < N; m++)
            {
                if (chain->find(words[m]) == false)
                {
                    chain->insert(words[m], chain->getSize() + 1);
                }
            }

            chain->resetProbes();

            for (long long m = 0; m < T; m++)
            {
                chain->find(words[m]);
            }
            // chain->showHashTable();
            // cout << chain->getCollisions() << " " << (double)chain->getProbes() / T << endl;
            report[n][0][i - 1][0] = chain->getCollisions();    
            report[n][0][i - 1][1] = (double) chain->getProbes() / T;
            delete chain;
        }

        // cout << "Collission for custom probing and double hashing" << endl;

        for (long long i = 1; i <= 2; i++) // loop for selecting probing type
        {
            for (long long j = 1; j <= 2; j++) // loop for selecting hash function
            {
                closedHash *c = new closedHash(size[s], j, i);

                for (long long k = 0; k < N; k++)
                {
                    if (c->find(words[k]) == false)
                    {
                        c->insert(words[k], c->getSize() + 1);
                    }
                }

                // c->showHashTable();
                c->resetProbes();
                for (long long k = 0; k < T; k++)
                {
                    c->find(words[k]);
                }


                report[n][j][i - 1][0] = c->getCollisions();    
                report[n][j][i - 1][1] = (double) c->getProbes() / T;

                // cout << c->getCollisions() << " " << (double)c->getProbes() / T << endl;
                delete c;
            }
        }
        n++;
    }

    ofstream file("output.txt");

    file << " " << setw(68) << setfill('_') << "" << endl;
    file << setfill(' ');
    file << "| Hash       | Collision         | Hash Function 1 | Hash Function 2 |" << endl;
    file << "| Table Size | Resolution Method | Colls. | Probes | Colls. | Probes |" << endl;
    file << "|" << setw(69) << setfill('_') << "|" << endl;
    file << setfill(' ');

    long long i = 0;

    for(long long n : size) {
        file << "| N = " << setw(7) << left << size[i];

        for(long long j = 0; j < 3; j++) {
            if(j == 0) file << "| Separate Chaining | ";
            else if(j == 1) file << "|" << setw(34) << right << "| Double Hashing    | ";
            else file << "|" << setw(34) << right << "| Custom Probing    | ";

            for(long long k = 0; k < 2; k++) {
                for(long long m = 0; m < 2; m++) {
                    if(m == 0) file << defaultfloat << setw(5) << setprecision(5) << right << report[i][j][k][m] << "  ";
                    if(m == 1) file << fixed << setw(2) << setprecision(3) << " " << report[i][j][k][m] << "  ";
                }

                file << "| ";
            }

            file << endl; 
        }

        file << "|" << setw(69) << setfill('_') << "|" << endl;
        file << setfill(' ');
        i++;
    }

    file.close();

    return 0;
}
