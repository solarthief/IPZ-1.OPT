#include "generator.h"

int main() {	
	string f_name;
	generator pp;
	parser t;
	cout << "Enter name of file: ";
	cin >> f_name;
	try {
		//t.pars(f_name);
		//t.print_tree();
		pp.code_gen(f_name);
	}
	catch (...) {
		cout << "ERROR found. Read error log" << endl;
		//t.print_error_log();
	}	
	return 0;
}