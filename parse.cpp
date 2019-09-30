//
// Created by WatsonYang on 2019-09-24.
//
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include <fstream>
#include "parse.h"
using namespace std;
bool isLetter(char c) {
    return c=='_' || (c>='a'&&c<='z') ||(c>='A'&&c<='Z');
}
bool isChar(char c) {
    return c=='+' || c=='-' || c == '*' || c=='/' || isdigit(c)||isLetter(c);
}

bool isStringChar(char c) {
    return c==32 || c==33 || (c>=35&&c<=126);
}
map<string,string> makeTokenMap() {
    map<string,string> tokenMap;
    tokenMap[string("const")] = "CONSTTK";
    tokenMap[string("int")] = "INTTK";
    tokenMap[string("char")] = "CHARTK";
    tokenMap[string("void")] = "VOIDTK";
    tokenMap[string("main")] = "MAINTK";
    tokenMap[string("if")] = "IFTK";
    tokenMap[string("else")] = "ELSETK";
    tokenMap[string("do")] = "DOTK";
    tokenMap[string("while")] = "WHILETK";
    tokenMap[string("for")] = "FORTK";
    tokenMap[string("scanf")] = "SCANFTK";
    tokenMap[string("printf")] = "PRINTFTK";
    tokenMap[string("return")] = "RETURNTK";

    return tokenMap;
}
map<string, string> makeSymMap() {
    map<string, string> tokenMap;
    tokenMap[string("+")] = "PLUS";
    tokenMap[string("-")] = "MINU";
    tokenMap[string("*")] = "MULT";
    tokenMap[string("/")] = "DIV";
    tokenMap[string("<")] = "LSS";
    tokenMap[string("<=")] = "LEQ";
    tokenMap[string(">")] = "GRE";
    tokenMap[string(">=")] = "GEQ";
    tokenMap[string("==")] = "EQL";
    tokenMap[string("!=")] = "NEQ";
    tokenMap[string("=")] = "ASSIGN";
    tokenMap[string(";")] = "SEMICN";
    tokenMap[string(",")] = "COMMA";
    tokenMap[string("(")] = "LPARENT";
    tokenMap[string(")")] = "RPARENT";
    tokenMap[string("[")] = "LBRACK";
    tokenMap[string("]")] = "RBRACK";
    tokenMap[string("{")] = "LBRACE";
    tokenMap[string("}")] = "RBRACE";
    return tokenMap;
}

//assume token cant be another token's prefix or gg
// int int1 ?
mpair* parseToken(string s) {
    map<string,string> tokenMap = makeTokenMap();
    auto it = tokenMap.begin();
    while(it != tokenMap.end()) {
        string pattern = it->first;
        int l = pattern.size();
        if(l>s.size()) {
            it ++;
            continue;
        }
        if(pattern==s.substr(0,l) && (s.size()==l || !isalnum(s[l]))) {
            return new mpair(pattern,it->second);
        }
        it ++;
    }
    return nullptr;
}

mpair* parseSymbol(const string& s) {
    // first match <= == >= != (although no '!'symbol)
    map<string, string> symMap = makeSymMap();

    if(s.size()>=2) {
        string sub = s.substr(0,2);
        //cout << sub <<endl;
        for(const auto& it : symMap){
            if(it.first==sub) {
                return new pair<string, string>(it.first,it.second);
            }
        }
        sub = s.substr(0,1);
        //cout << sub <<endl;
        for(const auto& it : symMap){
            if(it.first==sub) {
                return new pair<string, string>(it.first,it.second);
            }
        }
    }
    else {
        string sub = s.substr(0,1);
        for(const auto& it : symMap){
            if(it.first==sub) {
                return new pair<string, string>(it.first,it.second);
            }
        }
    }
    return nullptr;
}

// Idenfr = "a-zA-Z{a-zA-Z0-9}
mpair* parseIdenfr(string s) {
    int i = 0;
    while(i < s.size()) {
        if((i == 0 && isLetter(s[i])) || (i!=0 && (isdigit(s[i]) || isLetter(s[i])))) {
            i++;
        }
        else {
            break;
        }
    }
    if(i==0) {
        return nullptr;
    }
    return new pair<string, string>(s.substr(0,i),"IDENFR");
}

//unsigned int
mpair* parseInt(string s) {
    int i = 0;
    bool hasSign = false;
    while(i<s.size()) {

        if(isdigit(s[i])) {
            i++;
        }
        else {
            break;
        }
    }

    if(i>=1) {
        return new pair<string, string>(s.substr(0,i),"INTCON");
    }
    return nullptr;
}

mpair* parseChar(string s) {
    if(s.size()>=3 && s[0]=='\'' && isChar(s[1]) &&s[2]=='\'') {
        auto p =new pair<string, string>(s.substr(0,3),"CHARCON");
        return p;
    }
    else {
        return nullptr;
    }
}

mpair* parseString(string s) {
    if(s[0] != '\"') {
        return nullptr;
    }
    int i = 1;
    while(i<s.size() &&isStringChar(s[i])) {
        i++;
    }
    if(i<s.size() && s[i] == '\"')
    {
        i++;
        return new pair<string, string>(s.substr(0,i),"STRCON");
    }
    return nullptr;
}

mpair* _parseNext(const string& input) {
    pair<string, string>* match = nullptr;
    match = parseToken(input);
    if(match) {
        return match;
    }
    match = parseIdenfr(input);
    if(match) {
        return match;
    }
    match = parseSymbol(input);
    if(match) {
        return match;
    }
    match = parseString(input);
    if(match) {
        return match;
    }
    match = parseChar(input);
    if(match) {
        return match;
    }
    match = parseInt(input);
    if(match) {
        return match;
    }
    return match;
}

mpair* parseNext(string * pstr) {
    if(pstr== nullptr||pstr->empty()) {
        return nullptr;
    }
    while(isspace((*pstr)[0])) {
        *pstr = (*pstr).substr(1);
    }
    if(pstr->empty()) {
        return nullptr;
    }
    auto p = _parseNext(*pstr);
    if(p!= nullptr)
        *pstr = pstr->substr(p->first.size());

    return p;
}


