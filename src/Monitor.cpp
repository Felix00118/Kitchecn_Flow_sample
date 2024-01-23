#include <iostream>
#include <mutex>
#include "windows.h"
#include "Monitor.h"

//=================================================================================================================
// Global parameter
//=================================================================================================================
std::mutex gMutex;

//=================================================================================================================
//brief         Check the status and number of hamburger.
//param[in]     _Hamburger* burger is that will be monitored.
//return        Null
//=================================================================================================================
void Check_Hamburger(_Hamburger* burger, UINT Cook_center_id)
{
	uint32_t burger_num = 0;
	uint32_t check_cnt = 0;
	while (!burger->Integrated_finish_flag)
	{
#if (!SYNC_COOK)
        check_cnt++;
        Sleep(MONITOR_TIME);
        std::lock_guard<std::mutex> lock(gMutex);
        burger->Monitor_flag = 1;
        if (burger->Integrated > burger_num)
        {
        	burger_num = burger->Integrated;
        	std::cout << "\nCook Center[" << Cook_center_id << "]:" << std::endl;
        	std::cout << "Total Burger number:" << burger_num << std::endl;
        	std::cout << "Check Count No." << check_cnt << std::endl;
        }
        burger->Monitor_flag = 0;
        burger_num = burger->Integrated;

#elif (SYNC_COOK ==1)
		check_cnt++;
		Sleep(MONITOR_TIME);
		if (burger->Monitor_flag == 1)
		{
			std::lock_guard<std::mutex> lock(gMutex);

			burger->Monitor_flag = 0;
			if (burger->Integrated > burger_num)
			{
				burger_num = burger->Integrated;
				std::cout << "\nCook Center[" << Cook_center_id << "]:" << std::endl;
				std::cout << "Total Burger number:" << burger_num << std::endl;
				std::cout << "Check Count No." << check_cnt << std::endl;
			}
		}
#elif (SYNC_COOK ==2)
		if (burger->Monitor_flag == 1)
		{
			check_cnt++;
			Sleep(MONITOR_TIME);
			std::lock_guard<std::mutex> lock(gMutex);

			burger->Monitor_flag = 0;
			if (burger->Integrated > burger_num)
			{
				burger_num = burger->Integrated;
				std::cout << "\nCook Center[" << Cook_center_id << "]:" << std::endl;
				std::cout << "Total Burger number:" << burger_num << std::endl;
				std::cout << "Check Count No." << check_cnt << std::endl;
			}
		}
#endif
	}
	{
		std::lock_guard<std::mutex> lock(gMutex);
		std::cout << "\nCook Center[" << Cook_center_id << "]:" << std::endl;
		std::cout << "Finish cooking!!!" << std::endl;
		std::cout << "Check time:" << check_cnt << " times" <<std::endl;
	}
}