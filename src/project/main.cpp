#include<iostream>
#include <IFS/ifs.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char** argv) {
	cout << "Enter ISF Project" << endl;
	Circuit c; 
	c.loadCircuit(argv[1]);
	c.checkGateLists();
	for(int i=0;i<(c.getTopo())->size();i++){
		cout<<(*(c.getTopo()))[i]<<" ";
	}
	cout<<endl<<(c.getTopo())->size()<<endl;
	unsigned* pattern = new unsigned[c.getisize()];
	for(int i=0;i<c.getisize();i++){
		pattern[i] = rand()%4294967295;
	}
	unsigned* answer;
	answer = c.simulate(pattern, 1000,0);
	for(int i=0;i<c.getisize();i++){
		cout<<pattern[i]<<"  ";
	}
	cout<<endl;
	for(int i=0;i<c.getosize();i++){
		cout<<answer[i]<<"  ";
	}
	cout<<endl;
}
