#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "parse.h"
#include "grammer.h"
using namespace std;
void readTxt(string &file) {
    cout<< "-------------------------------------"<<endl;
    ifstream infile;
    infile.open(file);
    //assert(infile.is_open());

    string s;
    while(!infile.eof()) {
        infile >> s;
        cout << s <<endl;
    }
    infile.close();
}

void readTxt1(string &file) {
    cout<< "-------------------------------------"<<endl;
    ifstream infile;
    infile.open(file);
    //assert(infile.is_open());

    char c;
    while(!infile.eof()) {
        infile >> c;
        cout << c;
    }
    infile.close();
}

vector<char> readTxt2(string &file) {
    cout<< "-------------------------------------"<<endl;
    vector<char> input;
    ifstream infile;
    infile.open(file);
    //assert(infile.is_open());

    char c;
    infile >> noskipws; // include space and enter
    while(!infile.eof()) {
        infile >> c;
        cout << c;
        input.push_back(c);
    }
    infile.close();
    return input;
}

string readFile(string &file) {
    fstream fs(file);
    stringstream ss;
    ss << fs.rdbuf();
    fs.close();

    return ss.str();
}


void test1() {
    string infilePath = "testfile.txt";
    string outfilePath = "output.txt";
    map<string,string> tokenMap;
    //readTxt(infilePath);
    //readTxt1(infilePath);
    //vector<char> input = readTxt2(infilePath);
    string inputStr = readFile(infilePath);
    //if(!infile) std::cout<<"Failed"<<std::endl;
    cout<< inputStr <<endl;

    ofstream outfile(outfilePath);
    outfile << "CONSTTK const";
    outfile.close();
}

void test2() {
    string s = "abcdefg";
    cout << s.substr(3) << endl;
    cout << s.substr(0,1) << endl;
    cout << s.substr(0,0).size() <<endl;
}
int main() {
    string filePath = "testfile.txt";
    string outfilePath = "output.txt";
    string source = readFile(filePath);
    ofstream outfile(outfilePath);

    grammerAnalysis(source,&outfile);
    return 0;
}