#ifndef IFS_H_
#define IFS_H_

#include<circuit/circuit.h>

using namespace std;

enum FaultType {
	SA0,
	SA1,
	NEG,
	RDOB_AND,
	RDOB_NAND,
	RDOB_OR,
	RDOB_NOR,
	RDOB_XOR,
	RDOB_NXOR,
	RDOB_NOT,
	RDOB_BUFF
};

class IFS {
 
  public:
	IFS();
	~IFS();
	void loadFault(const char* filename);
	void faultOut(Circuit& c);
    
  private:
	vector<int> _faultId;
	vector<int> _faultGateId;
	vector<string> _faultTypeList;
	vector< vector<int> > _faultOutList;
};

#endif
