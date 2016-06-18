#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <stdlib.h>
#include "circuit.h"
#include "util.h"

using namespace std;

Circuit::Circuit(){
	_inputs = NULL;
	_outputs = NULL;
	_topoorder = new vector<int>;
}

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
						_isize = atoi(parLine[1].c_str());
					}
					// allocate outputs
					else if (parLine[2] == "outputs") {
						_outputs = new int[atoi(parLine[1].c_str())];
						_osize = atoi(parLine[1].c_str());
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
						Gate* pNewInput = new Gate(_inputs[inputIdx], INPUT);
						pNewInput->addFanin(pNewInput);
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
						Gate* pNewOutput = new Gate(_outputs[outputIdx]);
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
							Gate* pNewGate = new Gate(gateId, BUFF);
							_gateLists[gateId] = pNewGate;
						}
						// if exist
						else {
							_gateLists[gateId]->setGateType(BUFF);
						}
						// manage fanin/fanout
						_gateLists[gateId]->addFanin(_gateLists[faninId]);
						_gateLists[faninId]->addFanout(_gateLists[gateId]);
					}
					else if (parLine.size() == 4) {
						size_t faninPos1 = parLine[2].find("(") + 1;
						string sFaninId1 = parLine[2].substr(faninPos1, parLine[2].length() - 1 - faninPos1);
						int faninId1 = atoi(sFaninId1.c_str());

						size_t faninPos2 = parLine[3].find(")");
						string sFaninId2 = parLine[3].substr(0, faninPos2);
						int faninId2 = atoi(sFaninId2.c_str());

						// check if the fanins of the gate exist
						checkFaninById(faninId1);
						checkFaninById(faninId2);

						// check if gate fanout (id of gate) exist in gateList
						int gateId = atoi(parLine[0].c_str());
						string type = parLine[2].substr(0, faninPos1 - 1);
						// if not
						if (_gateLists.find(gateId) == _gateLists.end() ){
							Gate* pNewGate = new Gate(gateId, enum2Str(type));
							_gateLists[gateId] = pNewGate;
						}
						// if exist
						else {
							_gateLists[gateId]->setGateType(enum2Str(type));
						}
						_gateLists[gateId]->addFanin(_gateLists[faninId1]);
						_gateLists[faninId1]->addFanout(_gateLists[gateId]);
						_gateLists[gateId]->addFanin(_gateLists[faninId2]);
						_gateLists[faninId2]->addFanout(_gateLists[gateId]);
					}
				}
			}
		}
		cirFile.close();
	}
	for(int i=0;i<_osize;i++){
		topodfs(_outputs[i]);
	}
}

Circuit::~Circuit() {
	if (_inputs != NULL) delete[] _inputs;
	if (_outputs != NULL) delete[] _outputs;
	for (map<int, Gate*>::iterator it = _gateLists.begin(); it != _gateLists.end(); ++it)
		if (it->second != NULL) delete it->second;
	delete _topoorder;
}

map< int, Gate* > Circuit::getGateLists() {
	return _gateLists;
}

void Circuit::checkFaninById(int id) {
	if (_gateLists.find(id) == _gateLists.end()) {
		Gate* pNewGate = new Gate(id);
		_gateLists[id] = pNewGate;
	}
}

void Circuit::checkId(int id) {
	cout << "Gate: " << id 
		<< "\tType: " << _gateLists[id]->getGateType();

	cout << "\nFanin: ";
	size_t s = _gateLists[id]->getFanin()->size();
	for (size_t i = 0; i < s; i++)
		cout << (*(_gateLists[id]->getFanin()))[i]->getId() << ' ';

	cout << "\nFanout: ";
	s = _gateLists[id]->getFanout()->size();
	for (size_t i = 0; i < s; i++)
		cout << (*(_gateLists[id]->getFanout()))[i]->getId() << ' ';

	cout << "\n\n";
}

void Circuit::checkGateLists() {
	for (map< int, Gate* >::iterator it = _gateLists.begin(); it != _gateLists.end(); ++it)
		checkId(it->first);
}

void Circuit::outputDFS(int id, map<int, bool>& out) {
	if (_gateLists[id]->getFanout()->size() == 0){
		//cerr << id << " out!!!\n";
		out[id] = true;
		return;
	}
	else {
		outputDFS((*(_gateLists[id]->getFanout()))[0]->getId(), out);
		if (_gateLists[id]->getFanout()->size() > 1) 
			outputDFS((*(_gateLists[id]->getFanout()))[1]->getId(), out);
	}
}

int* Circuit::getPI(){
	return _inputs;
}

int* Circuit::getPO(){
	return _outputs;
}

int Circuit::getosize(){
	return _osize;
}

int Circuit::getisize(){
	return _isize;
}

void Circuit::topodfs(int a){
	if(_gateLists[a]->mark==0){
		if(_gateLists[a]->getGateType()!=0){
			for(int i=0;i<(_gateLists[a]->getFanin())->size();i++)
				topodfs((*(_gateLists[a]->getFanin()))[i]->getId());
		}
		_gateLists[a]->mark=1;
		_topoorder->push_back(a);
	}
}

vector<int>* Circuit::gettopo(){
	return _topoorder;
}

unsigned Circuit::simulate(unsigned* input, int faultgate, FaultType faulttype){
	unsigned answer = 0;
	unsigned temp;
	int picount = 0;
	int s = _topoorder->size();
	int type;
	unsigned value1=0;
	unsigned value2=0;
	for (int i=0;i<s;i++) {
		type = _gateLists[(*_topoorder)[i]]->getGateType();
		for (int j=0;j<(_gateLists[(*_topoorder)[i]]->getFanin())->size();j++) {
			if (j==0&&type!=INPUT)
				value1 = (*(_gateLists[(*_topoorder)[i]]->getFanin()))[j]->value;
			if (j==1&&type!=INPUT)
				value2 = (*(_gateLists[(*_topoorder)[i]]->getFanin()))[j]->value;
		}
		if (type==INPUT) {
			_gateLists[(*_topoorder)[i]]->value = input[picount];
			//cout<<(*_topoorder)[i]<<" "<<input[picount]<<endl;
			picount++;
			if ((*_topoorder)[i]==faultgate) {
				if (faulttype ==SA0)
					_gateLists[(*_topoorder)[i]]->value = 0;
				else if (faulttype ==SA1)
					_gateLists[(*_topoorder)[i]]->value = 4294967295;
				else if (faulttype ==NEG)
					_gateLists[(*_topoorder)[i]]->value = 4294967295-_gateLists[(*_topoorder)[i]]->value;
			}
		}
		else {
			if (type==BUFF) _gateLists[(*_topoorder)[i]]->value = value1;
			else if (type==NOT) _gateLists[(*_topoorder)[i]]->value = 4294967295-value1;
			else if (type==AND) _gateLists[(*_topoorder)[i]]->value = value1 & value2;
			else if (type==NAND) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 & value2);
			else if (type==OR) _gateLists[(*_topoorder)[i]]->value = value1 | value2;
			else if (type==NOR) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 | value2);
			else if (type==XOR) _gateLists[(*_topoorder)[i]]->value = value1 ^ value2;
			else if (type==NXOR) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 ^ value2);	
			if((*_topoorder)[i]==faultgate) {
				if (faulttype==SA0) _gateLists[(*_topoorder)[i]]->value = 0;
				else if (faulttype==SA1) _gateLists[(*_topoorder)[i]]->value = 4294967295;
				else if (faulttype==NEG) _gateLists[(*_topoorder)[i]]->value = 4294967295-_gateLists[(*_topoorder)[i]]->value;
				else if (faulttype==RDOB_AND) _gateLists[(*_topoorder)[i]]->value = value1 & value2;
				else if (faulttype==RDOB_NAND) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 & value2);
				else if (faulttype==RDOB_OR) _gateLists[(*_topoorder)[i]]->value = value1 | value2;
				else if (faulttype==RDOB_NOR) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 | value2);
				else if (faulttype==RDOB_XOR) _gateLists[(*_topoorder)[i]]->value = value1 ^ value2;
				else if (faulttype==RDOB_NXOR) _gateLists[(*_topoorder)[i]]->value = 4294967295-(value1 ^ value2);
				else if (faulttype==RDOB_NOT) _gateLists[(*_topoorder)[i]]->value = 4294967295-value1;
				else if (faulttype==RDOB_BUFF) _gateLists[(*_topoorder)[i]]->value = value1;
			}
		}
		//cout<<(*_topoorder)[i]<<" "<<_gateLists[(*_topoorder)[i]]->value<<" "<<value1<<" "<<value2<<endl;	
	}
	for (int i=0;i<_osize;i++) {
		temp = _gateLists[_outputs[i]]->value;
		answer += temp;
	}
	return answer;
}


