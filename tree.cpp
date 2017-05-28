//File: tree.h
//Abstract: contains realisation of tree class
//			that uses as AST for generator
//Copyright (c) 2017 by Maxim Yakovenko

#include "tree.h"

void Tree::setValue(const string& data) {
	value = data;
	p_is_empty = false;
}

void Tree::addChild(Tree tt) {
	if (!p_is_empty)	children.push_back(tt);
}

string Tree::getValue() {
	return value;
}

vector< Tree > Tree::getChildren() {
	return children;
}
