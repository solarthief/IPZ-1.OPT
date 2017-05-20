#include "table.h"

struct token {
	unsigned int code;
	unsigned int row_pos;
	unsigned int pos;
};

class lex {
private:
	vector<token>tbl;
public:
	lex() :tbl() {};
	lex(const lex &l) :tbl(l.tbl) {};
	void analyse_file(string file_path = "test.txt");
};