//File: parser.cpp
//Abstract: contains realisation of parser class
//			creates AST from vector of lexems
//Copyright (c) 2017 by Maxim Yakovenko

#include "parser.h"

//Rule #1
//	<SIGNAL-PROGRAM> -> <PROGRAM>
Tree parser::pars(string file_path){
	lexem TT;
	TT.lex_analys(file_path);
	lexems = TT.get_lexems();	
	ct = 0;	 
	max_ct = lexems.size();
	tree.setValue("<SIGNAL-PROGRAM>");
	tree.addChild(parse_program());		 
	return tree;
}

//Rule #2
//	<PROGRAM> -> PROGRAM <PROCEDURE-IDENTIFIER> ;
//					<BLOCK>.
Tree parser::parse_program(){
	 
	Tree curr;
	if (ct > max_ct || lexems[ct].code != 401) {		
		throw("PROGRAM keyword expected   row:"+  to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
	}		
	else {	 
		curr.setValue("<PROGRAM>");		 
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_procedure_identifier());
		if (ct > max_ct || lexems[ct].code != 59) {			
			throw("Semicolon expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {			 
			curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_block());
			if (ct > max_ct || lexems[ct].code != '.') {
				 
				throw("Dot expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
			}
			else {
				 
				 curr.addChild(parse_lex_by_code());
			}
		}
	}	 
	return curr;
}

//Rule #3
//	<BLOCK> -> <VARIABLE-DECLARATION> BEGIN 
//					<STATEMENTS-LIST> END
Tree parser::parse_block(){
	 
	Tree curr;
	 
	curr.setValue("<BLOCK>");
	curr.addChild(parse_variable_declarations());
	if (ct > max_ct || lexems[ct].code != 402) {
		 
		throw("BEGIN keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
	}
	else {
		 
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_statements_list());
		if (ct > max_ct || lexems[ct].code != 403) {
			 
			throw("END keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			if (ct >= max_ct || lexems[ct].code != '.') {
				 
				throw("Dot expected   row:"+ to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
			}
			
		}
	} 
	return curr;
}

//Rule #4
//	<VARIABLE-DECLARATION> -> VAR <DECLARATIONS-LIST>
//									|<empty>
Tree parser::parse_variable_declarations(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE-DECLARATION>");
	if (lexems[ct].code == 402) {		 
		return curr;
	}
	else {
		if (ct > max_ct || lexems[ct].code != 404) {			 
			throw("VAR keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_declarations_list());
		}
	}
	 
	 
	return curr;
}

//Rule #5
//	<DECLARATIONS-LIST> -> <DECLARATION> <DECLARATIONS-LIST>
//									|<empty>
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

//Rule #6
//	<DECLARATION> -> <VARIABLE-IDENTIFIER> :
//						<ATTRIBUTE><ATTRIBUTES-LIST> ;
//					Additional:	LABEL<LABEL><LABEL-LIST>;		
Tree parser::parse_declaration(){
	 
	Tree curr;
	 
	curr.setValue("<DECLARATION>");
	if (lexems[ct].code == 409) {
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_label());
		curr.addChild(parse_label_list());
		if (ct > max_ct || lexems[ct].code != ';') {
			 
			throw("Semicolon keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			curr.addChild(parse_lex_by_code());
			ct++;
		}
	}
	else {
		curr.addChild(parse_variable_identifier());
		if (ct > max_ct || lexems[ct].code != ':') {
			 
			throw("Colon expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {

			curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_attribute());
			curr.addChild(parse_attributes_list());
			if (ct > max_ct || lexems[ct].code != ';') {
				 
				throw("Semicolon keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
			}
			else {

				curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}
	 
	return curr;
}


//Additional rule #1
//	<LABEL> -> <UNSIGNED-INTEGER>	
//
Tree parser::parse_label() {
	Tree curr;
	curr.setValue("<LABEL>");
	if (lexems[ct].code >500 && lexems[ct].code <1000) {
		 curr.addChild(parse_lex_by_code());
		ct++;
	}
	return curr;
}

//Additional rule #2
//	<LABEL-LIST> -> ,<LABEL><LABEL-LIST>|
//						<EMPTY>	
//
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

//Rule #7
//	<ATTRIBUTES-LIST> -> <ATTRIBUTE><ATTRIBUTES-LIST>
//						| <EMPTY> ;
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

//Rule #8
//	<ATTRIBUTE> -> INTEGER | FLOAT | [<range>] ;
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
			 
			throw("[ expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_range());
			if (ct > max_ct || lexems[ct].code != ']') {
				 
				throw("] expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
			}
			else {				 
				curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}	 
	return curr;
}

//Rule #9
//	<RANGE> -> <UNSIGNED-INTEGER>..<UNSIGNED-INTEGER> 
Tree parser::parse_range(){
	 
	Tree curr;
	 
	curr.setValue("<RANGE>");
	if (ct > max_ct || lexems[ct].code <500|| lexems[ct].code >1000) {
		 
		throw("Numeric constant expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
	}
	else {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		if (ct > max_ct || lexems[ct].code != 302) {
			 
			throw(".. expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;
			if (ct > max_ct || lexems[ct].code <500 || lexems[ct].code >1000) {
				 
				throw("Numeric constant expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
			}
			else {
				 
				curr.addChild(parse_lex_by_code());
				ct++;
			}
		}
	}
	 
	 
	return curr;
}

//Rule #10
//	<STATEMENTS-LIST> -> <STATEMENT><STATEMENTS-LIST>
//							|<EMPTY>
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

//Rule #11
//	<STATEMENTS> -> <VARIABLE>:=<EXPRESSION>;
//						|LOOP <STATEMENT-LIST> ENDLOOP;
Tree parser::parse_statement(){
	 
	Tree curr;
	 
	curr.setValue("<STATEMENT>");
	if (lexems[ct].code == 407) {
		 
		 curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_statements_list());		
		if (ct > max_ct || lexems[ct].code != 408) {
			 
			throw("ENDLOOP keyword expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			 curr.addChild(parse_lex_by_code());
			ct++;			
			if (ct > max_ct || lexems[ct].code != ';') {
				 
				throw("Semicolon expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
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
			 
			throw(":= expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {
			 
			curr.addChild(parse_lex_by_code());
			ct++;
			curr.addChild(parse_expression());			
			if (ct > max_ct || lexems[ct].code != ';') {
				 
				throw("Semicolon expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
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

//Rule #12
//	<EXPRESSION> -> <VARIABLE>
//					|<UNSIGNED-INTEGER>
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

//Rule #13
//	<VARIABLE> -> <VARIABLE-IDENTIFIER><DIMENTION>		
Tree parser::parse_variable(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE>");
	curr.addChild(parse_variable_identifier());
	curr.addChild(parse_dimension()); 
	 
	return curr;
}

//Rule #14
//	<DIMENSION> -> [<EXPRESSION>]
//					|<EMPTY>
Tree parser::parse_dimension(){
	 
	Tree curr;
	 
	curr.setValue("<DIMENTION>");
	if (lexems[ct].code == 301 || lexems[ct].code == ';') {
		return curr;		
	}
	if (ct > max_ct || lexems[ct].code != '[') {
		 
		throw("[ expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
	}
	else {
		 
		curr.addChild(parse_lex_by_code());
		ct++;
		curr.addChild(parse_expression());
		if (ct > max_ct || lexems[ct].code != ']') {
			 
			throw("] expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
		}
		else {			 
			 curr.addChild(parse_lex_by_code());
			ct++;
		}
	} 
	 
	return curr;
}

//Rule #15
//	<VARIABLE-IDENTIFIER> -> <IDENTIFIER>	
Tree parser::parse_variable_identifier(){
	 
	Tree curr;
	 
	curr.setValue("<VARIABLE-IDENTIFIER>");
	if (ct > max_ct || lexems[ct].code < 1000) {		 
		throw("Variable expected   row:" + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
	}
	else {		 
		curr.addChild(parse_lex_by_code());
		ct++;
	}	 
	 
	return curr;
}

//Rule #16
//	<PROCEDURE-IDENTIFIER> -> <IDENTIFIER>
Tree parser::parse_procedure_identifier(){
	 
	Tree curr;
	 
	curr.setValue("<PROCEDURE-IDENTIFIER>");
	if (ct > max_ct || lexems[ct].code < 1000) {		 
		throw("Variable expected   row: " + to_string(lexems[ct].row_pos) + " pos: " + to_string(lexems[ct].pos));
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
	curr.addChild(lexems[ct].name);
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
