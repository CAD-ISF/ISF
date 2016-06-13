#include<iostream>
#include <IFS/ifs.h>
#include  <stdlib.h>
#include  <circuit/circuit.h>

using namespace std;

void printfec(vector<vector<int> >&);

int main(int argc, const char** argv) {
    cout << "Enter ISF Project" << endl;
    Circuit c; 
    c.loadCircuit(argv[1]);
    c.checkGateLists();
	IFS ifs;
	ifs.loadFault(argv[2]);
	//ifs.faultOut(c);
	int faultsize = (ifs.getfGId())->size();
	unsigned answer;
	unsigned* simu = new unsigned[faultsize];
	for(int i=0;i<(c.gettopo())->size();i++){
		//cout<<(*(c.gettopo()))[i]<<" ";
	}
	//cout<<endl<<(c.gettopo())->size()<<endl;
	unsigned* pattern = new unsigned[c.getisize()];
	for(int i=0;i<c.getisize();i++){
		pattern[i] = rand()%4294967295;
	}
	for(int i=0;i<faultsize;i++){
		answer = c.simulate(pattern, (*ifs.getfGId())[i] ,(*ifs.getfTList())[i]);
		simu[i] = answer;
	}
	for(int i=0;i<faultsize;i++){
		cout<<endl<<"["<<(*ifs.getfId())[i]<<"] = "<<simu[i];
	}
	cout<<endl;
//first-time grouping
	vector<vector<int> > fecgroup;
	MyHash* myhash = new MyHash(1024);
	for(int i=0;i<faultsize;i++){
		myhash->check(i+1,simu[i],simu);
	}
	for(int i=0;i<1024;i++){
		if((*myhash)[i].size()!=0){
			for(int j=0;j<(*myhash)[i].size();j++){
				vector<vector<int> > f = (*myhash)[i];
				if(f[j].size()>1)
					fecgroup.push_back(f[j]);
			}
		}
	}
	//cout<<"first : "<<endl;
	//printfec(fecgroup);
	unsigned oldsize;
	cout<<"fecgroup size = "<<fecgroup.size()<<endl;
	oldsize = fecgroup.size();
//second time
  	int count=0;
  while(count!=50){
	for(int i=0;i<c.getisize();i++){
		pattern[i] = rand()%4294967295;
	}
	vector<vector<int> > temp;
	bool same_answer;
	for(int i=0;i<fecgroup.size();i++){
		if((fecgroup[i]).size()!=1){
			temp.clear();
			for(int j=0;j<(fecgroup[i]).size();j++)
				simu[(fecgroup[i])[j]-1] = c.simulate(pattern, (*ifs.getfGId())[(fecgroup[i])[j]-1] ,(*ifs.getfTList())[(fecgroup[i])[j]-1]);
			for(int j=0;j<(fecgroup[i]).size();j++){
				same_answer = false;
				for(int k=0;k<temp.size();k++){
					vector<int>* f=&(temp[k]);
         			if(simu[(*f)[0]-1]==simu[(fecgroup[i])[j]-1]){
						same_answer=true;
						f->push_back((fecgroup[i])[j]);
						break;
					}
				}
				if(same_answer==false){
         			vector<int> newvector;
					newvector.push_back((fecgroup[i])[j]);
					temp.push_back(newvector);
      			}
			}
			for(int k=0;k<temp.size();k++){
				if(k==0)
					fecgroup[i] = temp[k];
				else if(temp[k].size()>1)
					fecgroup.push_back(temp[k]);
			}
		}		
	}
	if(oldsize == fecgroup.size())
		count++;
	cout<<"fecgroup size = "<<fecgroup.size()<<endl;
	oldsize = fecgroup.size();
  }
	//cout<<"second : "<<endl;
	printfec(fecgroup);
	
	
	delete [] pattern;
	delete [] simu;
}

void printfec(vector<vector<int> >& a){
	cout<<"fec group : "<<endl;
	int count=0;
	for(int i=0;i<a.size();i++){
		cout<<"["<<i+1<<"] = ";
		for(int j=0;j<(a[i]).size();j++){
			cout<<"  "<< (a[i])[j];
		}
		if((a[i]).size()==1)
			count++;
		cout<<endl;
	}
	cout<<"group count = "<<count<<endl;
}
