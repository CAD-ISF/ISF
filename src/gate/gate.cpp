#include "gate.h"
#include<iostream>

using namespace std;

Gate::Gate(int id) {
  _id = id;
}

Gate::Gate(int id, GateType type) {
  _id = id;
  _type = type;
}

Gate::Gate(int id, GateType type, Gate** fanin) {
    _id = id;
    _type = type;
    _fanin = fanin;  
}

Gate::Gate(Gate &gate) {
  _id = gate._id;
  _type = gate._type;
  _fanin = gate._fanin;

}

int Gate::getId() {
  return _id;
}

Gate** Gate::getFanin() {
  return _fanin;

}

GateType Gate::getGateType() {
  return _type;
}

void Gate::setFanin(Gate** fanin) {
  _fanin = fanin;

}

void Gate:: setGateType(GateType type) {
  _type = type;
}
