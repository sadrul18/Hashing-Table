#include<iostream>
#include<time.h>
#include <unordered_set>
using namespace std;

string randomWordGenerator()
{
    string word;
    int len=rand()%6+5;
    for(int i=0;i<len;i++)
    {
        word+='a'+rand()%25;
    }

    return word;
}

int main()
{
    freopen("output3.txt","w",stdout);
    srand(time(NULL));
    unordered_set<string> seenWords;
    int duplicate=0;
    for(int i = 0; i < 10000; ++i)
    {
        string word = randomWordGenerator();
        if(seenWords.count(word) > 0) 
        {
            cout << "[DUPLICATE] " << word << endl;
            duplicate++;
        }
        else 
        {
            seenWords.insert(word);
            cout << word << endl;
        }
    }

    cout<<endl<<"Total duplicate words are: "<<duplicate<<endl;
    return 0;
}