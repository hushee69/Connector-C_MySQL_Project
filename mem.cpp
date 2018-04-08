#include <Windows.h>

#include "mem.h"

#if defined _WIN64
#define MEM_ALLOC_BYTES						8
#else
#define MEM_ALLOC_BYTES						4
#endif

static HANDLE mainHeap;

void Mem::Init()
{
	mainHeap=GetProcessHeap();

	return;
}

void Mem::Uninit()
{
	

	return;
}

void *Mem::_alloc(SIZE_T size)
{
	register void *p;

	if( size==0 )
	{
		p=NULL;
	}
	else
	{
		size+=MEM_ALLOC_BYTES;
		p=HeapAlloc(mainHeap, HEAP_ZERO_MEMORY, size);
	}

	return p;
}

void *Mem::_realloc(void *old, SIZE_T size)
{
	register void *p;

	if( size==0 )
	{
		return NULL;
	}
	size+=MEM_ALLOC_BYTES;

	if( old==NULL )
	{
		old=HeapAlloc(mainHeap, HEAP_ZERO_MEMORY, size);
	}
	else
	{
		old=HeapReAlloc(mainHeap, HEAP_ZERO_MEMORY, old, size);
	}

	return old;
}

void Mem::_free(void *mem)
{
	if( mem )
	{
		HeapFree(mainHeap, 0, mem);
		mem = NULL;
	}

	return;
}

void Mem::_set(void *mem, char c, size_t size)
{
	size_t i=size;

	while( i-- )
	{
		((char*)mem)[i]=c;
	}

	return;
}

void Mem::_zero(void *mem, size_t size)
{
	_set(mem, 0, size);
}

void Mem::_copy(void *dest, const void *src, size_t size)
{
	int i=0;

	for( i=0; i<size; ++i )
	{
		((LPBYTE)dest)[i]=((LPBYTE)src)[i];
	}

	return;
}

bool Mem::_reallocEx(void *prev, SIZE_T size)
{
	if( size==0 )
	{
		free(*(LPBYTE*)prev);
	}
	else
	{
		register void *p=realloc(*(LPBYTE*)prev, size);

		if( p==NULL ) return false;
		*(LPBYTE*)prev=(LPBYTE)p;
	}

	return true;
}

void Mem::_zeroAndFree(void *mem, SIZE_T size)
{
	_zero(mem, size);
	free(mem);

	return;
}
