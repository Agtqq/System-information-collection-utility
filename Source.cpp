#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <atlbase.h>
#include <comdef.h>
#include <wbemidl.h>
#include <lmcons.h>
#include <vector>
#include <psapi.h>
#include <tchar.h>
#include <dxgi.h>
#include <locale>
#include <limits>
#include "functions.h"


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "wbemuuid.lib")

using namespace std;



void run_menu()
{
    int choice = -1;

    while (choice != 0)
    {
       cout << "=== Меню ===" << endl;
       cout << "1. Загрузка процессора" << endl;
       cout << "2. Имя компьютера" << endl;
       cout << "3. Имя пользователя" << endl;
       cout << "4. Архитектура процессора" << endl;
       cout << "5. Количество ядер процессора" << endl;
       cout << "6. Информация о памяти" << endl;
       cout << "7. Информация о диске" << endl;
       cout << "8. Информация о запущенных процессах" << endl;
       cout << "0. Выход" << endl;
       cout << "Введите номер выбранной функции: ";
       cin >> choice;

        switch (choice)
        {
        case 1:
            print_cpu_load();
            break;
        case 2:
            print_computer_name();
            
            break;
        case 3:
            print_user_name();
            break;
        case 4:
            print_processor_architecture();
            break;
        case 5:
            print_number_of_processor_cores();
            break;
        case 6:
            print_memory_status();
            break;
        case 7:
            print_disk_space();
            break;
        case 8:
            print_running_processes();
            break;
        case 0:
            cout << "Программа завершена." << endl;
            break;
        default:
           cout << "Некорректный выбор. Пожалуйста, выберите снова." << endl;
        }

       cout << endl;
        system("cls");
        
    }  

}


int main()
{
    setlocale(LC_ALL, "Russian");
    run_menu();
   
    return 0;
}
