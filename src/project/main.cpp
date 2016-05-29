#include<iostream>
//#include <gate/gate.h>
//#include <circuit/circuit.h>
#include <IFS/ifs.h>

using namespace std;

int main(int argc, const char** argv) {
  cout << "Enter ISF Project" << endl;
  Circuit c;
  c.loadCircuit(argv[1]);
  //c.checkGateLists();
  IFS ifs;
  ifs.loadFault(argv[2]);
  ifs.faultOut(c);

}
