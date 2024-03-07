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
       cout << "=== ���� ===" << endl;
       cout << "1. �������� ����������" << endl;
       cout << "2. ��� ����������" << endl;
       cout << "3. ��� ������������" << endl;
       cout << "4. ����������� ����������" << endl;
       cout << "5. ���������� ���� ����������" << endl;
       cout << "6. ���������� � ������" << endl;
       cout << "7. ���������� � �����" << endl;
       cout << "8. ���������� � ���������� ���������" << endl;
       cout << "0. �����" << endl;
       cout << "������� ����� ��������� �������: ";
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
            cout << "��������� ���������." << endl;
            break;
        default:
           cout << "������������ �����. ����������, �������� �����." << endl;
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
