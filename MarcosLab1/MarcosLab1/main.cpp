#include "parser.h"

int main() {	
	string f_name;
	parser pp;
	cout << "Enter name of file: ";
	cin >> f_name;
	try {
		pp.pars(f_name);
		pp.print_tree();
	}
	catch (...) {
		cout << "ERROR found. Read error log" << endl;
		pp.print_error_log();
	}	
	return 0;
}