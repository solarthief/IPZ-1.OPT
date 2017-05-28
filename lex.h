//File: lex.h
//Abstract: contains declaration of lexem class
//			and support structures
//Copyright (c) 2017 by Maxim Yakovenko

#ifndef TRANSLATOR_LEX_H_
#define TRANSLATOR_LEX_H_
#include "table.h"
#endif

struct token {
	unsigned int code;
	string name;
	unsigned int row_pos;
	unsigned int pos;
	token(unsigned int c, unsigned int r, unsigned int p, string s) :code(c), row_pos(r), pos(p), name(s) {};
};

struct err {
	string er;
	unsigned int row_pos;
	unsigned int pos;
	err(string c, unsigned int r, unsigned int p) :er(c), row_pos(r), pos(p) {};
};

class lexem {
private:
	vector<token> lexems;
	vector<err> log;
public:
	lexem() :lexems() {};
	lexem(const lexem &l) :lexems(l.lexems) {};
	void lex_analys(string file_path);
	void push(unsigned int, unsigned int, unsigned int,string s);
	void er_push(string, unsigned int, unsigned int);
	vector<token> get_lexems();
	vector<err> get_errors();	
	void create_lex_file();	
};

