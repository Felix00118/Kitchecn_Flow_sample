#ifndef POOL_CLASS_H
#define POOL_CLASS_H
#include <iostream>

//=================================================================================================================
// Type definition
//=================================================================================================================


//=================================================================================================================
// Global parameter
//=================================================================================================================


//=================================================================================================================
// Function
//=================================================================================================================


//=================================================================================================================
// Class
//=================================================================================================================
class Pool_Class
{
    public:
        typedef struct Pool
	    {
	    	char* next;
	    	char* end;
	    }_Pool;
	    
	    typedef struct Pool_node
	    {
	    	char** obj_pointer;
	    	size_t data_size;
	    	void* data;
	    	struct Pool_node* next;
	    }_Pool_node;
	    
	    Pool_Class(size_t memory_pool_size)
	    {
	    	mem_pool = Pool_Class::pool_create(memory_pool_size);
	    	pool_node_head = (_Pool_node*)&mem_pool[1];
	    	pool_physical_start = (char*)&mem_pool[1];
	    	pool_physical_obj_start = (char*)&mem_pool[1] + sizeof(_Pool_node);
	    	//std::cout << "construct!" << std::endl;
	    }
	    
	    ~Pool_Class()
	    {
	    	pool_free(mem_pool);
	    	//std::cout << "destruct!" << std::endl;
	    }
	    
	    void* pool_alloc(size_t size);
	    bool pool_recycle(char** p);
	    bool pool_relocate(void);

    private:
		_Pool* mem_pool;
    	_Pool_node* pool_node_head;
		char* pool_physical_start;
		char* pool_physical_obj_start;

		_Pool* pool_create(size_t Memory_size);
		void pool_free(_Pool* p);
		size_t pool_available(_Pool* p);
		void pool_move_obj(_Pool_node* destination, _Pool_node** source);

};


#endif //#ifndef POOL_CLASS_H
