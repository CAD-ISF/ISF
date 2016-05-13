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

  private:
    int* _inputs;
    int* _outputs;
    map< int, Gate* > _gateLists;


};

#endif
