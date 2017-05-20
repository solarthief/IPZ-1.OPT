#pragma once
#include "lex.h"
#include "tree.h"

class parser {
private:
	vector<token> lexems;
	vector<err> errors;
	Tree tree;
	unsigned int ct;
	unsigned int max_ct;
	char fill_symbol;
	unsigned int num_of_tabs;
public:
	parser(): lexems(),errors() {};
	parser(const parser &pp): lexems(pp.lexems), errors(pp.errors) {};
	void push_errors(string,unsigned int,unsigned int);
    void pars(string);
	Tree parse_program();
	Tree parse_block();
    Tree parse_variable_declarations();
    Tree parse_declarations_list();
    Tree parse_declaration();
    Tree parse_attributes_list();
    Tree parse_attribute();
    Tree parse_range();
    Tree parse_statements_list();
    Tree parse_statement();
    Tree parse_expression();
    Tree parse_variable();
    Tree parse_dimension();
    Tree parse_variable_identifier();
	Tree parse_procedure_identifier();
	void print_tree();
	void rec_print_tree(Tree);
	void set_error_log();
	void print_error_log();
};

