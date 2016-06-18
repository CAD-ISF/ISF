#include<iostream>
#include<fstream>
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc/malloc.h>
#include"ifs.h"
#include<circuit/circuit.h>

using namespace std;

IFS::IFS() {}
IFS::~IFS() {}

void IFS::loadFault(const char *filename) {

    ifstream faultFile(filename);
    string line, temp;
	FaultType faulttype;
	char* ch1;
	char* ch2;
    while(getline(faultFile, line)) {
        size_t start = 0, end = 0;
        end = line.find_first_of(" \t\n", start);
        temp = line.substr(start, end - start);
        //cout << temp << '\t';
        if (temp != "") _faultId.push_back(atoi(temp.c_str()));
        start = end + 1;
        
        while((end = line.find_first_of(" \t\n", start)) != string::npos) {
            temp = line.substr(start, end - start);
            start = end + 1;
            if(temp == "") continue;
            else {
                //cout << temp << '\t';
                _faultGateId.push_back(atoi(temp.c_str()));
                break;
            }
        }     
        temp = line.substr(start, end - start);
		temp.erase(0,temp.find_first_not_of("\t"));
		if(int(temp[temp.length()-1])==13)
			temp.erase(temp.length()-1,1);
		//cout << temp << '\n';
        //if (temp != "") _faultTypeList.push_back(temp);
		if(temp=="SA0")
			faulttype=SA0;
		else if(temp=="SA1")
			faulttype=SA1;
		else if(temp=="NEG")
			faulttype=NEG;
		else if(temp=="RDOB_AND")
			faulttype=RDOB_AND;
		else if(temp=="RDOB_NAND")
			faulttype=RDOB_NAND;
		else if(temp=="RDOB_OR")
			faulttype=RDOB_OR;
		else if(temp=="RDOB_NOR")
			faulttype=RDOB_NOR;
		else if(temp=="RDOB_XOR")
			faulttype=RDOB_XOR;
		else if(temp=="RDOB_NXOR")
			faulttype=RDOB_NXOR;
		else if(temp=="RDOB_NOT")
			faulttype=RDOB_NOT;
		else if(temp=="RDOB_BUFF")
			faulttype=RDOB_BUFF;
 		//cout<<endl<<temp<<endl<<temp.length();
		_faultTypeList.push_back(faulttype);       
    }
}

// find primary outputs of each fault gate
void IFS::faultOut(Circuit& c) {
    cout << "faultOut!\n";
    for(size_t i = 0; i < _faultGateId.size(); i++) {
        map<int, bool> out;
        c.outputDFS(_faultGateId[i], out);
        vector<int> outs;
        cout << '[' << _faultId[i] << "]\t";
        for(map<int, bool>::iterator it = out.begin(); it != out.end(); it++) {
            cout << it->first << ' ';
            outs.push_back(it->first);
        }
        cout << '\n';
        _faultOutList.push_back(outs);
    }
}

vector<int>* IFS::getfId(){
	return &_faultId;
}

vector<int>* IFS::getfGId(){
	return &_faultGateId;
}

vector<FaultType>* IFS::getfTList(){
	return &_faultTypeList;
}

vector< vector<int> >* IFS::getfOList(){
	return &_faultOutList;
}


