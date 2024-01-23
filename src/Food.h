#ifndef FOOD_H
#define FOOD_H

#include <cstdint>

//=================================================================================================================
// Type definition
//=================================================================================================================
typedef struct Hamburger
{
	union
	{
		struct
		{
			uint32_t Bread : 8;
			uint32_t Beef : 8;
			volatile uint32_t Integrated : 8;
			uint32_t Initial_flag : 1;
			volatile uint32_t Integrated_finish_flag : 1;
			volatile uint32_t Monitor_flag : 1;
		};
		uint32_t Full;
	};
}_Hamburger;

//=================================================================================================================
// Function
//=================================================================================================================
bool Initial_Hamburger(_Hamburger* burger);

#endif