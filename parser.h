#pragma once
#include "lex.h"
#include "tree.h"

class parser {
private:
	vector<token> lexems;
	Tree tree;
	unsigned int ct;
	unsigned int max_ct;
	char fill_symbol;
	unsigned int num_of_tabs;
public:
	parser(): lexems() {};
	parser(const parser &pp): lexems(pp.lexems) {};	
    Tree pars(string);
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
	Tree parse_label();
	Tree parse_label_list();
    Tree parse_variable_identifier();
	Tree parse_procedure_identifier();
	Tree parse_lex_by_code();
	void print_tree();
	void rec_print_tree(Tree);	
	Tree get_tree();
};

