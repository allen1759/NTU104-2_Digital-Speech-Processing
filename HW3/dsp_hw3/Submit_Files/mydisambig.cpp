#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include "Ngram.h"
using namespace std;

void init(int argc, char* argv[], Vocab & voc, Ngram & lm, vector<string> & texts, map<string, set<string> > & dict);
vector<string> Viterbi(string & str, map<string, set<string> > & dict, Vocab & voc, Ngram & lm);
double getBigramProb(const char *w1, const char *w2, Vocab & voc, Ngram & lm);
double getTrigramProb(const char *w1, const char *w2, const char *w3, Vocab & voc, Ngram & lm);

int main(int argc, char* argv[])
{
    int ngram_order = 2;
    Vocab voc;
    Ngram lm( voc, ngram_order );

    vector<string> texts;
    map<string, set<string> > dict;

    init(argc, argv, voc, lm, texts, dict);


    for(int i=0; i<texts.size(); i+=1) {
        string newline;
        for(int j=0; j<texts[i].size(); j+=1) {
            if( texts[i][j]!=' ' )
                newline += texts[i][j];
        }
        vector<string> ans = Viterbi(newline, dict, voc, lm);

        cout << "<s> ";
        for(int j=0; j<ans.size(); j+=1) {
            cout << ans[j] << " ";
        }
        cout << "</s>" << endl;
    }


    return 0;
}


void init(int argc, char* argv[], Vocab & voc, Ngram & lm, vector<string> & texts, map<string, set<string> > & dict)
{
    fstream textfile, mapfile;
    for(int i=1; i+1<argc; i+=1) {
        string str = argv[i];
        if( str=="-lm" ) {
            File lmFile( argv[i+1], "r" );
            lm.read( lmFile );
            lmFile.close();
        }
        if( str=="-text" ) {
            textfile.open( argv[i+1], ios::in );
        }
        if( str=="-map" ) {
            mapfile.open( argv[i+1], ios::in );
        }
    }

    string line;
    while( getline(textfile, line) ) {
        texts.push_back(line);
    }
    while( getline(mapfile, line) ) {
        string word = line.substr(0, 2);
        set<string> & currset = dict[word];
        line = line.substr(3);
        string newline;
        for(int i=0; i<line.size(); i+=1) {
            if(line[i]!=' ')
                newline += line[i];
        }
        for(int i=0; i+1<newline.size(); i+=2) {
            currset.insert( newline.substr(i, 2) );
        }
    }
}

vector<string> Viterbi(string & str, map<string, set<string> > & dict, Vocab & voc, Ngram & lm)
{
    vector< vector<double> > dp( str.size()/2 );
    vector< vector<string> > words( str.size()/2 );
    vector< vector<int> > preind( str.size()/2 );

    for(int i=0; i<dp.size(); i+=1) {
        string word = str.substr(i*2, 2);

        dp[i].resize( dict[word].size() );
        words[i].resize( dict[word].size() );
        preind[i].resize( dict[word].size() );

        set<string>::iterator it = dict[word].begin();
        for(int j=0; j<words[i].size(); j+=1) {
            words[i][j] = *it;
            ++it;
        }
    }

    // initial
    for(int j=0; j<dp[0].size(); j+=1) {
        dp[0][j] = getBigramProb("", words[0][j].c_str(), voc, lm);
        preind[0][j] = 0;
    }
    // dp
    for(int i=1; i<dp.size(); i+=1) {
        for(int j=0; j<dp[i].size(); j+=1) {
            dp[i][j] = -1e9;
            for(int k=0; k<dp[i-1].size(); k+=1) {
                double prob = getBigramProb(words[i-1][k].c_str(), words[i][j].c_str(), voc, lm);
                prob += dp[i-1][k];
                if( prob > dp[i][j] ) {
                    dp[i][j] = prob;
                    preind[i][j] = k;
                }
            }
        }
    }

    double maxprob = -1e9;
    int maxind = 0;
    for(int i=0; i<dp.back().size(); i+=1) {
        if( dp.back()[i] > maxprob ) {
            maxprob = dp.back()[i];
            maxind = i;
        }
    }
    vector<string> ret( dp.size() );
    for(int i=ret.size()-1; i>=0; i-=1) {
        ret[i] = words[i][maxind];
        maxind = preind[i][maxind];
    }
    return ret;
}

// Get P(W2 | W1) -- bigram
double getBigramProb(const char *w1, const char *w2, Vocab & voc, Ngram & lm)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}

// Get P(W3 | W1, W2) -- trigram
double getTrigramProb(const char *w1, const char *w2, const char *w3, Vocab & voc, Ngram & lm) 
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);
    VocabIndex wid3 = voc.getIndex(w3);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);
    if(wid3 == Vocab_None)  //OOV
        wid3 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return lm.wordProb( wid3, context );
}