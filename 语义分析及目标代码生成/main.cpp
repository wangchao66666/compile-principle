#include <bits/stdc++.h>
#include "PLPLO.cpp"
using namespace std;
void output(Tree *root){
	cout<<root->str;
	int m=root->child.size();
	if(m!=0)
	{
		cout<<"(";
		for(int i=0;i<m;i++){
			output(root->child[i]);
			if(i+1<m) cout<<",";
		}
		cout<<")";
	}
	
}
void codeput(){
	for(int i=0;i<code.size();i++){
		cout<<code[i].f<<" "<<code[i].l<<" "<<code[i].a<<endl; 
	}
}
int main(){
	freopen("program.code","w",stdout);
//	freopen("out1.txt","w",stdout);
	string str;
	while(getline(cin,str)){
		is_no(str);
		change(str);
	}
	if(isno) return -1;
	else{
//		for(int j=0;j<result.size();j++) cout<<result[j].str<<endl;
		Tree *root=Parser();
		if(is_error==1) return -1;
		else codeput();
	}
}
