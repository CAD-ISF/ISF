#include<iostream>
#include<fstream>
#include<string>
#include "stdlib.h"
#include"ifs.h"

using namespace std;

IFS::IFS() {}
IFS::~IFS() {}

void IFS::loadFault(const char *filename) {

	ifstream faultFile(filename);
	string line, temp;
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
		//cout << temp << '\n';
		if (temp != "") _faultTypeList.push_back(temp);

	}
}

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


