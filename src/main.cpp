// main.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
#include <iostream>
#include <thread>
#include <ctime>
#include "Include_global.h"

//=================================================================================================================
//brief         System start point
//param[in]     Null
//return        0: finish execution
//=================================================================================================================
int main()
{
	clock_t start, stop;
	Pool_Class pool(MEMORY_POOL_SIZE);
	_Hamburger* burger[BURGER_COOK_CENTER_NUM];
	for (int i = 0; i < BURGER_COOK_CENTER_NUM; i++)
	{
		burger[i] = (_Hamburger*)pool.pool_alloc(sizeof(_Hamburger));
		if (burger == NULL)
		{
			std::cout << "fail to allocate memory\n";
			system("pause");
			return 0;
		}
		else
		{
			Initial_Hamburger(burger[i]);
		}
	}

	std::cout << "Hamburger Cook Center number: " << BURGER_COOK_CENTER_NUM << "\n";
	std::cout << "At every cook center: Start making Hamburger with Bread: " << BREAD_NUM << "pcs and Beef:" << BEEF_NUM << "pcs\n";

	std::thread check[BURGER_COOK_CENTER_NUM];
	std::thread cook[BURGER_COOK_CENTER_NUM];
	start = clock();
	for (int i = 0; i < BURGER_COOK_CENTER_NUM; i++)
	{
		check[i] = std::thread(Check_Hamburger, burger[i], i);
		cook[i] = std::thread(Cook_Hamburger, burger[i]);
	}
	for (int i = 0; i < BURGER_COOK_CENTER_NUM; i++)
	{
		cook[i].join();
		check[i].join();
	}

	stop = clock();
	std::cout << "\nSpend time:" << ((double)stop - (double)start) / CLOCKS_PER_SEC << std::endl;

	for (int i = 0; i < BURGER_COOK_CENTER_NUM; i++)
	{
		pool.pool_recycle((char**)&burger[i]);
	}
	pool.pool_relocate();

	system("pause");
	return 0;
}


// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案