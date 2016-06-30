#include <stdlib.h>
#include <malloc/malloc.h>
#include "myHash.h"

using namespace std;

int MyHash::check(int id, unsigned answer, unsigned * simu) {
	// rough catagorize with bucketNum
	vector<vector<int> >* bn = &_buckets[bucketNum(answer)];

	bool same_answer = false;
	int samegroup;
	// check through all precise groups in bn
	for(int i = 0 ; i< bn->size();i++){
		vector<int>* f = &(*bn)[i];
		// judge with the answer of last fault in f
		if(simu[(*f)[0]-1] == answer){
			same_answer=true;
			f->push_back(id);
			samegroup = (*f)[0];
			break;
		}
	}
	// if no group of same pattern, create one
	if(same_answer==false){
		vector<int> newvector;
		newvector.push_back(id);
		bn->push_back(newvector);
		return 0;
	}
	return samegroup;   
}
