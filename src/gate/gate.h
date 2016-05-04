#ifndef GATE_H_
#define GATE_H_

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
    Gate(int id, GateType type, Gate** fanin);
    
    int getId();
    Gate** getFanin();

  private:
    int _id;
    Gate** _fanin;
    Gate** _nextGateList;
    GateType _type;
};

#endif
