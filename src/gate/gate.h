#ifndef GATE_H_
#define GATE_H_

#include<map>
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
    Gate(int id, GateType type, Gate** fanin);
    Gate(Gate &gate);
      
    int getId();
    Gate** getFanin();
    GateType getGateType();

    void setFanin(Gate** fanin);
    void setGateType(GateType type);

  private:
    int _id;
    Gate** _fanin;
    Gate** _nextGateList;
    GateType _type;
};

#endif
