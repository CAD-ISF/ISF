#include "gate.h"
#include<iostream>

using namespace std;

Gate::Gate(int id) {
  _id = id;
  _fanin = new vector<Gate*>;
  _nextGateList = new vector<Gate*>;
  mark =0;
}

Gate::Gate(int id, GateType type) {
  _id = id;
  _type = type;
  _fanin = new vector<Gate*>;
  _nextGateList = new vector<Gate*>;
  mark =0;
}

Gate::Gate(int id, GateType type, vector<Gate*>* fanin) {
  _id = id;
  _type = type;
  _fanin = fanin;  
  _nextGateList = new vector<Gate*>;
  mark =0;
}

Gate::Gate(Gate &gate) {
  _id = gate._id;
  _type = gate._type;
  _fanin = gate._fanin;
  _nextGateList = gate._nextGateList;
  mark =0;
}

Gate::~Gate() {
  if (_fanin != NULL) delete _fanin;
  if (_nextGateList != NULL) delete _nextGateList;
}

int Gate::getId() {
  return _id;
}

vector<Gate*>* Gate::getFanin() {
  return _fanin;
}

vector<Gate*>* Gate::getFanout() {
  return _nextGateList;
}

GateType Gate::getGateType() {
  return _type;
}

void Gate::setGateType(GateType type) {
  _type = type;
}

void Gate::addFanin(Gate* fanin) {
  _fanin->push_back(fanin);
}

void Gate::addFanout(Gate* fanout) {
  _nextGateList->push_back(fanout);
}

void Gate::resetFanin() {
  _fanin->clear();
}

void Gate::resetFanout() {
  _nextGateList->clear();
}
