#include<iostream>
#include<sstream>
#include<cstdlib>
#include<fstream>
#include "disk_device.h"
#include "catalog.h"
using namespace std;


int disk_int_input()
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

string disk_str_input() {
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

disk_device::disk_device() {
	this->name = "";
	this->head_c = NULL;
}

disk_device::disk_device(string name) {
	this->name = name;
	this->head_c = NULL;
}

disk_device::~disk_device() {
	catalog* current = this->get_pointer_head(), *st;
	while (current != NULL) {
		st = current;
		current = current->get_pointer_next();
		delete st;
	}
}


bool disk_device::checking_empty_disk(catalog* st) {
	if (st == NULL)
		return true;
	else
		return false;
}

bool disk_device::checking_full_disk(catalog* st) {
	if (st == NULL)
		return true;
	else
		return false;
}

void disk_device::set_name(string name) {
	this->name = name;
}

void disk_device::set_pointer_head(catalog* st) {
	this->head_c = st;
}

string disk_device::get_name() {
	return this->name;
}

catalog* disk_device::get_pointer_head() {
	return this->head_c;
}

void disk_device::show() {
	cout << "Дисковое устройство [" << this->name << "]" << endl;
	if (checking_empty_disk(this->head_c)) {
		cout << "Дисковое устройство пустое..." << endl;
	}
	else {
		cout << "[Каталоги]" << endl;
		catalog* current = this->head_c;
		while (current != NULL) {
			current->direct_show();
			current = current->get_pointer_next();
		}
		cout << endl;
	}
}

bool disk_device::add_catalog(catalog* st) {
	if (checking_empty_disk(this->get_pointer_head())) {
		this->set_pointer_head(st);
		this->get_pointer_head()->set_pointer_next(this->get_pointer_head(), NULL);
	}
	else {
		catalog* current = this->get_pointer_head(), *previous = NULL;
		cout << "Введите название опорного каталога -> ";
		string op_name = disk_str_input();
		while (current != NULL) {
			if (current->get_name() == op_name)
				break;
			else if (current->get_name() == st->get_name())
				return false;
			previous = current;
			current = current->get_pointer_next();
		}
		if (current == NULL) {
			cout << "Каталог с таким именем не найден..." << endl;
			return false;
		}
		else {
			cout << "Каким образом добавить новый каталог, до(0) или после(1) опорного? -> ";
			int ch = disk_int_input();
			if (ch <= 0) {
				st->set_pointer_next(st, current);
				if (previous == NULL)
					this->set_pointer_head(st);
				else
					previous->set_pointer_next(previous,st);
				///голова каталога
			}
			else {
				st->set_pointer_next(st, current->get_pointer_next());
				current->set_pointer_next(current, st);
			}
		}
	}
	return true;
}

void disk_device::add_catalog_f(catalog* st) {
	if (checking_empty_disk(this->get_pointer_head())) {
		this->set_pointer_head(st);
		this->get_pointer_head()->set_pointer_next(this->get_pointer_head(), NULL);
	}
	else {
		catalog* cur = this->get_pointer_head(), *st_add;
		while (1) {
			if (cur->get_pointer_next() == NULL) {
				st_add = cur;
				break;
			}
			cur = cur->get_pointer_next();
		}
		st->set_pointer_next(st, st_add->get_pointer_next());
		st_add->set_pointer_next(st_add, st);
	}
}

bool disk_device::add_catalog_from_file(string line) {
	string name, last, catalog_inf;
	int catalog_index = line.find('{');
	if (catalog_index == -1) {
		cout << "Произошла ошибка..." << endl;
		return false;
	}
	else {
		name = line.substr(0, catalog_index);
		catalog* st = new catalog();
		st->set_name(name);
		last = line.substr(line.length()-1, 1);
		if (last != "}") {
			cout << "Произошла ошибка..." << endl;
			return false;
		}
		catalog_inf = line.substr(catalog_index + 1, line.length() - name.length() - 2);
		while (catalog_inf.length() != 0) {
			int st_index = catalog_inf.find(',');
			string cat_str;
			if (st_index == catalog_inf.length() - 1 || st_index == 0) {
				cout << "Произошла ошибка..." << endl;
				return false;
			}
			if (st_index == -1) {
				cat_str = catalog_inf;
				catalog_inf = "";
			}
			else {
				cat_str = catalog_inf.substr(0, st_index);
				catalog_inf = catalog_inf.substr(st_index + 1, catalog_inf.length() - 1);
			}
			int st_index_l = cat_str.find(':');
			if (st_index_l == -1) {
				cout << "Произошла ошибка..." << endl;
				return false;
			}
			string name = cat_str.substr(0, st_index_l);
			string size = cat_str.substr(st_index_l + 1, cat_str.length() - st_index_l - 1);
			if (name.empty() || size.empty()) {
				cout << "Произошла ошибка..." << endl;
				return false;
			}
			file* temp = new file();
			temp->set_name(name);
			temp->set_size(atoi(size.c_str()));
			st->add_file_f(temp);
		}
		add_catalog_f(st);
		return true;
	}
}

void disk_device::download_in_cfile(string file_path) {
	ofstream fl;
	fl.open(file_path);
	if (fl.is_open()) {
		fl << get_name() << endl;
		if (get_pointer_head() != NULL) {
			catalog* cur_c = get_pointer_head();
			while (cur_c != NULL) {
				fl << cur_c->get_name() + "{";
				file* cur_f = cur_c->get_head()->get_pointer_next();
				if (cur_c->checking_empty_cat(cur_f)) {
					fl << "}" << endl;
				}
				else if (cur_f != cur_c->get_head()) {
					while (1) {
						if (cur_f->get_pointer_next() == cur_c->get_head()) {
							fl << cur_f->get_name() + ":" + to_string(cur_f->get_size()) + "}" << endl;
							break;
						}
						else {
							fl << cur_f->get_name() + ":" + to_string(cur_f->get_size()) + ",";
						}
						cur_f = cur_f->get_pointer_next();
					}
				}
				else {
					fl << "}" << endl;
				}
				cur_c = cur_c->get_pointer_next();
			}
			fl.close();
		}
	}
	else {
		fl.close();
	}
	cout << "Содержимое дискового устройства [" << this->get_name() << "] загружено..." << endl;
}

catalog* disk_device::searching_catalog(string name, int og) {
	if (checking_empty_disk(get_pointer_head())) {
		return NULL;
	}
	else {
		int j = 1;
		catalog* current = this->head_c;
		while (current != NULL) {
			if (current->get_name() == name) {
				if (og == 1)
					cout << "Каталог " << current->get_name() << " находится на позиции " << j << endl;
				return current;
			}
			j += 1;
			current = current->get_pointer_next();
		}
		return NULL;
	}
}

void disk_device::delete_catalog(string name) {
	catalog* st = NULL;
	if (!checking_empty_disk(head_c)) {
		if (this->head_c->get_name() == name) {
			st = this->head_c;
			this->head_c = st->get_pointer_next();
		}
		else {
			catalog* prev = NULL, * temp = this->head_c;
			while (temp != NULL) {
				if (temp->get_name() == name) {
					st = temp;
					temp->set_pointer_next(prev, temp->get_pointer_next());
					break;
				}
				prev = temp;
				temp = temp->get_pointer_next();
			}
		}
		delete st;
	}
	else {
		cout << "Удаление невозможно... В дисковом устройстве ничего нет..." << endl;
	}
	
}

void disk_device::download_from_cfile(string file_path) {
	fstream fl(file_path);
	while (!fl.is_open()) {
		cout << "Файл не найден..." << endl;
		cout << "Введите путь до файла -> ";
		cin >> file_path;
		fl.open(file_path);
	}
	catalog* current = this->get_pointer_head(), *st_p;
	while (current != NULL) {
		st_p = current;
		current = current->get_pointer_next();
		delete_catalog(st_p->get_name());
	}
	string st;
	if (getline(fl, st) && !st.empty()) {
		this->set_name(st);
		while (getline(fl, st)) {
			if (add_catalog_from_file(st) == false) {
				cout << "Произошла ошибка прочтения данных из файла..." << endl;
				fl.close();
				return;
			}
		}
		cout << "Содержимое дискового устройства [" << this->get_name() << "] загружено..." << endl;
	}
}

