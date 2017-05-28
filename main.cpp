#include "generator.h"


int main(int argc, char** argv) {	
	string file_path;
	generator pp;	
	if (argc < 2) {
		cout << "Enter name of file[.sig]: ";
		cin >> file_path;
	}
	else {
		file_path = argv[1];
	}	
	if (file_path.rfind(".sig") == -1) {
		file_path.append(".sig");
	}
	try {		
		pp.code_gen(file_path);
		cout <<"SIGNAL file {"+file_path+ "} was translated into assembly file {"+ file_path.erase(file_path.find_last_of("."))+".asm}";
	}
	catch (string s) {
		cout << "ERROR"<<endl;
		cout << s << endl;		
	}	
	catch (vector<err> t) {
		for (auto it = t.begin(); it != t.end(); it++) {
			cout << "ERROR" << endl;
			cout<<  it->er << " Row: " << it->row_pos << " pos: " << it->pos << endl;
		}
	}
	catch (...) {
		cout << "ERROR";
	}
	return 0;
}