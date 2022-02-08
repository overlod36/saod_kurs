#include<iostream>
#include "file.h"
using namespace std;

file::file() {
	this->name = "";
	this->memory_size = -1;
	this->next = NULL;
	this->prev = NULL;
}

file::file(string name, int size) {
	this->name = name;
	this->memory_size = size;
	this->next = NULL;
	this->prev = NULL;
}

file::~file() {

}

void file::set_name(string name) {
	this->name = name;
}

string file::get_name() {
	return this->name;
}

void file::set_size(int size) {
	this->memory_size = size;
}

int file::get_size() {
	return this->memory_size;
}

void file::set_pointer_next(file* st, file* next_st) {
	st->next = next_st;
}

void file::set_pointer_prev(file* st, file* prev_st) {
	st->prev = prev_st;
}

file* file::get_pointer_next() {
	return this->next;
}

file* file::get_pointer_prev() {
	return this->prev;
}
