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
	vector<int>* getfId();
	vector<int>* getfGId();
	vector<FaultType>* getfTList();
	vector< vector<int> >* getfOList();
    
  private:
    vector<int> _faultId;
    vector<int> _faultGateId;
    vector<FaultType> _faultTypeList;
    vector< vector<int> > _faultOutList;
};

class MyHash
{
//friend class CirMgr;
public:
   MyHash() : _numBuckets(0), _buckets(0) {}
   MyHash(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
   ~MyHash() { reset(); }

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<vector<int> >[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<vector<int> >& operator [] (size_t i) { return _buckets[i]; }
   const vector<vector<int> >& operator [](size_t i) const { return _buckets[i]; }
 
   	int check(int id, unsigned answer, unsigned * simu){
      	vector<vector<int> >* bn=&_buckets[bucketNum(answer)];
      	bool same_answer=false;
		int samegroup;
		for(int i=0 ; i< bn->size() ;i++ ){
         	vector<int>* f=&(*bn)[i];
         	if(simu[(*f)[0]-1]==answer){
				//cout<<"fuck uuuuuuuuuuuuuuuuuuuu  "<<(*f)[0]<<"  "<<id<<endl;
				same_answer=true;
				f->push_back(id);
				samegroup = (*f)[0];
				break;
			}
      	}
      	if(same_answer==false){
         	vector<int> newvector;
			newvector.push_back(id);
			bn->push_back(newvector);
         	return 0;
      	}
      	return samegroup;   
   	}   

private:

   size_t                   _numBuckets;
   vector<vector<int> >*        _buckets;

   size_t bucketNum(const unsigned k) const {
      size_t sum= k;
      return (sum % _numBuckets); 
   }
};


#endif
