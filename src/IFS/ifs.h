#ifndef IFS_H_
#define IFS_H_

#include <vector>
#include "myHash.h"
#include <circuit/circuit.h>
//#include <circuit.h>

using namespace std;

class Circuit;

class IFS {

  public:
	IFS();
	~IFS();
	void loadFault(const char* filename);
	void faultOut(Circuit&);
	vector<int>*			getfId();				// get fault id
	vector<int>*			getfGId();				// get fault gate id
	vector<FaultType>*		getfTList();			// get fault type list
	int						getfGIdbyfId(int);		// get fault gate id by fault id
	FaultType				getfTbyfId(int);		// get fault type by fault id
	vector< vector<int> >*	getfOList();			// get fault primary output list
	vector< vector<int> >*	groupf(Circuit&);
	vector< vector<int> >*	getFecGroup();

  private:
	map< int, pair<int, FaultType> > _fault;
	vector<int> _faultId;
	vector<int> _faultGateId;
	vector<FaultType> _faultTypeList;
	vector< vector<int> > _faultOutList;
	vector< vector<int> > _fecGroup;
};


#endif
