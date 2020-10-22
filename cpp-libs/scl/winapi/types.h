#pragma once

namespace winapi
{
#if MAXDWORD == UINT32_MAX

#define PRIuDWORD PRIu32
#define PRIdDWORD PRId32
#define PRIxDWORD PRIx32

#elif MAXDWORD == UINT64_MAX

#define PRIuDWORD PRIu64
#define PRIdDWORD PRId64
#define PRIxDWORD PRIx64

#else
#error DWORD size is unknown.
#endif

}
