//
// Created by WatsonYang on 2019-09-26.
//

#ifndef GRAMMERANALYSIS_GRAMMER_H
#define GRAMMERANALYSIS_GRAMMER_H

#include <string>
#include <fstream>
#include <iostream>
using namespace std;
void grammerAnalysis(string source, ofstream* out);
map<string,vector<mpair>*(*)(string *)> funcmap();
vector<mpair>* _string(string *pstr);
vector<mpair>* program(string *pstr);
vector<mpair>* addOptr(string *pstr);
vector<mpair>* multOptr(string *pstr);
vector<mpair>* releOptr(string *pstr);
vector<mpair>*  expression(string *pstr);
vector<mpair>*  statement(string *pstr);
vector<mpair>* character(string *pstr);
vector<mpair>* unsignedInt(string *pstr);
vector<mpair>*  integer(string *pstr);

vector<mpair>*  typeIdentifier(string *pstr);
vector<mpair>*  headDeclaration(string *pstr);
vector<mpair>*  constDefinition(string *pstr);
vector<mpair>*  constDeclaration(string *pstr);
vector<mpair>*  arrayIdentifier(string *pstr);
vector<mpair>*  varDefinition(string *pstr);
vector<mpair>*  varDeclaration(string *pstr);
vector<mpair>*  term(string *pstr);
vector<mpair>*  factor(string *pstr);
vector<mpair>*  printStatement(string *pstr);
vector<mpair>* assignStatement(string *pstr);
vector<mpair>* returnStatement(string *pstr);
vector<mpair>* compoundStatement(string *pstr);
vector<mpair>*  statementlist(string *pstr);
vector<mpair>* _main(string *pstr);

#endif //GRAMMERANALYSIS_GRAMMER_H
