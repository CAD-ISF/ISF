#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <malloc/malloc.h>
#include "ifs.h"
#include <circuit/circuit.h>
 
using namespace std;

void printinBinary(size_t);

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
		
		_fault[ _faultId.back() ] = make_pair( _faultGateId.back(), _faultTypeList.back() );
	}
}

vector<int>* IFS::getfId() {
	return &_faultId;
}

vector<int>* IFS::getfGId() {
	return &_faultGateId;
}

vector<FaultType>* IFS::getfTList() {
	return &_faultTypeList;
}

int IFS::getfGIdbyfId( int id ) {
	return _fault[id].first;
}

FaultType IFS::getfTbyfId( int id ) {
	return _fault[id].second;
}

vector< vector<int> >* IFS::getfOList() {
	return &_faultOutList;
}

vector< vector<int> >* IFS::getFecGroup() {
	return &_fecGroup;
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

vector<vector<int> >* IFS::groupf(Circuit &c) {
	int faultsize = _faultGateId.size();
	unsigned answer;
	unsigned* simu = new unsigned[faultsize];

	// randomly generate patterns
	cout << "patterns : " << '\n';
	unsigned* pattern = new unsigned[c.getisize()];
	for ( int i = 0; i < c.getisize(); i++ ) {
		unsigned pow = 1;
		pattern[i] = 0;
		for ( int j = 0; j < 32; j++ ) {
			pattern[i] += ( rand() % 2 ) * pow;
			if ( j != 31 ) pow *= 2;
		}
	}
	for ( int i = 0; i < c.getisize(); i++ ) {
		char print[1000];
		sprintf(print, "[%d] = ", c.getPI()[i]);
		cout << setw(10) << print;
		printinBinary(pattern[i]);
		cout << '\n';
	}
	cout << '\n';

	// simulation calculation
	cout << "simulations : " << '\n';
	for ( int i = 0; i < faultsize; i++ ) {
		answer = c.simulate(pattern, _faultGateId[i] ,_faultTypeList[i]);
		simu[i] = answer;
	}
	for ( int i = 0; i < faultsize; i++ ) {
		char print[1000];
		sprintf(print, "[%d] = ", _faultId[i]);
		cout << setw(10) << print;
		printinBinary(simu[i]);
		cout << '\n';
	}
	cout << endl;

	// first-time grouping......................................................
	// .....
	MyHash* myhash = new MyHash(1024);
	for ( int i = 0; i < faultsize; i++ ) {
		// check for faults with the same answer and catagorize
		myhash->check( _faultId[i], simu[i], simu );
	}

	// collect precise groups from myhash into _fecGroup
	for ( int i = 0; i < myhash->numBuckets(); i++ ) {
		vector<vector<int> > f = (*myhash)[i];
		for ( int j = 0; j < (*myhash)[i].size(); j++ ) {
			if ( f[j].size() > 1 )
				_fecGroup.push_back(f[j]);
		}
	}

	unsigned oldsize;
	oldsize = _fecGroup.size();
	cout << "_fecGroup size = " << oldsize << endl;

	// Second-time grouping.....................................................
	// .....
	int count = 0;
	while ( count != 500 ) {
		for ( int i = 0; i < c.getisize(); i++ ) {
			unsigned pow = 1;
			pattern[i] = 0;
			for ( int j = 0; j < 32; j++ ) {
				pattern[i] += ( rand() % 2 ) * pow;
				if ( j != 31 ) pow *= 2;
			}
		}
		vector<vector<int> > newfecgroup;	// new list of fec groups
		vector<vector<int> > temp;	// new list of fec groups
		bool same_answer;
		// for each group
		for ( int i = 0; i < _fecGroup.size(); i++ ) {
			temp.clear();
			// generate simulation answer for each id
			for ( int j = 0; j < _fecGroup[i].size(); j++ ) {
				size_t k = _fecGroup[i][j] - 1;
				simu[k] = c.simulate( pattern, _faultGateId[k], _faultTypeList[k] );
			}
			// re-grouping
			for ( int j = 0; j < _fecGroup[i].size(); j++ ) {
				same_answer = false;
				for ( int k = 0; k < temp.size(); k++ ) {
					vector<int>* f = &(temp[k]);
					if ( simu[ (*f)[0] - 1 ] == simu[ _fecGroup[i][j] - 1 ] ) {
						same_answer = true;
						f->push_back( _fecGroup[i][j] );
						break;
					}
				}
				if ( same_answer == false ) {
					vector<int> newvector;
					newvector.push_back( _fecGroup[i][j] );
					temp.push_back( newvector );
				}
			}
			for ( int k = 0; k < temp.size(); k++ ) {
				if ( temp[k].size() > 1 ) {
					newfecgroup.push_back( temp[k] );
				}
			}
		}
		if ( oldsize == newfecgroup.size() ) count++;
		else {
			cout << "_fecGroup size = " << newfecgroup.size() << endl;
			count = 0;
		}
		_fecGroup = newfecgroup;
		oldsize = _fecGroup.size();
	}

	delete [] pattern;
	delete [] simu;
	return &_fecGroup;
}

void printinBinary( unsigned long int n ) {
	unsigned p = 1 << 31;
	for ( int i = 0; i < 32; i++ ) {
		if ( n & p ) cout << "1 ";
		else cout << "0 ";
		p >>= 1;
	}
}
