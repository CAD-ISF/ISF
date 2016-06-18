#ifndef IFS_H_
#define IFS_H_

#include<vector>
//#include<circuit/circuit.h>

using namespace std;

class Circuit;
class MyHash;

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
    void faultOut(Circuit&);
	vector<int>* getfId();				// get fault id
	vector<int>* getfGId();				// get fault gate id
	vector<FaultType>* getfTList();		// get fault type list
	vector< vector<int> >* getfOList(); // get fault primary output list
    
  private:
    vector<int> _faultId;
    vector<int> _faultGateId;
    vector<FaultType> _faultTypeList;
    vector< vector<int> > _faultOutList;
};


#endif
