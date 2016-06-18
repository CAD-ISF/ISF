#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include<map>
#include<gate/gate.h>
#include <IFS/ifs.h>

using namespace std;


class Circuit {
  public:
	friend class IFS;
	Circuit();
	~Circuit();
	void loadCircuit(const char* fileName);

	map< int, Gate* > getGateLists();

	void checkFaninById(int id);
	void checkId(int id);
	void checkGateLists();
	int* getPI();
	int* getPO();
	int getosize();
	int getisize();
	void topodfs(int);
	vector<int>* gettopo();
	unsigned simulate(unsigned*, int, FaultType);

	void outputDFS(int id, map<int, bool>& out);

  private:
	int* _inputs;
	int* _outputs;
	int _osize;
	int _isize;
	map< int, Gate* > _gateLists;
	vector<int>* _topoorder;
};

#endif
