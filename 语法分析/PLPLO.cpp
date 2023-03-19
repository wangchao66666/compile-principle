#include <bits/stdc++.h>
#include "PLLexiacl.cpp"
using namespace std;
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

bool is_error=0;
int i=0;
int k=0;
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
	Tree *newchild=Node->newNode("SUBPROG"); 
	if(result[i].str=="CONST") Const(newchild);
	if(result[i].str=="VAR")  Var(newchild);
	if(result[i].str=="PROCEDURE") Procedure(newchild);
	Sentence(newchild);
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
	k++;
	if(k>3){
		is_error=1;
		return;
	}
	ProcedureHead(thisNode);
	Subprog(thisNode);
	k--;
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
	if(result[i].type=="IDENTIFIER"&&result[i+1].str==":="){
		thisNode->newNode(result[i].str);
		thisNode->newNode(result[i+1].str),i+=2;
	}
	else{
		is_error=1;
		return;
	}
	Expression(thisNode); 
}

void Expression(Tree *Node){
	Tree *newchild=Node->newNode("EXPRESSION");
	if(result[i].str=="+"||result[i].str=="-"){
		newchild->newNode(result[i].str),i++;
	}
	Item(newchild);
	while(result[i].str=="+"||result[i].str=="-"){
		newchild->newNode(result[i].str),i++;
		Item(newchild);
	}
}

void Item(Tree *Node){
	Tree *newchild=Node->newNode("ITEM");
	Factor(newchild);
	while(result[i].str=="*"||result[i].str=="/"){
		newchild->newNode(result[i].str);
		i++;
		Factor(newchild); 
	}
}

void Factor(Tree *Node){
	Tree *newchild=Node->newNode("FACTOR");
	if(result[i].type=="IDENTIFIER"||result[i].type=="NUMBER"){
		newchild->newNode(result[i].str),i++;
		return; 
	}
	else if(result[i].str=="("){
		newchild->newNode("LP");
		i++;
		Expression(newchild);
		if(result[i].str==")"){
			newchild->newNode("RP");
			i++;
			return;
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
	if(result[i].str=="ODD"){
		child->newNode("ODD");
		i++;
		Expression(child);
	}
	else{
		Expression(child);
		if(result[i].str=="="||result[i].str=="#"||result[i].str=="<"||result[i].str=="<="||result[i].str==">"||result[i].str==">="){
			child->newNode(result[i].str);i++;	
		}
		else{
			is_error=1;
			return; 
		}
		Expression(child);
	}
}
void IF(Tree* Node){
	Tree *thisNode=Node->newNode("IFSENTENCE");
	Tree *newchild=thisNode->newNode("IF");
	i++;
	Condition(thisNode);
	if(result[i].str=="THEN"){
		thisNode->newNode("THEN");
		i++;
	}
	else{
		is_error=1;
		return;	
	}
	Sentence(thisNode);
}


void Whilesectence(Tree *Node){
	Tree *thisNode=Node->newNode("WHILESENTENCE");
	Tree *newchild=thisNode->newNode("WHILE");
	i++;
	Condition(thisNode);
	if(result[i].str=="DO"){
		thisNode->newNode("DO");i++;
		
	}
	else{
		is_error=1;
		return;
	}
	Sentence(thisNode);
}

void Callsentence(Tree *Node){
	Tree *thisNode=Node->newNode("CALLSENTENCE");
	thisNode->newNode("CALL");
	i++;
	if(result[i].type=="IDENTIFIER"){
		thisNode->newNode(result[i].str);
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
			thisNode->newNode(result[i].str);i++;
		}
		else {
			is_error=1;
			return;
		}
		while(result[i].str==","){
			thisNode->newNode("COMMA"),i++;
			if(result[i].type=="IDENTIFIER"){
				thisNode->newNode(result[i].str);i++;
			}
			else {
				is_error=1;
				return;
			}
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
			thisNode->newNode(result[i].str);i++;
		}
		else {
			is_error=1;
			return;
		}
		while(result[i].str==","){
			thisNode->newNode("COMMA"),i++;
			if(result[i].type=="IDENTIFIER"){
				thisNode->newNode(result[i].str);i++;
			}
			else {
				is_error=1;
				return;
			}
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


