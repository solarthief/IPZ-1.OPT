//File: lex.cpp
//Abstract: contains realisation of lexem class
//			uses for creating vector of tokens for parser 
//Copyright (c) 2017 by Maxim Yakovenko

#include "lex.h"

void lexem::er_push(string s, unsigned int r, unsigned int i) {
	log.push_back(err(s,r,i));
}

void lexem::push(unsigned int c, unsigned int r, unsigned int p,string s) {
	lexems.push_back(token(c, r, p,s));
}

vector<token> lexem::get_lexems() {
	return (*this).lexems;
}

vector<err> lexem::get_errors() {
	return (*this).log;
}

void lexem::create_lex_file() {
	ofstream file;
	file.open("Lexem.dat");
	file << "Lexems:" << endl;
	for (auto it = begin(lexems); it != end(lexems); ++it) {
		file << "Code: " << it->code<<" Lexem: "<<it->name << " Row: " << it->row_pos << " pos: " << it->pos<<endl;
	}
	file.close();
	if (log.size() != 0) {
		file.open("Error_log.txt");
		for (auto it = begin(log); it != end(log); ++it) {
			file << it->er << " Row: " << it->row_pos << " pos: " << it->pos << endl;
		}
		file.close();
	}
}


void lexem::lex_analys(string file_path) {
	char ch;
	string buf;
	unsigned int row = 1, pos = 1,count=1;
	unsigned short lexCode;
	bool SuppressOutput;
	table TT;
	TT.form_tables_from_files();
	ascii_code smbls;
	smbls.get_ascii_table();
	ifstream file;
	file.open(file_path);
	if (!file) {
		throw("File is empty");
	}
	file.get(ch);
	while (file.good()) {
		buf = "";
		lexCode = 0;
		SuppressOutput = false;

		switch (smbls[ch]) {
		case 0:		//whitespace
			while (file.good()) {
				if (ch == 10) {
					pos = 0;
					count = 0;
					row++;
				}
				file.get(ch);
				count++;
				if (smbls[ch] != 0) break;
			}
			SuppressOutput = true;
			break;
		case 1:		//numeric const
			pos = count;
			while (file.good() && (smbls[ch] == 1)){
				buf += ch;
				file.get(ch);
				count++;
			}
			lexCode = TT.const_tab_search(stoi(buf));
			break;
		case 2: //identifier
			pos = count;
			while (file.good() && (smbls[ch] == 1|| smbls[ch] == 2)) {
				buf += ch;
				file.get(ch);
				count++;
			}
			
			if (int id = TT.key_tab_search(buf)) lexCode = id;
			else lexCode = TT.idn_tab_search(buf);
			break;
		case 3: //one-symbol delimiters
			lexCode = int(ch);
			buf += ch;
			file.get(ch);	
			pos=count++;
			break;
		case 5: //comment
			file.get(ch);
			pos=count;
			if (!file) {
				(*this).er_push("*)expected but eof found", row, count);
				(*this).push(0, row, pos,buf);
				break;
			}
			else {
				file.get(ch);
				count++;
				while (ch != ')') {
					while (file.good() && ch != '*') {
						file.get(ch);
						count++;
					}
					if (!file) {
						(*this).er_push("*)expected but eof found", row, count);
						(*this).push(0, row, pos,buf);
						break;
					}
					else {
						file.get(ch);
						count++;
					}
				}
				if (ch = ')')
					SuppressOutput = true;
				if (file.good()) {
					file.get(ch);
					count++;
				}
			}
			break;
		case 41:
			pos = count;
			buf += ch;
			if (file.good()) {
				file.get(ch);
				count++;
			}
			if (ch == '=') {
				buf += ch;
				lexCode = TT.key_tab_search(buf);
				if (file.good()) {
					file.get(ch);
					count++;
				}
			}
			else {
				lexCode = int(':');
			}
			break;
		case 42:
			pos = count;
			buf += ch;
			if (file.good()) {
				file.get(ch);
				count++;
				if (ch == '.' && file.good()) {
					buf += ch;
					lexCode = TT.key_tab_search(buf);
					file.get(ch);
					count++;
				}
				else {
					lexCode = int('.');
				}
			}	
			break;
		default:
			buf = "Wrong symbol: ";
			buf+=ch;
			file.get(ch);
			pos = count++;
			break;
		}


		if (!SuppressOutput) {			
				(*this).push(lexCode, row, pos,buf);
			if (lexCode == 0)
				(*this).er_push(buf, row, pos);
		}
	}
	if (!log.empty()) {
		throw(log);
	}
	TT.update_tables();
	(*this).create_lex_file();
}