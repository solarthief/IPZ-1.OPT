#ifndef TRANSLATOR_TREE_H_
#define TRANSLATOR_TREE_H_
#include <string>
#include <vector>
#endif

using namespace std;

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


