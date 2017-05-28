#include "table.h"

void table::form_tables_from_files(string idn_path, 
string const_path, string key_path, 
string delim_path) {
	unsigned int m_idn = 1001, m_const = 501, m_key = 401, m_delim = 301;
	unsigned int count = 0;
	string word;
	string cons;
	ifstream idn_file;
	idn_file.open(idn_path);
	while (idn_file.good()) {
		idn_file >> word;
		if (!word.empty())
			idn_table.push_back(idn(m_idn + count++, word));
	}
	idn_file.close();
	count = 0;
	ifstream delim_file;
	delim_file.open(delim_path);
	while (delim_file.good()) {
		delim_file >> word;
		if (!word.empty())
			key_table.push_back(idn(m_delim + count++, word));
	}
	delim_file.close();
	count = 0;
	ifstream key_file;
	key_file.open(key_path);
	while (key_file.good()) {
		key_file >> word;
		if(!word.empty())
			key_table.push_back(idn(m_key + count++, word));
	}
	key_file.close();	
	count = 0;
	ifstream const_file;
	const_file.open(const_path);
	while (const_file.good()) {
		const_file >> cons;
		if (!cons.empty())
			const_table.push_back(cdn(m_const + count++, stoi(cons)));
	}
	const_file.close();
}


void table::update_tables(string idn_path, string const_path) {
	ofstream idn_file;
	idn_file.open("new"+idn_path);
	for (auto it = begin(idn_table); it != end(idn_table); ++it)
	{
		idn_file << it->name;
		if ((it+1)!=end(idn_table)) idn_file<< " ";
	}
	idn_file.close();
	ofstream const_file;
	const_file.open("new"+const_path);
	for (auto it = begin(const_table); it != end(const_table); ++it)
	{
		const_file << to_string(it->value);
		if ((it + 1) != end(const_table)) const_file << " ";
	}
	const_file.close();
}

unsigned int table::const_tab_search(int val) {
	int id=500;
	for (auto it = begin(const_table); it != end(const_table); ++it) {
		id = it->i;
		if (it->value == val) return it->i;		
	}
	const_table.push_back(cdn(id+1,val));
	return id+1;
}

unsigned int table::idn_tab_search(string val) {
	int id=1000;
	for (auto it = begin(idn_table); it != end(idn_table); ++it) {
		id = it->i;
		if (it->name == val) return it->i;
	}
	idn_table.push_back(idn(id+1, val));
	return id+1;
}

unsigned int table::key_tab_search(string val) {
	int id=0;
	for (auto it = begin(key_table); it != end(key_table); ++it) {
		if (it->name == val) return it->i;
	}
	return id;
}


void ascii_code::get_ascii_table(string as_path){
	ifstream as;
	as.open(as_path);
	int i = 0;
	unsigned short val;
	while (as.good()) {
		as >> val;
		code[i++] = val;
	}
	as.close();
}