#include <bits/stdc++.h>
using namespace std;
bool isno=0;
struct Word{
	string type;
	string str;
	Word(string t,string s):type(t),str(s){};
};

vector<string> a;
vector<Word> result;

bool is_number(char a){
	return a>='0'&&a<='9';
}
bool is_smallidentifier(char a){
	return a>='a'&&a<='z';
}
bool is_bigidentifier(char a){
	return a>='A'&&a<='Z';
}
bool is_twospecial(char a){
	if(a=='>'||a=='<'||a==':') return 1;
	else return 0;
}
bool is_special(char a){
	if(a=='='||a=='+'||a=='-'||a=='*'||a=='/'||a=='#')
	return 1;
	if(a==';'||a==','||a=='.'||a=='('||a==')') 
	return 1;
	else return 0;
}

bool is_special2(string a){
	if(a=="CONST"||a=="VAR"||a=="PROCEDURE"||a=="BEGIN"||a=="END"
	||a=="ODD"||a=="IF"||a=="THEN"||a=="CALL"||a=="WHILE"||a=="DO"||a=="READ"||a=="WRITE")
	return 1;
	if(a==">="||a=="<="||a==":="||a==">"||a=="<") return 1;
	else return 0;
}

void print(string s,bool is1,bool is2,int m){
	if(is_special2(s)){
		a.push_back(s);
		result.emplace_back("Special",s);
		s.clear();
		is1=0;is2=0;
	}
	if(is1){
		string st="NUMBER "+to_string(m);
		a.push_back(st);
		result.emplace_back("NUMBER",to_string(m));
		s.clear(),is1=0;
	}
	if(is2){
		string st="IDENTIFIER "+s;
		a.push_back(st);
		result.emplace_back("IDENTIFIER",s);
		s.clear(),is2=0;
	}
}
bool is_no(string s){
	string str;
	bool isnum=0,isdir=0;
	for(int i=0;i<s.size();i++){
		if(is_number(s[i])){
			isnum=1;
			str+=s[i];
		}
		else if(is_bigidentifier(s[i])) {
			if(isnum) {
				isno=1;
				return 1;
			}
			isdir=1;
			str+=s[i];
		}
		else if(is_smallidentifier(s[i])) {
			if(isnum) {
				isno=1;
				return 1;
			}
			isdir=1;
			str+=s[i]-'a'+'A';
		}
		else if(s[i]==':'&&s[i+1]!='=') {
			isno=1;
			str.clear(),isnum=0,isdir=0;
			return 1;
		}
		else if(s[i]==' '||s[i]=='	'){
			str.clear(),isnum=0,isdir=0;
		}
		else if(is_special(s[i])||is_twospecial(s[i])) {
			
			str.clear(),isnum=0,isdir=0;
		}
		else if(is_special2(str)) {
			str.clear(),isnum=0,isdir=0;
		}
		else {
			isno=1;
			str.clear(),isnum=0,isdir=0;
			return 1;
		}
		if(str.size()>10) {
			isno=1;
			return 1;
		}
	}
	str.clear();
	return 0;
}
void change(string s){
	string str;
	int l=s.size();
	bool is1=0,is2=0;
	int m=0;
	for(int i=0;i<s.size();i++){
		if((s[i]==' '||s[i]=='	')) {
			if(is2) print(str,is1,is2,m),str.clear(),is2=0;
			if(is1) print(str,is1,is2,m),str.clear(),is1=0,m=0;
		}
		if(is_special(s[i])){
			if(is2) print(str,is1,is2,m),str.clear(),is2=0;
			if(is1) print(str,is1,is2,m),str.clear(),is1=0,m=0;
			string st;
			st+=s[i];
			a.push_back(st);
			result.emplace_back("Special",st);
		}
		if(is_twospecial(s[i])){
			if(is2)print(str,is1,is2,m),str.clear(),is2=0;
			if(is1)print(str,is1,is2,m),str.clear(),is1=0,m=0;
			if(s[i+1]=='=') 
			{
				string st1;
				st1+=s[i];
				st1+=s[i+1];
				a.push_back(st1);
				result.emplace_back("Special",st1);
				i++;
			}
			if((s[i]=='>'&&s[i+1]!='=')||(s[i]=='<'&&s[i+1]!='=')) {
				string st2;
				st2+=s[i];
				a.push_back(st2);
				result.emplace_back("Special",st2);	
			}
		}
		if(is_number(s[i])){
			is1=1;
			if(is2) {
				str+=s[i];
				is1=0;
			}
			else{
				m*=10;
				m+=s[i]-'0';
			} 
		}
		if(is_bigidentifier(s[i])) {
			if(is1) print(str,is1,is2,m),str.clear(),is1=0;
			is2=1;
			str+=s[i];
		}
		if(is_smallidentifier(s[i])) {
			if(is1) print(str,is1,is2,m),str.clear(),is1=0;
			is2=1;
			str+=s[i]-'a'+'A';
		}
	}
	if(is2)	print(str,is1,is2,m),str.clear(),is2=0;
	if(is1)	print(str,is1,is2,m),str.clear(),is1=0;
}


