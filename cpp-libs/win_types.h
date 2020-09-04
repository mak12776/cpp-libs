#pragma once

#include <Windows.h>
#include <cstdint>
#include <inttypes.h>

#if MAXDWORD == UINT32_MAX

#define PRIuDWORD PRIu32
#define PRIdDWORD PRId32
#define PRIxDWORD PRIx32

#else
#error DWORD size is unknown.
#endif