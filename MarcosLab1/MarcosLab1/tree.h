#pragma once
#include "table.h"

class Tree{
private:
	string value;
	bool p_is_empty;
	vector< Tree > children;
public:
	Tree(): value(),children(), p_is_empty(true){}
	Tree(string s): value(s),children(), p_is_empty(false){}
	Tree(string s, vector< Tree > v): value(s), children(v), p_is_empty(false){}
	void setValue(const string&);
	void addChild(Tree);
	string getValue();
	vector< Tree > getChildren();
};

void Tree::setValue(const string& data) {
	value = data;
	p_is_empty = false;
}

void Tree::addChild(Tree tt) {
	if(!p_is_empty)	children.push_back(tt);
}

string Tree::getValue() {
	return value;
}

vector< Tree > Tree::getChildren() {
	return children;
}