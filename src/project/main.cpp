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
	int faultsize = (ifs.getfGId())->size();
	unsigned answer;
	unsigned* simu = new unsigned[faultsize];
	//for(int i=0;i<(c.gettopo())->size();i++){
	//cout<<(*(c.gettopo()))[i]<<" ";
	//}
	// randomly generate patterns
	cout << "patterns : " << '\n';
	unsigned* pattern = new unsigned[c.getisize()];
	for(int i=0;i<c.getisize();i++){
		unsigned pow = 1;
		pattern[i] = 0;
		for(int j = 0; j < 32; j++) {
			pattern[i] += (rand() % 2) * pow;
			if(j != 31) pow *= 2;
		}
		cout << '[' << c.getPI()[i] << "] = " << pattern[i] << '\n';
	}
	cout << '\n';

	// simulation calculation
	cout << "simulations : " << '\n';
	for(int i=0;i<faultsize;i++){
		answer = c.simulate(pattern, (*ifs.getfGId())[i] ,(*ifs.getfTList())[i]);
		simu[i] = answer;
		cout << '[' << (*ifs.getfId())[i] << "] = " << simu[i] << '\n';
	}
	cout << endl;

	// first-time grouping......................................................
	// .....
	vector<vector<int> > fecgroup;	// list of all groups
	MyHash* myhash = new MyHash(1024);
	for (int i=0;i<faultsize;i++){
		// check for faults with the same answer and catagorize
		myhash->check((*ifs.getfId())[i], simu[i], simu);
	}
	// for all rough groups in myhash
	for (int i = 0; i < myhash->numBuckets(); i++){
		vector<vector<int> > f = (*myhash)[i];
		for (int j=0; j < (*myhash)[i].size(); j++){
			if (f[j].size() > 1)
				fecgroup.push_back(f[j]);
		}
	}
	//cout<<"first : "<<endl;
	//printfec(fecgroup);
	unsigned oldsize;
	oldsize = fecgroup.size();
	cout << "fecgroup size = " << oldsize << endl;

	// Second time..............................................................
	// .....
	int count = 0;
	while (count != 50) {
		for (int i = 0; i < c.getisize(); i++) {
			unsigned pow = 1;
			pattern[i] = 0;
			for (int j = 0; j < 32; j++) {
				pattern[i] += (rand() % 2) * pow;
				if (j != 31) pow *= 2;
			}
		}
		vector<vector<int> > newfecgroup;	// new list of fec groups
		vector<vector<int> > temp;	// new list of fec groups
		bool same_answer;
		// for each group
		for ( int i = 0; i < fecgroup.size(); i++ ) {
			temp.clear();
			// generate simulation answer for each id
			for ( int j = 0; j < (fecgroup[i]).size(); j++ ) {
				size_t k = (fecgroup[i])[j] - 1;
				simu[k] = c.simulate(pattern, (*ifs.getfGId())[k], (*ifs.getfTList())[k]);
			}
			// re-grouping
			for ( int j = 0; j < (fecgroup[i]).size(); j++ ) {
				same_answer = false;
				for ( int k = 0; k < temp.size(); k++ ) {
					vector<int>* f = &(temp[k]);
					if ( simu[ (*f)[0] - 1 ] == simu[ (fecgroup[i])[j] - 1 ] ) {
						same_answer = true;
						f->push_back((fecgroup[i])[j]);
						break;
					}
				}
				if ( same_answer == false ) {
					vector<int> newvector;
					newvector.push_back( (fecgroup[i])[j] );
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
		else cout << "fecgroup size = " << newfecgroup.size() << endl;
		fecgroup = newfecgroup;
		oldsize = fecgroup.size();
	}
	//cout<<"second : "<<endl;
	printfec(fecgroup);


	delete [] pattern;
	delete [] simu;
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
