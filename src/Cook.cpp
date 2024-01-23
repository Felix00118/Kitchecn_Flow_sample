#include <iostream> 
#include "windows.h"
#include "Cook.h"

//=================================================================================================================
//brief         The sequence of cooking hamburger.
//param[in]     _Hamburger* burger is that will execute cooking(bread and beef) and integrated.
//return        Null
//=================================================================================================================
void Cook_Hamburger(_Hamburger* burger)
{
	if (burger->Initial_flag)
	{
		//Grill Bread
		RUN(BREAD_COOK_TIME);
		if(BREAD_NUM <= MAX_NUM)
		    burger->Bread = BREAD_NUM;
		else
			burger->Bread = MAX_NUM;

		//Roast Beef
		RUN(BEEF_COOK_TIME);
		if (BEEF_NUM <= MAX_NUM)
		    burger->Beef = BEEF_NUM;
		else
			burger->Beef = MAX_NUM;

		//Integrate Burger
#if SYNC_COOK
		while (burger->Bread >= BREAD_CONSUME && burger->Beef >= BEEF_CONSUME)
		{
			if (burger->Monitor_flag == 0)
			{
				RUN(INTEGRATED_TIME);
				std::lock_guard<std::mutex> lock(gMutex);
				burger->Bread -= BREAD_CONSUME;
				burger->Beef -= BEEF_CONSUME;
				burger->Integrated++;
				burger->Monitor_flag = 1;
			}
		}
#else
		while (burger->Bread >= BREAD_CONSUME && burger->Beef >= BEEF_CONSUME)
		{
			if (burger->Monitor_flag == 0)
			{
				burger->Bread -= BREAD_CONSUME;
				burger->Beef -= BEEF_CONSUME;
				burger->Integrated++;
				RUN(INTEGRATED_TIME);
			}
		}
#endif
		burger->Integrated_finish_flag = TRUE;
	}
}