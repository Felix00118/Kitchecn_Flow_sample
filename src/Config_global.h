#ifndef CONFIG_GLOBAL_H
#define CONFIG_GLOBAL_H

#include "windows.h"
#include <mutex>
#include "Food.h"
#include "Pool_class.h"

//=================================================================================================================
// Constant
//=================================================================================================================
//Asynchronous(SYNC_COOK =0), synchronous(SYNC_COOK =1) or fully synchronous(SYNC_COOK =2) cooking
#define SYNC_COOK 2

//Burger production line
#define BURGER_COOK_CENTER_NUM  3

//Total Food Quantity of Burger production line
#define BREAD_NUM         15
#define BEEF_NUM           6
#define MAX_NUM          255

//Make one Burger will consume the bread and beef
#define BREAD_CONSUME      2
#define BEEF_CONSUME       1

//Cooking time
#define BREAD_COOK_TIME   50
#define BEEF_COOK_TIME   100
#define INTEGRATED_TIME  500

//Monitor time interval
#define MONITOR_TIME     200

//Memory pool size
#define MEMORY_POOL_SIZE  sizeof(Pool_Class::_Pool) + ((sizeof(_Hamburger) + sizeof(Pool_Class::_Pool_node))* 3) + sizeof(char **)  // sizeof(char **) is for end node-> obj_pointer = 0;

//=================================================================================================================
// Global parameter
//=================================================================================================================
extern std::mutex gMutex;

//=================================================================================================================
// Function
//=================================================================================================================
#define RUN(x)  Sleep(x)

#endif