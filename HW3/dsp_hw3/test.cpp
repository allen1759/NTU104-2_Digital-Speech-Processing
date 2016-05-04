#include <fstream>
#include <iostream>
#include <set>
using namespace std;

int main(int argc, char * argv[])
{
    fstream f1(argv[1], ios::in);
    fstream f2(argv[2], ios::in);
    fstream out("oo.txt", ios::out);

    set<string> s1, s2;
    string str;
    while( getline(f1, str) )
        s1.insert(str);
    while( getline(f2, str) )
        s2.insert(str);

    if( s1.size()!=s2.size() )
        out << "size error" << endl;
    auto i = s1.begin();
    auto j = s2.begin();
    while( i!=s1.end() && j!=s2.end() ) {
        if( *i != *j)
            out << *i << endl << *j << endl << endl;
        i++;
        j++;
    }


    return 0;
}
