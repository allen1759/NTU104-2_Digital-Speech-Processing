#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "hmm.h"
using namespace std;

const int modelSize = 5;
void testing( HMM (&allModels)[modelSize], fstream & testFile, fstream & resFile, vector<string> & modelNameStr );
int Viterbi( HMM (&allModels)[modelSize], const string & obsers );

int main(int argc, char* argv[])
{
    //if( argc != 5 ) {
    //    cout << "Please input correct parameters." << endl;
    //    return -1;
    //}
    // setting parameters
    //int numOfIter;
    //sscanf(argv[1], "%d", &numOfIter);
    //string initFileName = argv[2];
	//string trainFileName = argv[3];
    //string outFileName = argv[4];
	int numOfIter = 10;
	string modelListName = "modellist.txt";
	string testFileName = "testing_data1.txt";
	string resultFileName = "result1.txt";

	// load model name
    fstream modelList;
	modelList.open( modelListName.c_str(), ios::in );
	if( !modelList.is_open() ) {
		cout << "Cannot find the file: " << modelListName << endl;
		return -1;
	}
	vector<string> modelNameStr;
	string line;
	while( getline(modelList, line) && line!="" ) {
		modelNameStr.push_back( line );
	}
	modelList.close();

    // load all of models (using hmm.h)
	HMM allModels[modelSize];
	Load_models( modelListName.c_str(), allModels, modelSize );
	dump_models( allModels, modelSize );
    
	fstream testFile;
	testFile.open( testFileName.c_str(), ios::in );
	if( !testFile.is_open() ) {
		cout << "Cannot find the file: " << testFileName << endl;
		return -1;
	}

	fstream resFile;
	resFile.open( resultFileName.c_str(), ios::out );
	if( !resFile.is_open()	) {
		cout << "Cannot find the file: " << testFileName << endl;
		return -1;
	}

	testing( allModels, testFile, resFile, modelNameStr );
    
    testFile.close();
    resFile.close();

    return 0;
}


void testing( HMM (&allModels)[modelSize], fstream & testFile, fstream & resFile, vector<string> & modelNameStr )
{
	string line;
	int cntTotal = 0;
	int correct = 0;

	while( getline(testFile, line) && line!="" ) {
		int predictModle = Viterbi( allModels, line);

		resFile << modelNameStr[ predictModle ] << endl;
	}
}

int Viterbi( HMM (&allModels)[modelSize], const string & obsers )
{
	const int stateNum = allModels[0].state_num;	// 1~N
	const int timeNum = obsers.size();				// 1~T
	vector< vector< vector<double> > > delta(modelSize, std::vector< std::vector<double> >(stateNum, std::vector<double>(timeNum, 0.0)));	//[modelNum][stateNum][timeNum]

	// delta for all models
	for (int m = 0; m < modelSize; m += 1) {
		// initial delta varible (same as alpha)
		HMM & currModel = allModels[m];
		for (int i = 0; i < stateNum; i += 1) {
			delta[m][i][0] = currModel.initial[i];
			delta[m][i][0] *= currModel.observation[obsers[0] - 'A'][i];
		}
	}

	for( int m=0; m<modelSize; m+=1 ) {
		// induction delta
		HMM & currModel = allModels[m];
		for( int t=0; t+1<timeNum; t+=1 ) {
			for( int j=0; j<stateNum; j+=1 ) {
				double currAns = 0.0;
				for( int i=0; i<stateNum; i+=1 ) {
					currAns = delta[m][i][t] * currModel.transition[i][j];
					if( currAns > delta[m][j][t+1] )
						delta[m][j][t+1] = currAns;
				}
				delta[m][j][t+1] *= currModel.observation[ obsers[t]-'A' ][ j ];
			}
		}
		// for debug
		double print[6][50];
		for(int i=0; i<6; i+=1) {
			for(int j=0; j<50; j+=1) {
				print[i][j] = delta[m][i][j];
			}
		}

		int x = 0;
	}

	double maxVal = 0.0;
	int maxModel = 0;
	double arr[10][10] = { 0 };
	for( int m=0; m<modelSize; m+=1 ) {
		for( int i=0; i<stateNum; i+=1 ) {
			if (arr[m][i] < delta[m][i][timeNum - 1])
				arr[m][i] = delta[m][i][timeNum - 1];

			if( delta[m][i][timeNum-1] > maxVal ) {
				maxVal = delta[m][i][timeNum-1];
				maxModel = m;
			}
		}
	}

	return maxModel;
}

