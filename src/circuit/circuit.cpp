#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <stdlib.h>
#include "circuit.h"
#include "util.h"

using namespace std;

Circuit::Circuit(){}

void Circuit::loadCircuit(const char* fileName) {
  ifstream cirFile(fileName);
  if (cirFile.is_open()) {
    string line;
    int inputIdx = 0, outputIdx = 0;
    while (getline(cirFile, line)) {
      if ( line.length() > 1) {
        vector<string> parLine;
        strParse(line, parLine);
        if (parLine[0] == "#") {
          // allocate inputs
          if (parLine[2] == "inputs") {
            _inputs = new int[atoi(parLine[1].c_str())];
          }
          // allocate outputs
          else if (parLine[2] == "outputs") {
            _outputs = new int[atoi(parLine[1].c_str())];
          }
          // allocate DFF
          //
          //----ToDo-----
          //
          // allocate gates
          //
          //
        }
        else {
          //
          // Read Inputs
          //
          if (parLine[0].find("INPUT") != parLine[0].npos ) {
            // parse input signal id
            size_t idPos = parLine[0].find("(") + 1;
            string sid = parLine[0].substr(idPos, parLine[0].length() - 1 - idPos);
            // add to _inputs
            _inputs[inputIdx] = atoi(sid.c_str());
            // add to _gateLists
            Gate newInput(_inputs[inputIdx], INPUT);
            Gate* pNewInput = new Gate(newInput);
            pNewInput->setFanin(&pNewInput);
            _gateLists[_inputs[inputIdx]] = pNewInput;
            inputIdx++;
          }
          //
          // Read Outputs
          //
          else if (parLine[0].find("OUTPUT") != parLine[0].npos ) {
            // parse input signal id
            size_t idPos = parLine[0].find("(") + 1;
            string sid = parLine[0].substr(idPos, parLine[0].length() - 1 - idPos);
            // add to _outpuss
            _outputs[outputIdx] = atoi(sid.c_str());
            // add to _gateLists
            Gate newOutput(_outputs[outputIdx]);
            Gate* pNewOutput = new Gate(newOutput);
            _gateLists[_outputs[outputIdx]] = pNewOutput;
            outputIdx++;
          }
          //
          // Read BUFF
          //
          else if (parLine.size() == 3) {
            size_t faninPos = parLine[2].find("(") + 1;
            string sid = parLine[2].substr(faninPos, parLine[2].length() - 1 - faninPos);
            int faninId = atoi(sid.c_str());
            // check if Buff fanin exist in gateList
            checkFaninById(faninId);
            // check if Buff fanout (id of Buff) exist in gateList
            int gateId = atoi(parLine[0].c_str());
            // if not
            if (_gateLists.find(gateId) == _gateLists.end() ){
              Gate newGate(gateId, BUFF, &_gateLists[faninId]);
              Gate* pNewGate = new Gate(newGate);
              _gateLists[gateId] = pNewGate;
            }
            // if exist
            else {
              _gateLists[gateId]->setFanin(&_gateLists[faninId]);
              _gateLists[gateId]->setGateType(BUFF);
            }
          }
          else if (parLine.size() == 4) {
            size_t faninPos1 = parLine[2].find("(") + 1;
            string sFaninId1 = parLine[2].substr(faninPos1, parLine[2].length() - 1 - faninPos1);
            int faninId1 = atoi(sFaninId1.c_str());
            
            size_t faninPos2 = parLine[3].find(")");
            string sFaninId2 = parLine[3].substr(0, faninPos2);
            int faninId2 = atoi(sFaninId2.c_str());
            
            // check if the fanins of the gate exist

            Gate** fanin = new Gate*[2];
            fanin[0] = checkFaninById(faninId1);
            fanin[1] = checkFaninById(faninId2);
            
            // check if gate fanout (id of gate) exist in gateList
            int gateId = atoi(parLine[0].c_str());
            string type = parLine[2].substr(0, faninPos1 - 1);
            // if not
            if (_gateLists.find(gateId) == _gateLists.end() ){
              Gate newGate(gateId, enum2Str(type), fanin);
              Gate* pNewGate = new Gate(newGate);
              _gateLists[gateId] = pNewGate;
            }
            // if exist
            else {
              _gateLists[gateId]->setFanin(fanin);
              _gateLists[gateId]->setGateType(enum2Str(type));
            }
          }
        }
      }
    }
    cirFile.close();
  }
}


Gate* Circuit::checkFaninById(int id) {
  if (_gateLists.find(id) == _gateLists.end()) {
    Gate newGate(id);
    Gate* pNewGate = new Gate(newGate);
    _gateLists[id] = pNewGate;
    return pNewGate;
  }
  else return _gateLists[id];
}
