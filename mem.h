#pragma once

#include <WinDef.h>

namespace Mem
{
	void Init();
	void Uninit();
	void *_alloc(SIZE_T size);
	void *_realloc(void *old, SIZE_T size);
	void _free(void *mem);
	void _zero(void *mem, size_t size);
	void _set(void *mem, char c, size_t size);
	void _copy(void *dest, const void *src, size_t size);
	bool _reallocEx(void *prev, SIZE_T size);
	void _zeroAndFree(void *mem, SIZE_T size);
}
