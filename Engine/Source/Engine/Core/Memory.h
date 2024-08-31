#pragma once

void *operator new (size_t count);
void operator delete (void *ptr) noexcept;
