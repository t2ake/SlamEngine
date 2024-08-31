#include "Memory.h"

#include "Utils//ProfilerCPU.h"

#include <cstdlib>

void *operator new (size_t count)
{
	auto ptr = malloc(count);
	TracyAlloc(ptr, count);
	return ptr;
}

void operator delete (void *ptr) noexcept
{
	TracyFree(ptr);
	free(ptr);
}
