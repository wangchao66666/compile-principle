#include <bits/stdc++.h>
#include "PLLexiacl.cpp"
using namespace std;
bool is_error=0;
int i=0;
int k=0;
int nowlevel=0;
int Dx=3;			


class Tree{
public:
	Tree* root;
	vector<Tree *> child;
	string str;
public:
	Tree(string ss){
		this->str=ss,this->root=NULL,this->child.clear();
	}
	Tree *newNode(string s){
		Tree* Node=new Tree(s);
		Node->root=this;
		this->child.push_back(Node);
		return Node;
	}	
};

struct Symbol{
    string name;
    string kind;
    int value;
    int level;
    int addr;
    Symbol(){}
	Symbol(string n,string k,int v,int l,int a):name(n),kind(k),value(v),level(l),addr(a){}
};
vector <Symbol> table;
int Find(string name){
	for(int j=table.size()-1;j>=0;j--){
		if(table[j].name==name) return j;
	}
	return	-1;
}

void Symbol_input(string name,string kind,int value,int thislevel){
	int x=Find(name);
	if(x!=-1&&table[x].level==thislevel) {
		is_error=1;
		return;
	}
	if(kind=="CONSTANT") table.emplace_back(name,kind,value,0,0);
	else if(kind=="VARIABLE") table.emplace_back(name,kind,0,thislevel,Dx++);
	else if(kind=="PROCEDURE") table.emplace_back(name,kind,0,thislevel,0);
	else{
		is_error=1;
		return;
	}
}


struct Objectcode{
    string f;
    int l;
    int a;
	Objectcode(string _f,int _l,int _a):f(_f),l(_l),a(_a){}
};
vector<Objectcode> code;
void Objectcodeinput(string f,int l,int a){
	code.emplace_back(f,l,a);
}

Tree* Parser();
void Subprog(Tree *Node);
void Const(Tree *Node);
void Var(Tree *Node);
void Procedure(Tree *Node);
void ProcedureHead(Tree *Node);
void Sentence(Tree *Node);
void Assignment(Tree *Node);
void Epsilon(Tree *Node);
void Combined(Tree *Node);
void Writesentence(Tree *Node);
void Readsentence(Tree *Node);
void Callsentence(Tree *Node);
void Whilesectence(Tree *Node);
void Condition(Tree *Node);
void Expression(Tree *Node);
void Factor(Tree *Node);
void Item(Tree *Node);
void IF(Tree *Node);
void Empty(Tree *Node);

vector<pair<int,int>>CallId;
Tree* Parser(){
	if(result.size()==0) {
		is_error=1;
		return NULL;
	}
	Tree *ROOT=new Tree("PROGRAM");	
	
	Subprog(ROOT);
	if(result[i].str=="."){
		ROOT->newNode("."),i++;
	}
	else is_error=1;
	if(i<result.size()){
		is_error=1;
	}
	return ROOT;
	
}

void Subprog(Tree *Node){
	Dx=3;
	int psize=table.size();
	if(psize==0){
        Symbol_input("","CONSTANT",0,0);
        psize++;
    }
    table[psize-1].addr=code.size();
	Objectcodeinput("JMP",0,0);
	Tree *newchild=Node->newNode("SUBPROG"); 
	if(result[i].str=="CONST") Const(newchild);
	if(result[i].str=="VAR")  Var(newchild);
	if(result[i].str=="PROCEDURE") Procedure(newchild);
	code[table[psize-1].addr].a=code.size();
    table[psize-1].addr=code.size();
    for(int j=0;j<CallId.size();j++){
    	if(CallId[i].first==psize) code[CallId[i].second].a=table[psize].addr;
	}
    Objectcodeinput("INT",0,Dx);
	Sentence(newchild);
	Objectcodeinput("OPR",0,0);
}

void Const(Tree *Node){
	Tree *thisNode=Node->newNode("CONSTANTDECLARE");
	thisNode->newNode("CONST");
	i++;
	if(result[i].type=="IDENTIFIER"&&result[i+1].str=="="&&result[i+2].type=="NUMBER") {
		Tree* newchild=thisNode->newNode("CONSTANTDEFINE");
		newchild->newNode(result[i].str);
		newchild->newNode(result[i+1].str);
		newchild->newNode(result[i+2].str);
		Symbol_input(result[i].str,"CONSTANT",atoi(result[i+2].str.c_str()),k);
		i+=3;
	}
	else{
		is_error=1;
		return;	
	}
	while(result[i].str==","){
		thisNode->newNode("COMMA");
		i++; 
		if(result[i].type=="IDENTIFIER"&&result[i+1].str=="="&&result[i+2].type=="NUMBER") {
			Tree* newchild=thisNode->newNode("CONSTANTDEFINE");
			newchild->newNode(result[i].str);
			newchild->newNode(result[i+1].str);
			newchild->newNode(result[i+2].str);
			Symbol_input(result[i].str,"CONSTANT",atoi(result[i+2].str.c_str()),k);
			i+=3;
		}
		else {
			is_error=1;
			return;	
		}
	}
	if(result[i].str==";") thisNode->newNode(";"),i++;
	else {
		is_error=1;
		return;	
	}
}

void Var(Tree *Node){
	Tree *thisNode=Node->newNode("VARIABLEDECLARE");
	thisNode->newNode("VAR"),i++;
	if(result[i].type=="IDENTIFIER"){
		thisNode->newNode(result[i].str);
		Symbol_input(result[i].str,"VARIABLE",0,k);
		i++;
	}
	else{
		is_error=1;
		return;
	}
	while(result[i].str==","){
		thisNode->newNode("COMMA");i++;
		if(result[i].type=="IDENTIFIER"){
			thisNode->newNode(result[i].str);
			Symbol_input(result[i].str,"VARIABLE",0,k);
			i++;
		}
		else{
			is_error=1;
			return;
		}
	}
	if(result[i].str==";"){
		thisNode->newNode(";"),i++;
	}
	else{
		is_error=1;
		return;
	}
}
void ProcedureHead(Tree *Node){
	Tree *Head=Node->newNode("PROCEDUREHEAD");
	Head->newNode("PROCEDURE");i++;
	if(result[i].type=="IDENTIFIER"){
		Head->newNode(result[i].str);
		Symbol_input(result[i].str,"PROCEDURE",0,k);
		i++;
	}
	else{
		is_error=1;
		return;
	}
	if(result[i].str==";"){
		Head->newNode(";");
	    i++;
	}
	else{
		is_error=1;
		return;
	}
} 
void Procedure(Tree *Node){
	Tree *thisNode=Node->newNode("PROCEDUREDECLARE");
	ProcedureHead(thisNode);
	k++;
	if(k>3){
		k=0; 
		is_error=1;
		return;
	}
	int psize=table.size();
	int pDx=Dx;
	Subprog(thisNode);
	k--;
	table.resize(psize);
	Dx=pDx;
	if(result[i].str==";"){
		thisNode->newNode(";");
		i++;
	}else{
		is_error=1;
		return;
	}
	while(result[i].str=="PROCEDURE"){
		Procedure(thisNode);
	}
}

void Sentence(Tree *Node){
	Tree *newNode=Node->newNode("SENTENCE");
	if(i+1<result.size()&&result[i+1].str==":=") Assignment(newNode);
	else if(result[i].str=="IF") IF(newNode);
	else if(result[i].str=="WHILE") Whilesectence(newNode);
	else if(result[i].str=="CALL")  Callsentence(newNode);
	else if(result[i].str=="READ") Readsentence(newNode);
	else if(result[i].str=="WRITE") Writesentence(newNode);
	else if(result[i].str=="BEGIN") Combined(newNode) ;
	else Empty(newNode);
}
void Empty(Tree *Node){
	Node->newNode("EMPTY");
}
void Assignment(Tree *Node){
	Tree *thisNode=Node->newNode("ASSIGNMENT");
	if(result[i].type=="IDENTIFIER") thisNode->newNode(result[i].str);
	else{
		is_error=1;
		return;
	}
	int x=Find(result[i].str);
	if(x==-1||table[x].kind!="VARIABLE"){
		is_error=1;
		return;
	}
	i++;
	if(result[i].str==":=") thisNode->newNode(result[i].str);
	else{
		is_error=1;
		return;
	}
	i++;
	Expression(thisNode);
	int y=abs(table[x].level-k);
	Objectcodeinput("STO",y,table[x].addr);
}

void Expression(Tree *Node){
	Tree *newchild=Node->newNode("EXPRESSION");
	int operation;
	bool is_fu=0;
	if(result[i].str=="+"||result[i].str=="-"){
		newchild->newNode(result[i].str);
		if(result[i].str=="-") is_fu=1,operation=20;
		i++;
	}
	
	Item(newchild);
	if(is_fu) Objectcodeinput("OPR",0,operation);
	while(result[i].str=="+"||result[i].str=="-"){
		newchild->newNode(result[i].str);
		if(result[i].str=="+") operation=1;
		else if(result[i].str=="-") operation=2;
		i++;
		Item(newchild);
		Objectcodeinput("OPR",0,operation);
	}
}

void Item(Tree *Node){
	Tree *newchild=Node->newNode("ITEM");
	Factor(newchild);
	int operation;
	while(result[i].str=="*"||result[i].str=="/"){
		newchild->newNode(result[i].str);
		operation=(result[i].str=="*")? 3:4;
		i++;
		Factor(newchild); 
		Objectcodeinput("OPR",0,operation);
	}
}

void Factor(Tree *Node){
	Tree *newchild=Node->newNode("FACTOR");
	if(result[i].type=="IDENTIFIER"){
		newchild->newNode(result[i].str);
		int x=Find(result[i].str);
		if(x==-1){
			is_error=1;
			return; 
		}
		if(table[x].kind=="CONSTANT") Objectcodeinput("LIT",0,table[x].value);
		else if(table[x].kind=="VARIABLE") Objectcodeinput("LOD",abs(table[x].level-k),table[x].addr);
		else{
			is_error=1;
			return;
		}
		i++; 
	}
	else if(result[i].type=="NUMBER"){
		newchild->newNode(result[i].str);
		Objectcodeinput("LIT",0,atoi(result[i].str.c_str()));
		i++;
	}
	else if(result[i].str=="("){
		newchild->newNode("LP");
		i++;
		Expression(newchild);
		if(result[i].str==")"){
			newchild->newNode("RP");
			i++;
		}
		else{
			is_error=1;
			return;
		}
	}
	else{
		is_error=1;
		return;
	}
}
void Condition(Tree *Node){
	Tree *child=Node->newNode("CONDITION");
	int operation;
	if(result[i].str=="ODD"){
		child->newNode("ODD");
		i++;
		Expression(child);
		Objectcodeinput("OPR",0,5);
	}
	else{
		Expression(child);
		if(result[i].str=="="||result[i].str=="#"||result[i].str=="<"||result[i].str=="<="||result[i].str==">"||result[i].str==">="){
			child->newNode(result[i].str);
			if(result[i].str=="=") operation=6;
			else if(result[i].str=="#") operation=7;
			else if(result[i].str=="<") operation=8;
			else if(result[i].str=="<=") operation=9;
			else if(result[i].str==">") operation=10;
			else if(result[i].str==">=") operation=11;
			i++;	
		}
		else{
			is_error=1;
			return; 
		}
		Expression(child);
		Objectcodeinput("OPR",0,operation);
	}
}
void IF(Tree* Node){
	Tree *thisNode=Node->newNode("IFSENTENCE");
	Tree *newchild=thisNode->newNode("IF");
	i++;
	Condition(thisNode);
	Objectcodeinput("JPC",0,0);
	int psize=code.size()-1;
	if(result[i].str=="THEN"){
		thisNode->newNode("THEN");
		i++;
	}
	else{
		is_error=1;
		return;	
	}
	Sentence(thisNode);
	code[psize].a=code.size();
}


void Whilesectence(Tree *Node){
	Tree *thisNode=Node->newNode("WHILESENTENCE");
	Tree *newchild=thisNode->newNode("WHILE");
	i++;
	int pcsize=code.size();
	Condition(thisNode);
	Objectcodeinput("JPC",0,0);
	int pdsize=code.size()-1;
	if(result[i].str=="DO"){
		thisNode->newNode("DO");i++;
	}
	else{
		is_error=1;
		return;
	}
	Sentence(thisNode);
	Objectcodeinput("JMP",0,pcsize);
	code[pdsize].a=code.size();
}

void Callsentence(Tree *Node){
	Tree *thisNode=Node->newNode("CALLSENTENCE");
	thisNode->newNode("CALL");
	i++;
	if(result[i].type=="IDENTIFIER"){
		thisNode->newNode(result[i].str);
		int x=Find(result[i].str);
		if(x==-1||table[x].kind!="PROCEDURE"){
			is_error=1;
			return;
		}
		if(table[x].addr==-1) CallId.push_back({x,code.size()});
		Objectcodeinput("CAL",abs(k-table[x].level),table[x].addr);
		i++;
	}
	else{
		is_error=1;
		return;
	}
}

void Readsentence(Tree* Node){
	Tree *thisNode=Node->newNode("READSENTENCE");
	thisNode->newNode("READ");
	i++;
	if(result[i].str=="("){
		thisNode->newNode("LP");
		i++;
		if(result[i].type=="IDENTIFIER"){
			thisNode->newNode(result[i].str);
		}
		else {
			is_error=1;
			return;
		}
		int x=Find(result[i].str);
		if(x==-1||table[x].kind!="VARIABLE"){
			is_error=1;
			return;
		}
		Objectcodeinput("OPR",0,12);
		Objectcodeinput("STO",abs(k-table[x].level),table[x].addr);
		i++;
		while(result[i].str==","){
			thisNode->newNode("COMMA"),i++;
			if(result[i].type=="IDENTIFIER"){
				thisNode->newNode(result[i].str);
			}
			else {
				is_error=1;
				return;
			}
			int x=Find(result[i].str);
			if(x==-1||table[x].kind!="VARIABLE"){
				is_error=1;
				return;
			}
			Objectcodeinput("OPR",0,12);
			Objectcodeinput("STO",abs(k-table[x].level),table[x].addr);
			i++;
		}
		if(result[i].str==")"){
			thisNode->newNode("RP");i++;
		}
		else {
			is_error=1;
			return;
		}
	}
	else {
		is_error=1;
		return;
	}
}

void Writesentence(Tree *Node){
	Tree *thisNode=Node->newNode("WRITESENTENCE");
	thisNode->newNode("WRITE"),i++;
	if(result[i].str=="("){
		thisNode->newNode("LP");
		i++;
		if(result[i].type=="IDENTIFIER"){
			thisNode->newNode(result[i].str);
		}
		else {
			is_error=1;
			return;
		}
		int x=Find(result[i].str);
		if(x==-1) {
			is_error=1;
			return;
		}
		if(table[x].kind=="CONSTANT"){
			Objectcodeinput("LIT",0,table[x].value);
			Objectcodeinput("OPR",0,13);
		}
		else if(table[x].kind=="VARIABLE"){
			Objectcodeinput("LOD",abs(k-table[x].level),table[x].addr);
			Objectcodeinput("OPR",0,13);
		}
		else{
			is_error=1;
			return;
		}
		i++;
		while(result[i].str==","){
			thisNode->newNode("COMMA"),i++;
			if(result[i].type=="IDENTIFIER"){
				thisNode->newNode(result[i].str);
			}
			else {
				is_error=1;
				return;
			}
			int x=Find(result[i].str);
			if(x==-1) {
				is_error=1;
				return;
			}
			if(table[x].kind=="CONSTANT"){
				Objectcodeinput("LIT",0,table[x].value);
				Objectcodeinput("OPR",0,13);
			}
			else if(table[x].kind=="VARIABLE"){
				Objectcodeinput("LOD",abs(k-table[x].level),table[x].addr);
				Objectcodeinput("OPR",0,13);
			}
			else{
				is_error=1;
				return;
			}
			i++;
		}
		if(result[i].str==")"){
			thisNode->newNode("RP");i++;
		}
		else {
			is_error=1;
			return;
		}
	}
	else {
		is_error=1;
		return;
	}
}

void Combined(Tree *Node){
	Tree *thisNode=Node->newNode("COMBINED");
	thisNode->newNode("BEGIN");
	i++;
	Sentence(thisNode);
	while(result[i].str==";"){
		thisNode->newNode(";");
		i++;
		Sentence(thisNode);
	}
	if(result[i].str=="END"){
		thisNode->newNode("END");
		i++;
	}
	else{
		is_error=1;
		return;
	}
}


