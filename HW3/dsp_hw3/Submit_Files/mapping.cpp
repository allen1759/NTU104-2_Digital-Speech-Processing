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

    for(map<string, set<string> >::iterator it=dict.begin(); it!=dict.end(); ++it) {
        out << it->first;
        for(set<string>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
            out << " " << *i;
        out << endl;
        for(set<string>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
            out << *i << " " << *i << endl;
    }

    return 0;
}
