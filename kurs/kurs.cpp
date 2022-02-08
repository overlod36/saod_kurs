#include<iostream>
#include<fstream>
#include<string>
#include <windows.h>
#include"disk_device.h"
#include"catalog.h"
#include"file.h"
using namespace std;

disk_device* disk1;

int menu_int_input()
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

string menu_str_input() {
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


void menu_searching() {
    int ch;
    if (disk1->get_pointer_head() == NULL) {
        cout << "В дисковом устройстве нечего искать..." << endl;
        return;
    }
    else {
        while (1) {
            cout << "[МЕНЮ поиска каталога/файла]:" << endl;
            cout << "[1]-Поиск каталога-----------|" << endl;
            cout << "[2]-Поиск файла--------------|" << endl;
            cout << "[0]-Выход из меню поиска-----|" << endl;
            cout << "-> ";
            ch = menu_int_input();
            if (ch == 1) {
                string name_cat;
                catalog* st;
                cout << "Введите имя каталога, который надо найти -> ";
                name_cat = menu_str_input();
                st = disk1->searching_catalog(name_cat, 1);
                if (st == NULL)
                    cout << "Каталог не был найден..." << endl;
            }
            else if (ch == 2) {
                string name_cat, name_file;
                catalog* cat_st; file* file_st;
                while (1) {
                    cout << "Введите название каталога, в котором будет искаться файл -> ";
                    name_cat = menu_str_input();
                    cat_st = disk1->searching_catalog(name_cat, 1);
                    if (cat_st == NULL) {
                        cout << "Такого каталога нет в дисковом устройстве..." << endl;
                        continue;
                    }
                    else
                        break;
                }
                cout << "Введите имя файла, который надо найти -> ";
                name_file = menu_str_input();
                cout << "Каким образом производить поиск, прямым(1) или обратным(0) образом -> ";
                ch = menu_int_input();
                if (ch < 0) {
                    file_st = cat_st->searching_file(name_file, 0, 1);
                }
                else {
                    file_st = cat_st->searching_file(name_file, 1, 1);
                }
                if (file_st == NULL)
                    cout << "Файл не был найден..." << endl;
                
            }
            else if (ch == 0)
                return;
            else {
                cout << "Неверный ввод... Попробуйте снова..." << endl;
            }
        }
    }
}

void menu_download() {
    string file_path;
    int ch;
    while (1) {
        cout << "[МЕНЮ загрузки/выгрузки файла]:" << endl;
        cout << "[1]-Загрузка содержимого из файла---------|" << endl;
        cout << "[2]-Выгрузка содержимого в файл-----------|" << endl;
        cout << "[0]-Выход из меню загрузки/выгрузки-------|" << endl;
        cout << "-> ";
        ch = menu_int_input();
        if (ch == 1) {
            cout << "Введите путь до файла -> ";
            cin >> file_path;
            disk1->download_from_cfile(file_path);
        }
        else if (ch == 2) {
            cout << "Введите путь до файла -> ";
            cin >> file_path;
            disk1->download_in_cfile(file_path);
        }
        else if (ch == 0) {
            return;
        }
        else {
            cout << "Неверный ввод... Попробуйте снова..." << endl;
        }
    }
}

void menu_add() {
    int ch;
    while (1) {
        cout << "[МЕНЮ добавления]:" << endl;
        cout << "[1]-Добавление каталога---------|" << endl;
        cout << "[2]-Добавление файла------------|" << endl;
        cout << "[0]-Выход из меню добавления----|" << endl;
        cout << "-> ";
        ch = menu_int_input();
        if (ch == 1) {
            string name_cat;
            cout << "Введите название нового каталога -> ";
            name_cat = menu_str_input();
            if (disk1->searching_catalog(name_cat, 0) == NULL) {
                catalog* st = new catalog();
                st->set_name(name_cat);
                if (disk1->add_catalog(st))
                    cout << "Добавление произошло успешно!" << endl;
                else
                    cout << "Добавление отменено..." << endl;
            }
            else {
                cout << "Такой каталог уже есть..." << endl;
            }
            
        }
        else if (ch == 2) {
            if (disk1->get_pointer_head() == NULL)
                cout << "Невозможно добавить файл, пока не будет добавлен хотя бы один каталог..." << endl;
            else {
                catalog* cat; string str_cat, name;
                bool catchin = false;
                while (1) {
                    cout << "Введите название каталога, в который будет добавлен новый файл -> ";
                    str_cat = menu_str_input();
                    cat = disk1->searching_catalog(str_cat, 0);
                    if (cat == NULL) {
                        cout << "Такого каталога нет в дисковом устройстве..." << endl;
                        continue;
                    }
                    else
                        break;
                }
                cout << "Введите название нового файла -> ";
                name = menu_str_input();
                if (cat->searching_file(name, 1, 0) == NULL) {
                    cout << "Введите размер нового файла -> ";
                    int size = menu_int_input();
                    file* temp = new file();
                    temp->set_name(name);
                    temp->set_size(size);
                    cat->add_file(temp);
                }
                else
                    cout << "Такой файл уже есть..." << endl;
            }
        }
        else if (ch == 0) {
            return;
        }
        else {
            cout << "Неверный ввод... Попробуйте снова..." << endl;
        }
    }
}

void delete_menu() {
    if (disk1->get_pointer_head() == NULL) {
        cout << "В дисковом устройстве нечего удалять..." << endl;
        return;
    }
    else {
        int ch;
        while (1) {
            cout << "[МЕНЮ удаления]:" << endl;
            cout << "[1]-Удаление каталога---------------------------------------|" << endl;
            cout << "[2]-Удаление файла------------------------------------------|" << endl;
            cout << "[3]-Удаление всего содержимого дискового устройства---------|" << endl;
            cout << "[0]-Выход из меню удаления----------------------------------|" << endl;
            cout << "-> ";
            ch = menu_int_input();
            if (ch == 1) {
                catalog* temp;
                string name;
                while (1) {
                    cout << "Введите название каталога, который надо удалить -> ";
                    name = menu_str_input();
                    temp = disk1->searching_catalog(name, 0);
                    if (temp != NULL)
                        break;
                    else {
                        cout << "Ничего не найдено..." << endl;
                    }
                }
                disk1->delete_catalog(name);
                cout << "Произошло удаление каталога..." << endl;
            }
            else if (ch == 2) {
                catalog* temp_c;
                string name_c, name_f;
                file* temp_f;
                while (1) {
                    cout << "Введите название каталога, в котором надо удалить файл -> ";
                    name_c = menu_str_input();
                    temp_c = disk1->searching_catalog(name_c, 0);
                    if (temp_c != NULL) {
                        if (temp_c->checking_empty_cat(temp_c->get_head()->get_pointer_next())) {
                            cout << "Файлов в каталоге нет..." << endl;
                            break;
                        }
                        else {
                            cout << "Введите название файла, который надо удалить -> ";
                            name_f = menu_str_input();
                            temp_f = temp_c->searching_file(name_f,1, 0);
                            if (temp_f != NULL)
                                break;
                            else {
                                cout << "Файл не найден..." << endl;
                                continue;
                            }
                        }
                           
                    }
                    else {
                        cout << "Каталог не найден..." << endl;
                        continue;
                    }
                }
                temp_c->delete_file(name_f);
            }
            else if (ch == 3) {
                catalog* current = disk1->get_pointer_head(), * st_p;
                while (current != NULL) {
                    st_p = current;
                    current = current->get_pointer_next();
                    disk1->delete_catalog(st_p->get_name());
                }
            }
            else if (ch == 0) {
                return;
            }
            else
                cout << "Неверный ввод..." << endl;
        }
    }
}

void show_menu() {
    if (disk1->get_pointer_head() == NULL) {
        cout << "В дисковом устройстве нечего выводить..." << endl;
        return;
    }
    else {
        int choice;
        while (1) {
            cout << "[МЕНЮ показа]:" << endl;
            cout << "[1]-Показать содержимое дискового устройства------|" << endl;
            cout << "[2]-Показать содержимое каталога------------------|" << endl;
            cout << "[0]-Выход из меню показа содержимого--------------|" << endl;
            cout << "-> ";
            choice = menu_int_input();
            if (choice == 1) {
                disk1->show();
            }
            else if (choice == 2) {
                catalog* cat;
                string cat_name;
                while (1) {
                    cout << "Введите название каталога, содержимое которого надо показать -> ";
                    cat_name = menu_str_input();
                    cat = disk1->searching_catalog(cat_name, 0);
                    if (cat == NULL) {
                        cout << "Такого каталога нет в дисковом устройстве..." << endl;
                        continue;
                    }
                    else
                        break;
                }
                cout << "Выберите, каким образом выводить содержимое каталога, прямым(1) или обратным(0) образом -> ";
                choice = menu_int_input();
                if (choice > 0)
                    cat->direct_show();
                else
                    cat->reverse_show();
            }
            else if (choice == 0) {
                return;
            }
            else
                cout << "Неверный ввод..." << endl;
        }
    }
}

int main()
{
    using namespace std;
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice; string name_disk; string file_path;
    disk1 = new disk_device();
    cout << "Введите название дискового устройства -> ";
    name_disk = menu_str_input();
    disk1->set_name(name_disk);
    while (1) {
        cout << "[МЕНЮ]:" << endl;
        cout << "Текущее дисковое устройство -> " << disk1->get_name() << endl;
        cout << "[1]-Переименование дискового устройства-----------------------------------------------------------------------|" << endl;
        cout << "[2]-Добавление каталога в дисковое устройство/Добавление файла в каталог--------------------------------------|" << endl;
        cout << "[3]-Удаление каталога из дискового устройства/Удаление файла из каталога--------------------------------------|" << endl;
        cout << "[4]-Показ содержимого дискового устройства--------------------------------------------------------------------|" << endl;
        cout << "[5]-Поиск каталога в дисковом устройстве/файла в каталоге-----------------------------------------------------|" << endl;
        cout << "[6]-Загрузка информации дискового устройства в файл/Загрузка информации из файла в дисковое устройство--------|" << endl;
        cout << "[0]-Выход из главного меню------------------------------------------------------------------------------------|" << endl;
        cout << "-> ";
        choice = menu_int_input();
        if (choice == 1) {
            string name_d;
            cout << "Введите новое имя дискового устройства -> ";
            name_d = menu_str_input();
            disk1->set_name(name_d);
        }
        else if (choice == 2) {
            menu_add();
        }
        else if (choice == 3) {
            delete_menu();
        }
        else if (choice == 4) {
            show_menu();
        }
        else if (choice == 5) {
            menu_searching();
        }
        else if (choice == 6) {
            menu_download();
        }
        else if (choice == 0) {
            cout << "Завершение работы..." << endl;
            break;
        }
        else {
            cout << "Неверный ввод, повторите попытку..." << endl;
        }
    }

}