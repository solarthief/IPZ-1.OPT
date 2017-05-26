#include "generator.h"




//Rule #1
//	<SIGNAL-PROGRAM> -> <PROGRAM>
//Semantic:
//	{[1]}

void generator::code_gen(string file_path) {
	PP.pars(file_path);
	ofstream asm_file;
	asm_file.open("file.asm");
	asm_file.close();
	synt_tree = PP.get_tree();
	code_gen_program(synt_tree.getChildren()[0]);
}


//Rule #2
//	<PROGRAM> -> PROGRAM <PROCEDURE-IDENTIFIER> ;
//					<BLOCK>.				
//Semantic:
//	{code segment \n assume cs: code \n 
//		[2] PROCID [1]
//	mov ax, 4c00h \n int 21h \n code ends \n end PROCID \n}
void generator::code_gen_program(Tree curr) {
	
	code_gen_procedure_identifier(curr.getChildren()[1]);
	code_gen_block(curr.getChildren()[3]);
	
}

//Rule #3
//	<BLOCK> -> <VARIABLE-DECLARATION> BEGIN 
//					<STATEMENTS-LIST> END
//Semantic:
//	{[2][1]}
void generator::code_gen_block(Tree curr) {
	code_gen_variable_declarations(curr.getChildren()[0]);
	cout << "\ncode segment \nassume cs: code \nds: data\n";
	cout << "\n@" + proc_id + ":\n";
	cout << "mov ax,data\nmov ds,ax\n";
	code_gen_statements_list(curr.getChildren()[2]);
	cout << "mov ax, 4c00h \nint 21h \ncode ends \nend @" + proc_id << endl;
	return;
}


//Rule #4
//	<VARIABLE-DECLARATION> -> VAR <DECLARATIONS-LIST>
//									|<empty>
//Semantic:
//	{data segment\n [1]
//	 data ends\n}
void generator::code_gen_variable_declarations(Tree curr){
	if (!curr.getChildren().size()) {
		return;
	}
	cout << "data segment\n";
	code_gen_declarations_list(curr.getChildren()[1]);
	cout << "data ends\n";
}

//Rule #5
//	<DECLARATIONS-LIST> -> <DECLARATION> <DECLARATIONS-LIST>
//									|<empty>
//Semantic:
//	{[2][1]}
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
//Semantic:
//	{\n }
void generator::code_gen_declaration(Tree curr) {
	code_gen_variable_identifier(curr.getChildren()[0]);
	cout << "@" + var_id;
	code_gen_attribute(curr.getChildren()[2]);
	code_gen_attributes_list(curr.getChildren()[3]);
	return;
}

//Rule #7
//	<ATTRIBUTES-LIST> -> <ATTRIBUTE><ATTRIBUTES-LIST>
//						| <EMPTY> ;
//Semantic:
//	{\n[2][1]}
void generator::code_gen_attributes_list(Tree curr) {
	if (!curr.getChildren().size()) {
		return;
	}
	code_gen_attribute(curr.getChildren()[0]);
	code_gen_attributes_list(curr.getChildren()[1]);
	return;
}

//Rule #8
//	<ATTRIBUTE> -> INTEGER | FLOAT | [<range>] ;
//Semantic:
//	{\n }
void generator::code_gen_attribute(Tree curr) {
	if (curr.getChildren()[0].getValue() == to_string(405))
		cout << " dw ?\n";
	else if (curr.getChildren()[0].getValue() == to_string(405))
		cout << " dd ?\n";
	return;
}


//Rule #10
//	<STATEMENTS-LIST> -> <STATEMENT><STATEMENTS-LIST>
//							|<EMPTY>
//Semantic:
//	{\n [2][1]}
void generator::code_gen_statements_list(Tree curr) {
	if (!curr.getChildren().size()) {
		cout << "nop\n";
		return;
	}
	code_gen_statement(curr.getChildren()[0]);
	code_gen_statements_list(curr.getChildren()[1]);
	return;
}

//Rule #11
//	<STATEMENTS> -> <VARIABLE>:=<EXPRESSION>;
//						|LOOP <STATEMENT-LIST> ENDLOOP;
//Semantic:
//	{\n [2][1]}
void generator::code_gen_statement(Tree curr) {
	if (curr.getChildren()[0].getValue() == to_string(407)) {
		cout << "L:\n";
		code_gen_statements_list(curr.getChildren()[1]);
		cout << "goto L\n";
	}		
	else {
		code_gen_variable(curr.getChildren()[0]);
		code_gen_expression(curr.getChildren()[2]);
	}
	return;
}

//Rule #12
//	<EXPRESSION> -> <VARIABLE>
//					|<UNSIGNED-INTEGER>
//Semantic:
//	{\n [2][1]}
void generator::code_gen_expression(Tree curr) {
	if (stoi(curr.getValue())>500&& stoi(curr.getValue())<1000) {
		cout << curr.getChildren()[0].getValue();
		return;
	}
		
	return;
}

//Rule #13
//	<VARIABLE> -> <VARIABLE-IDENTIFIER><DIMENTION>
//							
//Semantic:
//	{\n [2][1]}
void generator::code_gen_variable(Tree curr) {
	
	return;
}

//Rule #14
//	<DIMENSION> -> [<EXPRESSION>]
//					|<EMPTY>					
//Semantic:
//	{\n [2][1]}
void generator::code_gen_dimension(Tree curr) {
	if (!curr.getChildren().size()) {
		return;
	}
	code_gen_expression(curr.getChildren()[1]);
	return;
}


//Rule #15
//	<VARIABLE-IDENTIFIER> -> <IDENTIFIER>
//							
//Semantic:
//	
void generator::code_gen_variable_identifier(Tree curr) {
	var_id = curr.getChildren()[0].getValue();
	return;
}

//Rule #16
//	<PROCEDURE-IDENTIFIER> -> <IDENTIFIER>
//							
//Semantic:
//	
void generator::code_gen_procedure_identifier(Tree curr) {
	proc_id = curr.getChildren()[0].getValue();
	return;
}