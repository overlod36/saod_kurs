#include<iostream>
#include<string>
#include "catalog.h"
#include "file.h"
using namespace std;


int catalog_int_input()
{
	int num;
	while (1)
	{
		cin >> num;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "Неверный ввод..." << endl;
			continue;
		}
		cin.ignore(32767, '\n');
		if (cin.gcount() > 1)
		{
			cout << "Неверный ввод..." << endl;
			continue;
		}
		break;
	}
	return num;
}

bool catalog::checking_empty_cat(file* st) {
	if (st == NULL)
		return true;
	else
		return false;
}

bool catalog::checking_full_cat(file* st) {
	if (st == NULL)
		return true;
	else
		return false;
}

string catalog_str_input() {
	string name;
	while (1) {
		getline(cin, name);
		if (name.empty()) {
			cout << "Неверный ввод..." << endl;
			cout << "-> ";
		}
		else
			return name;
	}
}

catalog::catalog() {
	this->name = "";
	this->head_f = new file();
	this->next = NULL;
}

catalog::catalog(string name) {
	this->name = name;
	this->head_f = new file();
	this->next = NULL;
}

catalog::~catalog() {
	if (!checking_empty_cat(head_f->get_pointer_next())) {
		file* current = head_f->get_pointer_next();
		while (current != head_f) {
			file* temp = current;
			current = current->get_pointer_next();
			delete temp;
		}
	}
}

void catalog::set_name(string name) {
	this->name = name;
}

void catalog::set_pointer_head(file* st) {
	this->head_f = st;
}

void catalog::set_pointer_next(catalog* st, catalog* next_st) {
	st->next = next_st;
}

string catalog::get_name() {
	return this->name;
}

file* catalog::get_head() {
	return this->head_f;
}

catalog* catalog::get_pointer_next() {
	return this->next;
}

void catalog::direct_show() {
	cout << "------> Каталог [" << this->name << "]" << endl;
	file* cur = this->head_f->get_pointer_next();
	if (cur == NULL)
		cout << "----------> Каталог пуст..." << endl;
	else {
		cout << "----------> [Файлы]" << endl;
		while (cur != head_f) {
			cout << "-------------------> Имя файла [" << cur->get_name() << "], его размер [" << cur->get_size() << "]"<< endl;
			cur = cur->get_pointer_next();
		}
	}
}

void catalog::reverse_show() {
	cout << "------> Каталог [" << this->name << "]" << endl;
	file* current = this->head_f->get_pointer_prev();
	if (checking_empty_cat(head_f->get_pointer_next()))
		cout << "----------> Каталог пуст..." << endl;
	else {
		cout << "----------> [Файлы]" << endl;
		do {
			cout << "-------------------> Имя файла [" << current->get_name() << "], его размер [" << current->get_size() << "]" << endl;
			current = current->get_pointer_prev();
		} while (current != head_f);
	}
}

file* catalog::searching_file(string name, int ch, int og) {
	if (ch > 0) {
		int j = 1;
		if (checking_empty_cat(get_head()->get_pointer_next())) {
			return NULL;
		}
		else {
			file* st = this->head_f->get_pointer_next();
			while (st != head_f) {
				if (name == st->get_name()) {
					if (og == 1) {
						cout << "Файл " << st->get_name() << " находится в каталоге " << this->get_name() << ", на позиции " << j << endl;
					}
					return st;
				}
				j += 1;
				st = st->get_pointer_next();
			}
			return NULL;
		}
	}
	else {
		if (checking_empty_cat(get_head()->get_pointer_prev())) {
			return NULL;
		}
		else {
			int j = 1;
			file* ct = this->head_f->get_pointer_next();
			while (ct != head_f) {
				j += 1;
				ct = ct->get_pointer_next();
			}
			file* st = this->head_f->get_pointer_prev();
			while (st != head_f) {
				if (name == st->get_name()) {
					if (og == 1) {
						cout << "Файл " << st->get_name() << " находится в каталоге " << this->get_name() << ", на позиции " << j << endl;
					}
					
					return st;
				}
				j -= 1;
				st = st->get_pointer_prev();
			}
		}
	}
	
}

void catalog::delete_file(string name){
	if (checking_empty_cat(get_head()->get_pointer_next())) {
		cout << "Удаление невозможно... В каталоге ничего нет..." << endl;
	}
	else {
		file* temp = head_f->get_pointer_next();
		while (temp != head_f) {
			if (temp->get_name() == name) {
				temp->set_pointer_next(temp->get_pointer_prev(), temp->get_pointer_next());
				temp->set_pointer_prev(temp->get_pointer_next(), temp->get_pointer_prev());
				delete temp;
				break;
			}
			temp = temp->get_pointer_next();
		}
	}
}

void catalog::add_file_f(file* st) {
	if (checking_empty_cat(get_head()->get_pointer_next())) {
		get_head()->set_pointer_next(get_head(), st);
		get_head()->set_pointer_prev(get_head(), st);
		st->set_pointer_next(st, get_head());
		st->set_pointer_prev(st, get_head());
	}
	else {
		file* cur = get_head()->get_pointer_next(), *st_add;
		while (1) {
			if (cur->get_pointer_next() == get_head()) {
				st_add = cur;
				break;
			}
			cur = cur->get_pointer_next();
		}
		st->set_pointer_next(st, st_add->get_pointer_next());
		st->set_pointer_prev(st, st_add);
		st->get_pointer_next()->set_pointer_prev(st->get_pointer_next(), st);
		st_add->set_pointer_next(st_add, st);
	}
}

bool catalog::add_file(file* st) {
	if (checking_empty_cat(get_head()->get_pointer_next())) {
		get_head()->set_pointer_next(get_head(),st);
		get_head()->set_pointer_prev(get_head(), st);
		st->set_pointer_next(st, get_head());
		st->set_pointer_prev(st, get_head());
	}
	else {
		file* cur = get_head()->get_pointer_next();
		cout << "Введите название опорного файла -> ";
		string op_name = catalog_str_input();
		while (cur != NULL) {
			if (cur->get_name() == op_name) {
				break;
			}
			else if (cur->get_name() == st->get_name()) {
				return false;
			}
			cur = cur->get_pointer_next();
		}
		if (cur == NULL) {
			cout << "Файл с таким именем не найден..." << endl;
			return false;
		}
		else {
			cout << "Каким образом добавить новый файл, до(0) или после(1) опорного? -> ";
			int ch = catalog_int_input();
			if (ch <= 0) {
				st->set_pointer_next(st, cur);
				cur->get_pointer_prev()->set_pointer_next(cur->get_pointer_prev(),st);
				st->set_pointer_prev(st, cur->get_pointer_prev());
				cur->set_pointer_prev(cur, st);
			}
			else {
				st->set_pointer_next(st, cur->get_pointer_next());
				st->set_pointer_prev(st, cur);
				st->get_pointer_next()->set_pointer_prev(st->get_pointer_next(), st);
				cur->set_pointer_next(cur, st);
			}
		}
		
	}
	return true;
}
