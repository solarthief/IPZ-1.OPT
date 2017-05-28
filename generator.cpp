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
	num_of_layers = 1;
	is_r = false;
	was_in_range = false;
	
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
	cout << "data segment";
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
	was_declared = false;
	code_gen_variable_identifier(curr.getChildren()[0]);
	if (find(varbls.begin(), varbls.end(), var_id) != varbls.end()||var_id==proc_id) {
		throw("Double declaration "+ curr.getChildren()[0].getChildren()[0].getChildren()[0].getValue());
	}
	varbls.push_back(var_id);
	cout << "\n@" + var_id;
	code_gen_attribute(curr.getChildren()[2]);	
	code_gen_attributes_list(curr.getChildren()[3]);
	was_declared = false;
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
	else {
		code_gen_attribute(curr.getChildren()[0]);
		code_gen_attributes_list(curr.getChildren()[1]);
	}
	return;
}

//Rule #8
//	<ATTRIBUTE> -> INTEGER | FLOAT | [<range>] ;
//Semantic:
//	{\n }
void generator::code_gen_attribute(Tree curr) {
	if (curr.getChildren()[0].getValue() == to_string(405)) {
		if (was_declared) {
			throw(string("Type was already declared"));
		}
		cout << " dw ";	
		was_declared = true;
	}		
	else {
		if (curr.getChildren()[0].getValue() == to_string(406))
		{
			throw(string("Type was already declared"));
			cout << " dd ";
			was_declared = true;
		}
		else {
			code_gen_range(curr.getChildren()[1]);
			return;
		}
	}
	if (was_in_range) {
		cout << "?\n";
		was_in_range = false;
	}
	return;
}

//Rule #9
//	<RANGE> -> <UNSIGNED-INTEGER>..<UNSIGNED-INTEGER> 
//Semantic:
//	{\n }
void generator::code_gen_range(Tree curr) {
	was_in_range = true;
	int a=stoi(curr.getChildren()[0].getChildren()[0].getValue());
	int b=stoi(curr.getChildren()[2].getChildren()[0].getValue());
	int range = a-b;
	cout<<abs(range)<<" dup (0)";	
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
	else {
		code_gen_statement(curr.getChildren()[0]);
		code_gen_statements_list(curr.getChildren()[1]);
	}
	return;
}

//Rule #11
//	<STATEMENTS> -> <VARIABLE>:=<EXPRESSION>;
//						|LOOP <STATEMENT-LIST> ENDLOOP;
//Semantic:
//	{\n [2][1]}
void generator::code_gen_statement(Tree curr) {	
	if (curr.getChildren()[0].getValue() == to_string(407)) {
		unsigned int current_layer = num_of_layers++;
		cout << "L"+to_string(current_layer)+ ": \n";
		code_gen_statements_list(curr.getChildren()[1]);
		cout << "jmp L"+to_string(current_layer)+ ": \n";
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
//Semantic:
//	{\n [2][1]}
void generator::code_gen_expression(Tree curr) {
	if (!is_r) {
		cout << "mov ax, ";
	}
	if (curr.getChildren()[0].getValue() == "<VARIABLE>") {
		is_r = true;
		code_gen_variable(curr.getChildren()[0]);		
	}
	else if (stoi(curr.getChildren()[0].getValue())>500&& stoi(curr.getChildren()[0].getValue())<1000) {
		cout << curr.getChildren()[0].getChildren()[0].getValue();
		is_r = false;
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
	bool qqw=is_r;
	code_gen_variable_identifier(curr.getChildren()[0]);
	if (!qqw) {
		cout << "\nmov @" + var_id;
	}
	else {
		cout <<"@"+var_id;
	}
	code_gen_dimension(curr.getChildren()[1]);
	if (!qqw) {
		cout << ", ax\n";
	}
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
	else {
		cout << "[";
		is_r = true;
		code_gen_expression(curr.getChildren()[1]);
		cout << "]";
		is_r = false;
	}
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