#include "parser.h"

void parser::pars(string file_path){
	tbl TT;
	TT.lex_analys(file_path);
	lexems = TT.get_lexems();
	errors = TT.get_errors();
	ct = 0;	 
	max_ct = lexems.size();
	if (!errors.empty()) {
		throw(1);
	}
	else {		 
		tree.setValue("<SIGNAL-PROGRAM>");
		tree.addChild(parse_program());		 
	}
}

Tree parser::parse_program(){
	 
	Tree curr;
	if (ct > max_ct || lexems[ct].code != 401) {
		push_errors("PROGRAM keyword expected", lexems[ct].row_pos,lexems[ct].pos);
		throw(1);
	}		
	else {
		 
		curr.setValue("<PROGRAM>");
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_procedure_identifier());
		if (ct > max_ct || lexems[ct].code != 59) {
			push_errors("Semicolon expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_block());
			if (ct > max_ct || lexems[ct].code != '.') {
				push_errors("Dot expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {
				 
				 curr.addChild(parse_lex_by_code());
			}
		}
	}
	 
	 
	return curr;
}


Tree parser::parse_block(){
	 
	Tree curr;
	 
	curr.setValue("<BLOCK>");
	curr.addChild(parse_variable_declarations());
	if (ct > max_ct || lexems[ct].code != 402) {
		push_errors("BEGIN keyword expected", lexems[ct].row_pos, lexems[ct].pos);
		throw(1);
	}
	else {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_statements_list());
		if (ct > max_ct || lexems[ct].code != 403) {
			push_errors("END keyword expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			if (ct >= max_ct || lexems[ct].code != '.') {
				push_errors("Dot expected", lexems[--ct].row_pos, lexems[--ct].pos);
				throw(1);
			}
			
		}
	}   
	 
	 
	return curr;
}


Tree parser::parse_variable_declarations(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE-DECLARATION>");
	if (lexems[ct].code == 402) {
		 
		return curr;
	}
	else {
		if (ct > max_ct || lexems[ct].code != 404) {
			push_errors("VAR keyword expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_declarations_list());
		}
	}
	 
	 
	return curr;
}

Tree parser::parse_declarations_list(){
	 
	Tree curr;
	 
	curr.setValue("<DECLARATION-LIST>");
	if (lexems[ct].code == 402) {
		 
		return curr;		
	}
	else {
			curr.addChild(parse_declaration());
			curr.addChild(parse_declarations_list());
		}
	 
	 
	return curr;
}


Tree parser::parse_declaration(){
	 
	Tree curr;
	 
	curr.setValue("<DECLARATION>");
	if (lexems[ct].code == 409) {
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_label());
		curr.addChild(parse_label_list());
		if (ct > max_ct || lexems[ct].code != ';') {
			push_errors("Semicolon keyword expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {

			 curr.addChild(parse_lex_by_code());
			ct++;
		}
	}
	else {
		curr.addChild(parse_variable_identifier());
		if (ct > max_ct || lexems[ct].code != ':') {
			push_errors("Colon expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {

			 curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_attribute());
			curr.addChild(parse_attributes_list());
			if (ct > max_ct || lexems[ct].code != ';') {
				push_errors("Semicolon keyword expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {

				 curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}
	 
	return curr;
}

Tree parser::parse_label() {
	Tree curr;
	curr.setValue("<LABEL>");
	if (lexems[ct].code >500 && lexems[ct].code <1000) {
		 curr.addChild(parse_lex_by_code());
		ct++;
	}
	return curr;
}

Tree parser::parse_label_list() {
	Tree curr;
	curr.setValue("<LABEL-LIST>");
	if (ct > max_ct || lexems[ct].code == ';') {
		return curr;
	}
	if (lexems[ct].code == ',') {
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_label());
		curr.addChild(parse_label_list());
	}	
	return curr;
}

Tree parser::parse_attributes_list(){	 
	Tree curr;
	 curr.setValue("<ATTRIBUTES-LIST>");
	if (lexems[ct].code == ';') {		 
		return curr;		
	}
	else {
		curr.addChild(parse_attribute());
		curr.addChild(parse_attributes_list());
	}   
	return curr;
}


Tree parser::parse_attribute(){
	 
	Tree curr;	 
	curr.setValue("<ATTRIBUTES>");	 
	if (lexems[ct].code == 405 || lexems[ct].code == 406) {		 
		 curr.addChild(parse_lex_by_code());
		ct++;		 
		return curr;
	}
	else {
		if (ct > max_ct || lexems[ct].code != '[') {
			push_errors("[ expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_range());
			if (ct > max_ct || lexems[ct].code != ']') {
				push_errors("] expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {
				 
				 curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}	 
	return curr;
}


Tree parser::parse_range(){
	 
	Tree curr;
	 
	curr.setValue("<RANGE>");
	if (ct > max_ct || lexems[ct].code <500|| lexems[ct].code >1000) {
		push_errors("Numeric constant expected", lexems[ct].row_pos, lexems[ct].pos);
		throw(1);
	}
	else {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		if (ct > max_ct || lexems[ct].code != 302) {
			push_errors(".. expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			if (ct > max_ct || lexems[ct].code <500 || lexems[ct].code >1000) {
				push_errors("Numeric constant expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {
				 
				 curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}
	 
	 
	return curr;
}


Tree parser::parse_statements_list(){
	 
	Tree curr;
	 
	curr.setValue("<STATEMENTS-LIST>");
	if (lexems[ct].code == 403 || lexems[ct].code == 408) {
		 
		return curr;
	}
	curr.addChild(parse_statement());
	curr.addChild(parse_statements_list());
	 
	 
	return curr;
}

Tree parser::parse_statement(){
	 
	Tree curr;
	 
	curr.setValue("<STATEMENT>");
	if (lexems[ct].code == 407) {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_statements_list());		
		if (ct > max_ct || lexems[ct].code != 408) {
			push_errors("ENDLOOP keyword expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;			
			if (ct > max_ct || lexems[ct].code != ';') {
				push_errors("Semicolon expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {
				 
				 curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}
	else {
		curr.addChild(parse_variable());
		if (ct > max_ct || lexems[ct].code != 301) {
			push_errors(":= expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_expression());			
			if (ct > max_ct || lexems[ct].code != ';') {
				push_errors("Semicolon expected", lexems[ct].row_pos, lexems[ct].pos);
				throw(1);
			}
			else {
				 
				curr.addChild(parse_lex_by_code());
				ct++;
				 
				return curr;
			}
		}
	}
	 
	 
	return curr;
}


Tree parser::parse_expression(){
	 
	Tree curr;
	 
	curr.setValue("<EXPRESSION>");
	if (lexems[ct].code == ';') {
		 
		return curr;		
	}
	if (lexems[ct].code > 500 && lexems[ct].code < 1000) {
		 
		curr.addChild(parse_lex_by_code());
		ct++;
		 
		return curr;
	}
	else {
		curr.addChild(parse_variable());
		//ct++;
	}
	
	//curr.addChild(parse_variable());
	 
	 
	return curr;
}

Tree parser::parse_variable(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE>");
	curr.addChild(parse_variable_identifier());
	curr.addChild(parse_dimension()); 
	 
	return curr;
}

Tree parser::parse_dimension(){
	 
	Tree curr;
	 
	curr.setValue("<DIMENTION>");
	if (lexems[ct].code == 301 || lexems[ct].code == ';') {
		return curr;		
	}
	if (ct > max_ct || lexems[ct].code != '[') {
		push_errors("[ expected", lexems[ct].row_pos, lexems[ct].pos);
		throw(1);
	}
	else {
		 
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_expression());
		if (ct > max_ct || lexems[ct].code != ']') {
			push_errors("] expected", lexems[ct].row_pos, lexems[ct].pos);
			throw(1);
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
		}
	}
	 
	 
	return curr;
}

Tree parser::parse_variable_identifier(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE-IDENTIFIER>");
	if (ct > max_ct || lexems[ct].code < 1000) {
		push_errors("Variable expected", lexems[ct].row_pos, lexems[ct].pos);
		throw(1);
	}
	else {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
	}
	 
	 
	return curr;
}

Tree parser::parse_procedure_identifier(){
	 
	Tree curr;
	 
	curr.setValue("<PROCEDURE-IDENTIFIER>");
	if (ct > max_ct || lexems[ct].code < 1000) {
		push_errors("Variable expected", lexems[ct].row_pos, lexems[ct].pos);
		throw(1);
	}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;			
		}	 
	return curr;
}

Tree parser::parse_lex_by_code() {
	Tree curr;
	curr.setValue(to_string(lexems[ct].code));
	curr.addChild(lexems[ct].ll);
	//ct++;
	return curr;
}

void parser::print_tree() {
	num_of_tabs =0;
	fill_symbol = '|';
	rec_print_tree(tree);
}


void parser::rec_print_tree(Tree curr) {
	cout << string(num_of_tabs, fill_symbol)<< curr.getValue() << endl;
	num_of_tabs++;
		if (curr.getChildren().empty()) {
			num_of_tabs--;
			return;
		}
		for (unsigned int it = 0; it < curr.getChildren().size(); ++it) {
			rec_print_tree(curr.getChildren()[it]);
		}	
		num_of_tabs--;
		return;
}


void parser::push_errors(string er, unsigned int row, unsigned int pos) {
	errors.push_back(err(er,row,pos));
}

void parser::set_error_log() {
	ofstream file;
	if (errors.size() != 0) {
		file.open("Error_log.txt");
		for (auto it = begin(errors); it != end(errors); ++it) {
			file << it->er << " Row: " << it->row_pos << " pos: " << it->pos << endl;
		}
		file.close();
	}
}

void parser::print_error_log() {
	if (errors.size() != 0) {
		for (auto it = begin(errors); it != end(errors); ++it) {
			cout << it->er << " Row: " << it->row_pos << " pos: " << it->pos << endl;
		}
	}
}


Tree parser::get_tree() {
	return tree;
}