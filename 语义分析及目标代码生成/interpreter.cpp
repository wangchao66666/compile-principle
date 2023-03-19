#include <bits/stdc++.h>
#include "PLPLO.cpp"
using namespace std;
vector<int> Data;
vector<Objectcode> Obcode;
int SP,BP,PC;
int findLevel(int l){
	int a=BP;
	while(l>0){
		a=Data[a]; 
		l--;
	}
//	cout<<a<<"sss"<<endl; 
	return a;
}

int main(){
//	freopen("p.txt","w",stdout); 
	string str;
	ifstream readfile("program.code");
	if(!readfile.is_open()){
		return -1;
	}
	while(getline(readfile,str,'\n')){
		string f;
		int l,a;
		istringstream is(str);
		is>>f>>l>>a;
		Obcode.emplace_back(f,l,a);
	}
//	for(int i=0;i<Obcode.size();i++) cout<<Obcode[i].f<<Obcode[i].l<<Obcode[i].a<<endl;

	SP=0,BP=0,PC=0;
	while(PC!=Obcode.size()-1){
		Objectcode inx=Obcode[PC++];
		if(inx.f=="LIT") Data.push_back(inx.a),SP=Data.size()-1;
		else if(inx.f=="LOD") Data.push_back(Data[findLevel(inx.l)+inx.a]),SP=Data.size()-1;
		else if(inx.f=="STO") {
			SP=Data.size()-1;
			Data[findLevel(inx.l)+inx.a]=Data[SP];
			Data.pop_back();
			SP=Data.size()-1;
		}
		else if(inx.f=="CAL"){
			Data.push_back(findLevel(inx.l));
			SP++;
			Data.push_back(BP);
			Data.push_back(PC);
			BP=SP;
			PC=inx.a;
			SP=Data.size()-1;
		}
		else if(inx.f=="INT") {
			for(int i=0;i<inx.a;i++) Data.push_back(0);
			SP=Data.size()-1;
		}
		else if(inx.f=="JMP") PC=inx.a;
		else if(inx.f=="JPC") {
			SP=Data.size()-1;
			if(Data[SP]==0){
				PC=inx.a;
				Data.pop_back();
				SP=Data.size()-1;
			}
		}
		else if(inx.f=="OPR"){
			//for(int i=0;i<Data.size();i++) cout<<Data[i]<<"  sass"<<endl;
			if(inx.a==20){
				int ans=Data[SP];
				Data.pop_back();
				Data.push_back(0-ans);
				SP=Data.size()-1;
			}
			else if(inx.a==1) {
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x+y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==2) {
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x-y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==3){
				SP=Data.size()-1;
				int x=Data[SP];
				int y=Data[SP-1];
				int m=x*y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==4) {
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x/y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==5) {
				SP=Data.size()-1;
				int x=Data[SP]%2;
				Data.pop_back();
				Data.push_back(x);
				SP=Data.size()-1;
			}
			else if(inx.a==6) {
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x==y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==7) {
				SP=Data.size()-1;
				int x=Data[SP];
				int y=Data[SP-1];
				int m=x!=y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==8){
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x<y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			} 
			else if(inx.a==9){
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x<=y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			} 
			else if(inx.a==10){
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x>y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			} 
			else if(inx.a==11) {
				SP=Data.size()-1;
				int x=Data[SP-1];
				int y=Data[SP];
				int m=x>=y;
				Data.pop_back();
				Data.pop_back();
				Data.push_back(m);
				SP=Data.size()-1;
			}
			else if(inx.a==12) {
				SP=Data.size()-1;
				int x;
				cin>>x;
				Data.push_back(x);
				SP=Data.size()-1;	
			}
			else if(inx.a==13) {
				SP=Data.size()-1;
				cout<<Data[SP]<<endl;
				Data.pop_back();
				SP=Data.size()-1;
			}
			else if(inx.a==0) {
				SP=Data.size()-1; 
				SP=BP-1;
				BP=Data[SP+2];
				PC=Data[SP+3];
				SP=Data.size()-1;
			}
		}
		else {
			return -1;
		}
	}
	return 0;
}
