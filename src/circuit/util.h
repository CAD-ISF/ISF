#ifndef UTIL_H_
#define UTIL_H_

#include  <vector>
#include  <string>

using namespace std;


void strParse(string str, vector<string> &parStr) {
  size_t fSpc = 0, fnSpc = 0;   // First space pos, First not space pos
  while( fnSpc != str.npos) {
    str = str.substr(fnSpc);
    fSpc = str.find_first_of(" ");
    fnSpc = str.find_first_not_of(" ");
    if (fSpc == str.npos) {
      fSpc = str.length();
    }
    parStr.push_back(str.substr(0, fSpc - fnSpc));
    str = str.substr(fSpc);
    fnSpc = str.find_first_not_of(" ");
  }
}

GateType enum2Str(string type) {
  if (type == "INPUT") {
    return INPUT;
  }
  else if (type == "BUFF") {
    return BUFF;
  }
  else if (type == "NOT") {
    return NOT;
  }
  else if (type == "AND") {
    return AND;
  }
  else if (type == "NAND") {
    return NAND;
  }
  else if (type == "OR") {
    return OR;
  }
  else if (type == "NOR") {
    return NOR;
  }
  else if (type == "XOR") {
    return XOR;
  }
  else if (type == "NXOR") {
    return NXOR;
  }
}
#endif
