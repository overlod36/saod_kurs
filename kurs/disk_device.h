#pragma once
#include <iostream>
#include<fstream>
#include "catalog.h"
using namespace std;

class disk_device {
private:
	string name;
	catalog* head_c;
public:
	disk_device();
	disk_device(string name);

	~disk_device();

	void set_name(string name);
	void set_pointer_head(catalog* st);
	string get_name();
	catalog* get_pointer_head();
	bool checking_empty_disk(catalog* st);
	bool checking_full_disk(catalog *st);

	bool add_catalog(catalog* st);
	void add_catalog_f(catalog* st);
	bool add_catalog_from_file(string line);
	void download_from_cfile(string file_path);
	void download_in_cfile(string file_path);
	void delete_catalog(string name);
	catalog* searching_catalog(string name, int og);
	void show();
	//загрузка 
};