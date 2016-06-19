#include <iostream>
#include <IFS/ifs.h>
#include <IFS/myHash.h>
#include <stdlib.h>
#include <circuit/circuit.h>

using namespace std;

void printfec(vector<vector<int> >&);

int main(int argc, const char** argv) {
	cout << "Enter ISF Project" << endl;
	Circuit c; 
	c.loadCircuit(argv[1]);
	//c.checkGateLists();
	IFS ifs;
	ifs.loadFault(argv[2]);
	//ifs.faultOut(c);
	//ifs.groupf(c);
	printfec(*(ifs.groupf(c)));
}

void printfec ( vector<vector<int> >& a ) {
	cout << "fec group : " << endl;
	int count=0;
	for ( int i = 0; i < a.size(); i++ ) {
		cout << "[" << i + 1 << "] = ";
		for ( int j = 0; j < (a[i]).size(); j++ ) {
			cout << "  " << (a[i])[j];
		}
		cout << endl;
		count++;
	}
	cout << "group count = " << count << endl;
}

