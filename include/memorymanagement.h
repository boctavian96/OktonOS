 
#ifndef __MYOS__MEMORYMANAGEMENT_H
#define __MYOS__MEMORYMANAGEMENT_H


#include<common/types.h>

namespace myos
{
    
    struct MemoryChunk
    {
        bool allocated;
        MemoryChunk* next;
        MemoryChunk* prev;
        common::size_t size;
    };
    
    
    class MemoryManager
    {
    protected:
        MemoryChunk* firstchunk;
    public:
        static MemoryManager* activeMemoryManager;
        
        MemoryManager(common::size_t first, common::size_t size);
        ~MemoryManager();
        
        void* malloc(common::size_t size);
        void free(void* ptr);
    };
    
}


void* operator new(unsigned size);
void* operator new[](unsigned size);
// placement new
void* operator new(myos::common::size_t size, void *ptr);
void* operator new[](myos::common::size_t size, void *ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif