#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    fstream in(argv[1], ios::in);
    fstream out(argv[2], ios::out);

    map<string, set<string> > dict;
    string word, zhuyin, z;
    while( in >> word >> zhuyin ) {
        stringstream ss(zhuyin);
        while( getline(ss, z, '/') ) {
            dict[ z.substr(0, 2) ].insert(word);
        }
    }

    for(auto & key : dict) {
        out << key.first;
        for(auto & word : key.second)
            out << " " << word;
        out << endl; 
        for(auto & word : key.second)
            out << word << " " << word << endl;
    }

    return 0;
}
