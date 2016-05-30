#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include<map>
#include<gate/gate.h>

using namespace std;

class Circuit {
  public:
    Circuit();
    ~Circuit();
    void loadCircuit(const char* fileName);
    
    map< int, Gate* > getGateLists();
    
    void checkFaninById(int id);
    void checkId(int id);
    void checkGateLists();
    
    int* getPO();
    int getosize();
    int getisize();
    
    void topoDfs(int);
    vector<int>* getTopo();
    unsigned* simulate(unsigned*, int, int);

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
