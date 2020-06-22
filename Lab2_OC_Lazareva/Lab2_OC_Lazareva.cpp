// Lab2_OC_Lazareva.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <string.h>

using namespace std;

CRITICAL_SECTION g_cs;

DWORD WINAPI ThreadFunc2(PVOID pvParam)
{
    char* tmp = (char*)pvParam;
    DWORD dwResult = 0;
    char s[] = "HeLLo";
    char s1[] = "HoUSe";
    char s2[] = "MOUse";

    while (true)
    {
        int random = rand() % 3 + 1;
        EnterCriticalSection(&g_cs);

        switch (random)
        {
        case 1:
            strcpy(tmp, s);
            break;
        case 2:
            strcpy(tmp, s1);
            break;
        case 3:
            strcpy(tmp, s2);
            break;
        }
        LeaveCriticalSection(&g_cs);
        std::cout << "Поток 2 активен, строка изменена. " << std::endl;
        Sleep(300);
    }
    return dwResult;
}

DWORD WINAPI ThreadFunc1(PVOID pvParam)
{
    char* tmp = (char*)pvParam;
    DWORD dwResult = 0;
    
    while (true)
    {
        std::cout << "Ваша строка: " << tmp << std::endl;
        int a = 0, b = 0;

        EnterCriticalSection(&g_cs);

        for (int i = 0; i < 6; i++)
        {
            if ((tmp[i] > 'A') && (tmp[i] < 'Z')) a++;
            if ((tmp[i] > 'a') && (tmp[i] < 'z')) b++;
        }

        LeaveCriticalSection(&g_cs);

        std::cout << "Количество заглавных: " << a << "\nКоличество строчных: " << b << std::endl;
        std::cout << "Поток 1 активен, строка изменена. " << std::endl;

        Sleep(500);
    }
    return dwResult;
}

int main()
{
    setlocale(LC_CTYPE, "rus");
    InitializeCriticalSection(&g_cs);

    char s[] = "Hello";
    const char* str = s;
    int choice;

    // 2 поток
    DWORD dwTh2;
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunc2, (LPVOID)str, CREATE_SUSPENDED, &dwTh2);

    // 1 поток
    DWORD dwTh1;
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)str, CREATE_SUSPENDED, &dwTh1);
    
    for (;;)
    {
        std::cout << "Какое действие Вы хотите выполнить?" << std::endl;
        std::cout << "1 - Возобновление потока 1." << std::endl;
        std::cout << "2 - Приостановка потока 1." << std::endl;
        std::cout << "3 - Возобновление потока 2." << std::endl;
        std::cout << "4 - Приостановка потока 2." << std::endl;
        std::cout << "5 - Уничтожение потока 1." << std::endl;
        std::cout << "6 - Уничтожение потока 2." << std::endl;
        std::cout << "7 - Создать поток 1." << std::endl;
        std::cout << "8 - Создать поток 2." << std::endl << std::endl;


        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            ResumeThread(hThread1);
        }
        break;
        case 2:
        {
            SuspendThread(hThread1);
        }
        break;
        case 3:
        {
            ResumeThread(hThread2);
        }
        break;
        case 4:
        {
            SuspendThread(hThread2);
        }
        break;
        case 5:
        {
            TerminateThread(hThread1, 0);
        }
        break;
        case 6:
        {
            TerminateThread(hThread2, 0);
        }
        break;
        case 7:
        {
            HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)str, 0, &dwTh1);
        }
        break;
        case 8:
        {

            HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunc2, (LPVOID)str, 0, &dwTh2);
        }
        break;

        default:
            std::cout << "Выберите один из вариантов!" << std::endl;
            break;
        }
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"