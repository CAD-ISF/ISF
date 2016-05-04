#ifndef CIRCUIT_H_
#define CIRCUIT_G_

#include<map>
#include<gate/gate.h>

using namespace std;

class Circuit {
  public:
    Circuit();
    void loadCircuit(const char* fileName);
    void checkFaninById(int id);

  private:
    int* _inputs;
    int* _outputs;
    map< int, Gate* > _gateLists;


};

#endif
