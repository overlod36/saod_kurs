#pragma once
#include<iostream>
#include "file.h"
using namespace std;

class catalog {
private:
	string name;
	file* head_f;
	catalog* next;
public:
	catalog();
	catalog(string name);

	~catalog();

	void set_name(string name);
	void set_pointer_head(file* st);
	void set_pointer_next(catalog* st, catalog* next_st);
	string get_name();
	catalog* get_pointer_next();
	file* get_head();

	bool checking_empty_cat(file* st);
	bool checking_full_cat(file* st);

	
	void add_file_f(file* st);
	bool add_file(file* st);
	void delete_file(string name);
	file* searching_file(string name, int ch, int og);
	void direct_show();
	void reverse_show();
};