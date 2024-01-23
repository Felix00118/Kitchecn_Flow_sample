#include <stdlib.h>
#include "windows.h"
#include < string.h >
#include "Pool_class.h"

//=================================================================================================================
// Global parameter
//=================================================================================================================


//=================================================================================================================
//brief         Create the memory pool
//param[in]     size_t Memory_size is size of memory pool.
//return        _Pool* is pointer of memory pool.
//=================================================================================================================
Pool_Class::_Pool* Pool_Class::pool_create(size_t Memory_size)
{
	//_Pool* p = (_Pool*)malloc(Memory_size);
	_Pool* p = (_Pool*)calloc(1,Memory_size);
	if (p != NULL)
	{
		//memset(p, 0, Memory_size);
		p->next = (char*)& p[1];
		p->end = (char*)& p[0] + Memory_size - sizeof(char**);
	}
	return p;
}

//=================================================================================================================
//brief         Release the memory pool
//param[in]     _Pool* p is pointer of memory pool.
//return        Null
//=================================================================================================================
void Pool_Class::pool_free(_Pool* p)
{
	free(p);
	p = NULL;
}

//=================================================================================================================
//brief         Check the usable size of memory pool.
//param[in]     _Pool* p is pointer of memory pool.
//return        Available size
//=================================================================================================================
size_t Pool_Class::pool_available(Pool_Class::_Pool* p)
{
	if (((size_t)(p->end - p->next)) < sizeof(Pool_Class::_Pool_node))
		return 0;
	else
		return ((p->end - p->next) - sizeof(Pool_Class::_Pool_node));
}

//=================================================================================================================
//brief         Allocate the usable size of memory pool.
//param[in]     _Pool* p is pointer of memory pool.
//param[in]     size_t size is allocate size of memory pool.
//return        void* mem is pointer of the new allocate area
//=================================================================================================================
void* Pool_Class::pool_alloc(size_t size)
{
	Pool_Class::_Pool* p = Pool_Class::mem_pool;

	if (size <= 0) //check vliad size
	{
		return NULL;
	}

	if (pool_available(p) < size)
	{
		if (pool_relocate() == 0)
			return NULL;
		else if (pool_available(p) < size)
			return NULL;
	}

	_Pool_node* temp_node = (_Pool_node*)p->next;
	temp_node->data_size = size;
	temp_node->data = p->next + sizeof(_Pool_node);
	temp_node->obj_pointer = (char**)&temp_node->data; //add 20230625
	temp_node->next = (_Pool_node*)((char*)temp_node->data + (char)temp_node->data_size);
	void* mem = (void*)temp_node->data;
	p->next = (char*)temp_node->next;
	return mem;
}

//=================================================================================================================
//brief         Recycle to be the usable size of memory pool.
//param[in]     void* p is pointer of the object in memory pool.
//return        1: Recycle success 0: Not find the correspond node
//=================================================================================================================
bool Pool_Class::pool_recycle(char** p)
{
	Pool_Class::_Pool_node** temp_node = &pool_node_head;
	while ((*temp_node)->obj_pointer != NULL)
	{
		if ((*temp_node)->obj_pointer == (char**)p)
		{
			Pool_Class::_Pool_node* clean_address = (*temp_node);
			size_t clean_size = sizeof(Pool_Class::_Pool_node) + (*temp_node)->data_size;
			(*temp_node) = (*temp_node)->next;
			memset(clean_address, 0, clean_size);
			*p = NULL;
			return 1;
		}
		temp_node = &((*temp_node)->next);
	}
	return 0;
}

//=================================================================================================================
//brief         Move the object to destination in the memory pool.
//param[in]     _Pool_node* destination is pointer of the destination in memory pool.
//param[in]     _Pool_node** source is a pointer of the object pointer which points to the moved object in memory pool.
//return        Null
//=================================================================================================================
void Pool_Class::pool_move_obj(_Pool_node* destination, _Pool_node** source)
{
	//copy the data and remove the unused data
	size_t copy_size = sizeof(_Pool_node) + (*source)->data_size;
	size_t remove_size = (char*)(*source) + copy_size - ((char*)(destination)+copy_size);
	memmove(destination, (*source), copy_size);
	memset((char*)destination + copy_size, 0, remove_size);

	//update the information to node
	(*source) = destination;
	(*source)->data = ((char*)(*source) + sizeof(_Pool_node));
	*(*source)->obj_pointer = (char*)(*source)->data;
}

//=================================================================================================================
//brief         Relocate to get bigger usable chunk of memory pool.
//param[in]     _Pool* p is pointer of memory pool.
//return        1: get bigger chunk 0:no improve
//=================================================================================================================
bool Pool_Class::pool_relocate(void)
{
	Pool_Class::_Pool* p = Pool_Class::mem_pool;
	char* pre_next = p->next;
	Pool_Class::_Pool_node** temp_node = &pool_node_head;

	if ((*temp_node)->obj_pointer == NULL)  //No object in the mem_pool
	{
		p->next = pool_physical_start;
	}
	else //((*temp_node)->obj_pointer != NULL)
	{
		if (*(*temp_node)->obj_pointer != pool_physical_obj_start)  //Check if the head node is not on start address
		{
			Pool_Class::pool_move_obj((Pool_Class::_Pool_node*)pool_physical_start, temp_node);
		}

		while ((*temp_node)->obj_pointer != NULL)
		{
			if ((*temp_node)->next != (_Pool_node*)((char*)(*temp_node)->data + (char)(*temp_node)->data_size))  //need to arrange
			{
				_Pool_node* destination = (_Pool_node*)((char*)(*temp_node)->data + (char)(*temp_node)->data_size);
				_Pool_node** moved_address = &(*temp_node)->next;
				if ((*moved_address)->obj_pointer != NULL)  //need to copy the data from moved address to destination
				{
					pool_move_obj(destination, moved_address);
				}
				else //no need to copy the data(final node)
				{
					(*temp_node)->next = destination;
				}
			}
			temp_node = &((*temp_node)->next);
		}
		p->next = (char*)* temp_node;
	}

	if (p->next < pre_next) //get bigger size (size = p->end - p->next)
		return 1;
	else
		return 0;
}
