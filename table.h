#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct idn {
	unsigned int i;
	string name;
	idn(unsigned int id, string nm) :i(id), name(nm) {};
};

struct cdn {
	unsigned int i;
	int value;
	cdn(unsigned int id, int val) : i(id), value(val) {};
};

class table {
private:
	vector<idn>idn_table;
	vector<idn>key_table;
	vector<cdn>const_table;
public:
	void form_tables_from_files(string idn_path = "identifier_table.dat",
		string const_path = "const_table.dat", string key_path = "keyword_table.dat",
		string delim_path = "mdelim_table.dat");
	void print_table();
	void update_tables(string idn_path = "identifier_table.dat",
		string const_path = "const_table.dat");
	unsigned int const_tab_search(int);
	unsigned int idn_tab_search(string);
	unsigned int key_tab_search(string);
};

class ascii_code {
private:
	map<const int, unsigned int> code;
public:	
	int operator[](const int i) {
		return code[i];
	};
	void get_ascii_table(string as_path = "ascii_code.dat");
};