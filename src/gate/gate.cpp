#include "gate.h"

using namespace std;

Gate::Gate(int id) {
  _id = id;
}

Gate::Gate(int id, GateType type, Gate** fanin) {
    _id = id;
    _type = type;
    _fanin = fanin;  
}

int Gate::getId() {
  return _id;
}

Gate** Gate::getFanin() {
  return _fanin;

}

