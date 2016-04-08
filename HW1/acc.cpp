#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    string file1= argv[1];
	string file2= argv[2];

	// load model name
    fstream fs1, fs2;
	fs1.open( file1.c_str(), ios::in );
	fs2.open( file2.c_str(), ios::in );
	if( !fs1.is_open() ) {
		cout << "Cannot find the file: " << file1 << endl;
		return -1;
	}
	if( !fs2.is_open() ) {
		cout << "Cannot find the file: " << file2 << endl;
		return -1;
	}

	int cntTotal = 0;
	int correct = 0;
	string s1, s2;
	while( getline(fs1, s1) && s1!="" ) {
		getline(fs2, s2);
		cntTotal += 1;
		if( s1==s2 )
			correct += 1;
	}
	cout << (double)correct / cntTotal << endl;

	fs1.close();
	fs2.close();

    return 0;
}

