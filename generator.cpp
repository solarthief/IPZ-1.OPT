#include "generator.h"


//Rule #1
//	<SIGNAL-PROGRAM> -> <PROGRAM>
void generator::code_gen(string file_path) {
	PP.pars(file_path);
	file_path= file_path.erase(file_path.find_last_of("."));
	asm_file.open(file_path+".asm");	
	synt_tree = PP.get_tree();
	num_of_layers = 1;
	is_r = false;
	was_in_range = false;	
	code_gen_program(synt_tree.getChildren()[0]);
	asm_file.close();
}


//Rule #2
//	<PROGRAM> -> PROGRAM <PROCEDURE-IDENTIFIER> ;
//					<BLOCK>.				
void generator::code_gen_program(Tree curr) {
	
	code_gen_procedure_identifier(curr.getChildren()[1]);
	code_gen_block(curr.getChildren()[3]);
	
}

//Rule #3
//	<BLOCK> -> <VARIABLE-DECLARATION> BEGIN 
//					<STATEMENTS-LIST> END
void generator::code_gen_block(Tree curr) {
	code_gen_variable_declarations(curr.getChildren()[0]);
	asm_file << "\ncode segment \nassume cs: code \nds: data\n";
	asm_file << "\n@" + proc_id + ":\n";
	asm_file << "mov ax,data\nmov ds,ax\n";
	code_gen_statements_list(curr.getChildren()[2]);
	asm_file << "mov ax, 4c00h \nint 21h \ncode ends \nend @" + proc_id << endl;
	return;
}


//Rule #4
//	<VARIABLE-DECLARATION> -> VAR <DECLARATIONS-LIST>
//									|<empty>
void generator::code_gen_variable_declarations(Tree curr){
	if (!curr.getChildren().size()) {
		return;
	}
	asm_file << "data segment";
	code_gen_declarations_list(curr.getChildren()[1]);
	asm_file << "data ends\n";
}

//Rule #5
//	<DECLARATIONS-LIST> -> <DECLARATION> <DECLARATIONS-LIST>
//									|<empty>
void generator::code_gen_declarations_list(Tree curr) {
	if (!curr.getChildren().size()) {
		return;
	}
	code_gen_declaration(curr.getChildren()[0]);
	code_gen_declarations_list(curr.getChildren()[1]);
	return;
}

//Rule #6
//	<DECLARATION> -> <VARIABLE-IDENTIFIER> :
//						<ATTRIBUTE><ATTRIBUTES-LIST> ;
void generator::code_gen_declaration(Tree curr) {
	was_declared = false;
	code_gen_variable_identifier(curr.getChildren()[0]);
	if (find(varbls.begin(), varbls.end(), var_id) != varbls.end()||var_id==proc_id) {
		throw("Double declaration "+ curr.getChildren()[0].getChildren()[0].getChildren()[0].getValue());
	}
	varbls.push_back(var_id);
	asm_file << "\n@" + var_id;
	code_gen_attribute(curr.getChildren()[2]);	
	code_gen_attributes_list(curr.getChildren()[3]);
	was_declared = false;
	return;
}

//Rule #7
//	<ATTRIBUTES-LIST> -> <ATTRIBUTE><ATTRIBUTES-LIST>
//						| <EMPTY> ;
void generator::code_gen_attributes_list(Tree curr) {
	if (!curr.getChildren().size()) {
		return;
	}
	else {
		code_gen_attribute(curr.getChildren()[0]);
		code_gen_attributes_list(curr.getChildren()[1]);
	}
	return;
}

//Rule #8
//	<ATTRIBUTE> -> INTEGER | FLOAT | [<range>] ;
void generator::code_gen_attribute(Tree curr) {
	if (curr.getChildren()[0].getValue() == to_string(405)) {
		if (was_declared) {
			throw(string("Type was already declared"));
		}
		asm_file << " dw ";	
		was_declared = true;
	}		
	else {
		if (curr.getChildren()[0].getValue() == to_string(406))
		{
			throw(string("Type was already declared"));
			asm_file << " dd ";
			was_declared = true;
		}
		else {
			code_gen_range(curr.getChildren()[1]);
			return;
		}
	}
	if (was_in_range) {
		asm_file << "?\n";
		was_in_range = false;
	}
	return;
}

//Rule #9
//	<RANGE> -> <UNSIGNED-INTEGER>..<UNSIGNED-INTEGER> 
void generator::code_gen_range(Tree curr) {
	was_in_range = true;
	int a=stoi(curr.getChildren()[0].getChildren()[0].getValue());
	int b=stoi(curr.getChildren()[2].getChildren()[0].getValue());
	int range = a-b;
	asm_file<<abs(range)<<" dup (0)";	
	return;
}

//Rule #10
//	<STATEMENTS-LIST> -> <STATEMENT><STATEMENTS-LIST>
//							|<EMPTY>
void generator::code_gen_statements_list(Tree curr) {
	if (!curr.getChildren().size()) {
		asm_file << "nop\n";
		return;
	}
	else {
		code_gen_statement(curr.getChildren()[0]);
		code_gen_statements_list(curr.getChildren()[1]);
	}
	return;
}

//Rule #11
//	<STATEMENTS> -> <VARIABLE>:=<EXPRESSION>;
//						|LOOP <STATEMENT-LIST> ENDLOOP;
void generator::code_gen_statement(Tree curr) {	
	if (curr.getChildren()[0].getValue() == to_string(407)) {
		unsigned int current_layer = num_of_layers++;
		asm_file << "L"+to_string(current_layer)+ ": \n";
		code_gen_statements_list(curr.getChildren()[1]);
		asm_file << "jmp L"+to_string(current_layer)+ ": \n";
		num_of_layers--;
	}		
	else {
		code_gen_expression(curr.getChildren()[2]);
		code_gen_variable(curr.getChildren()[0]);			
	}
	return;
}

//Rule #12
//	<EXPRESSION> -> <VARIABLE>
//					|<UNSIGNED-INTEGER>
void generator::code_gen_expression(Tree curr) {
	if (!is_r) {
		asm_file << "mov ax, ";
	}
	if (curr.getChildren()[0].getValue() == "<VARIABLE>") {
		is_r = true;
		code_gen_variable(curr.getChildren()[0]);		
	}
	else if (stoi(curr.getChildren()[0].getValue())>500&& stoi(curr.getChildren()[0].getValue())<1000) {
		asm_file << curr.getChildren()[0].getChildren()[0].getValue();
		is_r = false;
		return;
	}	

	return;
}

//Rule #13
//	<VARIABLE> -> <VARIABLE-IDENTIFIER><DIMENTION>
//							
void generator::code_gen_variable(Tree curr) {
	bool qqw=is_r;
	code_gen_variable_identifier(curr.getChildren()[0]);
	if (!qqw) {
		asm_file << "\nmov @" + var_id;
	}
	else {
		asm_file <<"@"+var_id;
	}
	code_gen_dimension(curr.getChildren()[1]);
	if (!qqw) {
		asm_file << ", ax\n";
	}
	return;
}

//Rule #14
//	<DIMENSION> -> [<EXPRESSION>]
//					|<EMPTY>					
void generator::code_gen_dimension(Tree curr) {
	if (!curr.getChildren().size()) {
		return;
	}
	else {
		asm_file << "[";
		is_r = true;
		code_gen_expression(curr.getChildren()[1]);
		asm_file << "]";
		is_r = false;
	}
	return;
}


//Rule #15
//	<VARIABLE-IDENTIFIER> -> <IDENTIFIER>
//							
void generator::code_gen_variable_identifier(Tree curr) {
	var_id = curr.getChildren()[0].getValue();
	return;
}

//Rule #16
//	<PROCEDURE-IDENTIFIER> -> <IDENTIFIER>
//
void generator::code_gen_procedure_identifier(Tree curr) {
	proc_id = curr.getChildren()[0].getValue();
	return;
}