#pragma once
#include<iostream>
using namespace std;

class file {
private:
	string name;
	int memory_size;
	file* next;
	file* prev;
public:
	file();
	file(string name, int size);

	~file();

	void set_name(string name);
	void set_size(int size);
	void set_pointer_prev(file* st, file* st_next);
	void set_pointer_next(file* st, file* st_prev);
	string get_name();
	int get_size();
	file* get_pointer_prev();
	file* get_pointer_next();
};