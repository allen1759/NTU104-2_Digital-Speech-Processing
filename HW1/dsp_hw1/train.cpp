#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "hmm.h"
using namespace std;

void trainning( const vector<string> & allobserv, HMM & currModel );

int main(int argc, char* argv[])
{
    if( argc != 5 ) {
        cout << "Please input correct parameters." << endl;
        return -1;
    }
    // setting parameters
    int numOfIter;
    sscanf(argv[1], "%d", &numOfIter);
    string initFileName = argv[2];
    string trainFileName = argv[3];
    string outFileName = argv[4];

    // load init model from file (using hmm.h)
    HMM myModel;
    loadHMM( &myModel, initFileName.c_str() );
    // just for test
    // dumpHMM( stdout, &initModel );
    
    // load trainning data 
    fstream trainFile;
    trainFile.open(trainFileName.c_str(), ios::in);
    if( !trainFile.is_open() ) {
        cout << "Cannot find the file: " << trainFileName << endl;
        return -1;
    }
    vector<string> allobserv;
    string tmpstr;
    while( getline(trainFile, tmpstr) ) {
        allobserv.push_back( tmpstr );
    }
    
    
    // start iteration
    for( int i=0; i<numOfIter; i+=1 ) {
        trainning( allobserv, myModel );
    }
    
    // re-write trainning model
    FILE * outFilePointer = NULL;
    outFilePointer = fopen(outFileName.c_str(), "w");
    dumpHMM( outFilePointer, &myModel);

    return 0;
}

void trainning( const vector<string> & allobserv, HMM & currModel )
{
    const int stateNum = currModel.state_num;   // 1~N
    const int timeNum = allobserv.front().size(); // 1~T
    const int obsNum = currModel.observ_num;
    
    vector<double> accumuGamma(stateNum, 0.0);
    vector<double> accumuGammaFront(stateNum, 0.0);
    vector< vector<double> > accumuEpsilon(stateNum, std::vector<double>(stateNum, 0.0));
    vector< vector<double> > accumuObv(obsNum, std::vector<double>(stateNum, 0.0));
    
    
    for( int line=0; line<allobserv.size(); line+=1 ) {
        // initial alpha
        vector< vector<double> > alpha(stateNum, std::vector<double>(timeNum, 0.0));
        for( int i=0; i<stateNum; i+=1 ) {
            alpha[i][0] = currModel.initial[i];
            alpha[i][0] *= currModel.observation[ allobserv[line][0]-'A' ][ i ];
        }
        // induction alpha
        for( int t=1; t<timeNum; t+=1 ) {
            for( int i=0; i<stateNum; i+=1 ) {
                double val = 0;
                for( int j=0; j<stateNum; j+=1 ) {
                    val += alpha[j][t-1] * currModel.transition[j][i];
                }
                alpha[i][t] = val * currModel.observation[ allobserv[line][t]-'A' ][ i ];
            }
        }
        
        
        // initial beta
        vector< vector<double> > beta(stateNum, std::vector<double>(timeNum, 0.0));
        for( int i=0; i<stateNum; i+=1 ) {
            beta[i][timeNum-1] = 1.0;
        }
        // induction beta
        for( int t=timeNum-2; t>=0; t-=1 ) {
            for( int i=0; i<stateNum; i+=1 ) {
                double val = 0;
                for( int j=0; j<stateNum; j+=1 ) {
                    val += currModel.transition[i][j] *
                           currModel.observation[ allobserv[line][t+1]-'A' ][ j ] *
                           beta[j][t+1];
                }
                beta[i][t] = val;
            }
        }
        
        
        // ==================== gamma ====================
        vector< vector<double> > gamma(stateNum, std::vector<double>(timeNum, 0.0));
        // compute gamma
        for( int t=0; t<timeNum; t+=1 ) {
            double sum = 0;
            for( int i=0; i<stateNum; i+=1 ) {
                gamma[i][t] = alpha[i][t] * beta[i][t];
                sum += gamma[i][t];
            }
            for( int i=0; i<stateNum; i+=1 ) {
                gamma[i][t] /= sum;
            }
        }
        // ----- accumulate gamma -----
        for( int i=0; i<stateNum; i+=1 ) {
            accumuGammaFront[i] += gamma[i][0];
        }
        for( int t=0; t+1<timeNum; t+=1 ) {
            for( int i=0; i<stateNum; i+=1 ) {
                accumuGamma[i] += gamma[i][t];
            }
        }
        
        // ----- accumulate observation -----
        for( int i=0; i<obsNum; i+=1 ) {
            for( int j=0; j<stateNum; j+=1 ) {
                for( int k=0; k<allobserv[line].size(); k+=1 ) {
                    if( allobserv[line][k]-'A' == i ) {
                        accumuObv[i][j] += gamma[j][k];
                    }
                }
            }
        }
        
        
        // ==================== epsilon ====================
        vector< vector< vector<double> > > epsilon(stateNum, std::vector< std::vector<double> >(stateNum, std::vector<double>(timeNum, 0.0)));
        // compute epsilon
        for( int t=0; t+1<timeNum; t+=1 ) {
            double divide = 0.0;
            for( int i=0; i<stateNum; i+=1 ) {
                for( int j=0; j<stateNum; j+=1 ) {
                    epsilon[i][j][t] = alpha[i][t]
                                     * currModel.transition[i][j]
                                     * currModel.observation[ allobserv[line][t+1]-'A' ][ j ]
                                     * beta[j][t+1];
                    divide += epsilon[i][j][t];
                }
            }
            for( int i=0; i<stateNum; i+=1 ) {
                for( int j=0; j<stateNum; j+=1 ) {
                    epsilon[i][j][t] /= divide;
                    
                    // ----- accumulate epsilon -----
                    accumuEpsilon[i][j] += epsilon[i][j][t];
                }
            }
        }
    }
    
    
    // start modify the model
    
    // initial probability
    for( int i=0; i<stateNum; i+=1 ) {
        currModel.initial[i] = accumuGammaFront[i] / allobserv.size();
    }
    
    // transition matrix
    for( int i=0; i<stateNum; i+=1 ) {
        for( int j=0; j<stateNum; j+=1 ) {
            currModel.transition[i][j] = accumuEpsilon[i][j] / accumuGamma[i];
        }
    }
    
    // observation probability
    for( int i=0; i<obsNum; i+=1 ) {
        for( int j=0; j<stateNum; j+=1 ) {
            currModel.observation[i][j] = accumuObv[i][j] / accumuGamma[j];
        }
    }
}