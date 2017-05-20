#pragma once
#include "table.h"


struct token {
	unsigned int code;
	unsigned int row_pos;
	unsigned int pos;
	token(unsigned int c, unsigned int r, unsigned int p) :code(c), row_pos(r), pos(p) {};
};

struct err {
	string er;
	unsigned int row_pos;
	unsigned int pos;
	err(string c, unsigned int r, unsigned int p) :er(c), row_pos(r), pos(p) {};
};

class tbl {
private:
	vector<token> lexems;
	vector<err> log;
public:
	tbl() :lexems() {};
	tbl(const tbl &l) :lexems(l.lexems) {};
	void lex_analys(string file_path);
	void push(unsigned int, unsigned int, unsigned int);
	void er_push(string, unsigned int, unsigned int);
	vector<token> get_lexems();
	vector<err> get_errors();
	void print_lexems();
	void create_lex_file();	
};

