//File: generator.h
//Abstract: contains declaration of generator class
//
//Copyright (c) 2017 by Maxim Yakovenko

#ifndef TRANSLATOR_GENERATOR_H_
#define TRANSLATOR_GENERATOR_H_
#include "parser.h"
#endif

class generator {
private:
	Tree synt_tree;
	parser PP;
	string proc_id;
	string var_id;	
	ofstream asm_file;
	vector<string> varbls;
	vector<string> labels;
	unsigned int num_of_layers;
	bool is_r;
	bool was_in_range;
	bool was_declared;
public:
	void code_gen(string);
	void code_gen_program(Tree);
	void code_gen_block(Tree);
	void code_gen_variable_declarations(Tree);
	void code_gen_declarations_list(Tree);
	void code_gen_declaration(Tree);
	void code_gen_attributes_list(Tree);
	void code_gen_attribute(Tree);
	void code_gen_range(Tree);
	void code_gen_statements_list(Tree);
	void code_gen_statement(Tree);
	void code_gen_expression(Tree);
	void code_gen_variable(Tree);
	void code_gen_dimension(Tree);
	void code_gen_label(Tree);
	void code_gen_label_list(Tree);
	void code_gen_variable_identifier(Tree);
	void code_gen_procedure_identifier(Tree);
	void code_gen_lex_by_code();
};
