#ifndef GATE_H_
#define GATE_H_

#include<map>
#include<vector>
#include<string>

using namespace std;

enum GateType {
  INPUT,
  BUFF,
  NOT,
  AND,
  NAND,
  OR,
  NOR,
  XOR,
  NXOR
};


class Gate {
  public:
    Gate(int id);
    Gate(int id, GateType type);
    Gate(int id, GateType type, vector<Gate*>* fanin);
    Gate(Gate &gate);
    ~Gate();

    int getId();
    vector<Gate*>* getFanin();
    vector<Gate*>* getFanout();
    GateType getGateType();

    void setGateType(GateType type);
    void addFanin(Gate* fanin);
    void addFanout(Gate* fanout);
    void resetFanin();
    void resetFanout();

  private:
    int _id;
    vector<Gate*>* _fanin;
    vector<Gate*>* _nextGateList;
    GateType _type;
};

#endif
