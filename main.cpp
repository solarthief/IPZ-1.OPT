#include "generator.h"

int main(int argc, char** argv) {	
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
	catch (string s) {
		cout << s << endl;		
	}	
	catch (vector<err> t) {
		for (auto it = t.begin(); it != t.end(); it++) {
			cout<<  it->er << " Row: " << it->row_pos << " pos: " << it->pos << endl;
		}
	}
	catch (...) {
		cout << "ERROR";
	}
	return 0;
}