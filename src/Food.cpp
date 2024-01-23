#include <string.h>
#include "Food.h"

//=================================================================================================================
//brief         Initial Hamburger.
//param[in]     _Hamburger* burger is that will be initialized.
//return        1: finish initialize. 0: fail to initialize
//=================================================================================================================
bool Initial_Hamburger(_Hamburger* burger)
{
	if (burger == NULL)
		return 0;
	else
	    memset((void*)burger, 0, sizeof(_Hamburger));

	burger->Initial_flag = 1;
	return (burger->Initial_flag);
}