#include <iostream>
#include <time.h>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include "chaining.h"
#include "double&Custom.h"
using namespace std;

bool isPrime(long long n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
    {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }
    for (long long i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

long long nextPrime(long long n)
{
    if (n <= 1)
        return 2;
    long long prime = n;
    bool found = false;
    while (!found)
    {
        prime++;
        if (isPrime(prime))
            found = true;
    }
    return prime;
}

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
    string uniqueWords[N];
    long long size[3] = {5000, 10000, 20000};
    double report [3][3][2][2];

    for (long long i = 0; i < N; i++)
    {
        words[i] = randomWordGenerator();
    }

    //for loop for 10000 unique words
    unordered_set<string> seenWords;
    long long x=0;
    while(x!=10000)
    {
        string word = randomWordGenerator();
        if(seenWords.count(word) == 0)  
        {
            seenWords.insert(word);
            uniqueWords[x] = word;
            x++;
        }
    }

    long long n=0;

    for (long long s = 0; s < 3; s++)
    {
        for (long long i = 1; i <= 2; i++)
        {
            chaining *chain = new chaining(size[s], i, 5);

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
                chain->find(uniqueWords[m]);
            }
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
                    c->find(uniqueWords[k]);
                }


                report[n][j][i - 1][0] = c->getCollisions();    
                report[n][j][i - 1][1] = (double) c->getProbes() / T;

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
