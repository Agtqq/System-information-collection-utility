#include "functions.h"
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


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "wbemuuid.lib")

using namespace std;

void wait_for_enter()
{
    cout << "Нажмите Enter, чтобы вернуться в меню" << endl;
    cin.ignore();
    cin.get();
}

void print_running_processes()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses; //массив идентефикаторов проц, 
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) //сохраняет все идентефик в массиве
    {
        cerr << "Ошибка при перечислении процессов" << std::endl;
        return;
    }

    cProcesses = cbNeeded / sizeof(DWORD); //кол-во процессов

    for (DWORD i = 0; i < cProcesses; i++)
    {
        TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]); //открытие пр по идентефик (для получ инф и чтение его из вирт памяти)

        if (NULL != hProcess)
        {
            HMODULE hModule;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) //модуль процесса
            {
                GetModuleBaseName(hProcess, hModule, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)); //имя модуля
            }

            _tprintf(TEXT("%u\t%s\n"), aProcesses[i], szProcessName); //вывод пользователю

            CloseHandle(hProcess); //закрытие дескриптора процесса
        }
    }
    wait_for_enter();
}

void print_disk_space()
{
    ULARGE_INTEGER free_bytes_available, total_number_of_bytes, total_number_of_free_bytes;
    if (GetDiskFreeSpaceEx(NULL, &free_bytes_available, &total_number_of_bytes, &total_number_of_free_bytes))
    {
        wcout << L"Память на диске на диске: " << total_number_of_bytes.QuadPart / (1024 * 1024 * 1024) << L" GB" << endl;
        wcout << L"Свободное пространство на диске: " << free_bytes_available.QuadPart / (1024 * 1024 * 1024) << L" GB" << endl;
    }
    else
    {
        cerr << "Ошибка при получении информации" << endl;
    }
    wait_for_enter();
}

void print_memory_status()
{
    MEMORYSTATUSEX memoryStatus; //структура для получения информации о памяти
    memoryStatus.dwLength = sizeof(memoryStatus);
    if (GlobalMemoryStatusEx(&memoryStatus))
    {
        cout << "Количество ОП: " << memoryStatus.ullTotalPhys << " байт" << endl;
        cout << "Доступная ОП: " << memoryStatus.ullAvailPhys << " байт" << endl;
    }
    else
    {
        cerr << "Ошибка при поучении информации" << endl;
    }
    wait_for_enter();
}

void print_number_of_processor_cores()
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    if (system_info.dwNumberOfProcessors == 0)
    {
        cerr << "Ошибка при поучении информации" << endl;
    }
    else
    {
        wcout << L"Количество ядер процессора: " << system_info.dwNumberOfProcessors << endl;
    }
    wait_for_enter();
}

void print_processor_architecture()
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    wcout << L"Архитектура: ";
    switch (system_info.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        wcout << L"x64" << endl;
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        wcout << L"x86" << endl;
        break;
    default:
        wcout << L"неизвестно" << endl;
        break;
    }

    wait_for_enter();
}

void print_user_name()
{
    TCHAR user_name[UNLEN + 1]; //UNLEN - макс длинна имени пользователя
    DWORD size = UNLEN + 1;
    if (GetUserName(user_name, &size))
    {
        wcout << L"Имя пользователя: " << user_name << endl;
    }
    else
    {
        cerr << "Ошибка при получении информации." << endl;
    }

    wait_for_enter();

}

void print_computer_name()
{
    TCHAR computer_name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerName(computer_name, &size))
    {
        wcout << L"Имя компьютера: " << computer_name << endl;
    }
    else
    {
        cerr << "Ошибка при поучении информации." << endl;
    }

    wait_for_enter();
}


void print_cpu_load()
{
    FILETIME idle_time, kernel_time, user_time; //структура для представления времени 
    ULARGE_INTEGER prev_idle_time, prev_kernel_time, prev_user_time;
    prev_idle_time.QuadPart = 0;
    prev_kernel_time.QuadPart = 0;
    prev_user_time.QuadPart = 0;
    cout << "Для возвращения в меню нажмите Q " << endl;
    bool exit = false;
    while (!exit)
    {
        if (!GetSystemTimes(&idle_time, &kernel_time, &user_time)) //содержат информацию о времени работы системы, ядра процессора и пользовательского процессора
        {
            cerr << "Ошибка при поучении информации." << endl;
            return;
        }

        ULARGE_INTEGER current_idle_time, current_kernel_time, current_user_time;
        current_idle_time.LowPart = idle_time.dwLowDateTime;
        current_idle_time.HighPart = idle_time.dwHighDateTime;
        current_kernel_time.LowPart = kernel_time.dwLowDateTime;
        current_kernel_time.HighPart = kernel_time.dwHighDateTime;
        current_user_time.LowPart = user_time.dwLowDateTime;
        current_user_time.HighPart = user_time.dwHighDateTime;


        //расчет разницы между текущим временем и прошлым
        ULONGLONG idle_ticks = current_idle_time.QuadPart - prev_idle_time.QuadPart; //количество тактов, в которых процессор находился в состоянии бездействия
        ULONGLONG kernel_ticks = current_kernel_time.QuadPart - prev_kernel_time.QuadPart;
        ULONGLONG user_ticks = current_user_time.QuadPart - prev_user_time.QuadPart;
        ULONGLONG total_ticks = kernel_ticks + user_ticks; //общее количество тактов, в которых процессор был занят выполнением задач

        double cpu_load = (1.0 - static_cast<double>(idle_ticks) / total_ticks) * 100.0;  //вычисление нагрузки
        cout << "Нагрузка процессора: " << cpu_load << "%" << endl;

        prev_idle_time = current_idle_time;
        prev_kernel_time = current_kernel_time;
        prev_user_time = current_user_time;



        if (GetAsyncKeyState('Q') & 0x8000)
        {
            exit = true;
        }
        Sleep(1000);


    }
}
