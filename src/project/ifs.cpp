#include<iostream>
#include <gate/gate.h>
#include <circuit/circuit.h>

using namespace std;

int main(int argc, const char** argv) {
  cout << "Enter ISF Project" << endl;
  Circuit c;
  c.loadCircuit(argv[1]);
  c.checkGateLists();
}
