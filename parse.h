//
// Created by WatsonYang on 2019-09-24.
//

#ifndef WORDANALYSIS_PARSE_H
#define WORDANALYSIS_PARSE_H
#include<iostream>
#include <map>
#include <string>
using namespace std;
typedef pair<string,string> mpair;
map<string,string> makeTokenMap();
map<string, string> makeSymMap();

mpair* parseToken(string s);

mpair* parseIdenfr(string s);

mpair* parseInt(string s);

mpair* parseChar(string s);

mpair* parseString(string s);

mpair* parseNext(string * pstr);

vector<mpair>* parseInput(string input);

#endif //WORDANALYSIS_PARSE_H
