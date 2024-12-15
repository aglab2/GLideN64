#pragma once
#include "Types.h"

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

void CRC_Init();

u32 CRC_Calculate_Strict( u32 crc, const void *buffer, u32 count );

static inline u32 CRC_Calculate(u32 crc, const void* buffer, u32 count)
{
	return (u32)XXH3_64bits_withSeed(buffer, count, crc);
}

static inline u32 CRC_CalculatePalette(u32 crc, const void* buffer)
{
	u8 combined[32];

	int count = 16;
	u8* p = (u8*)buffer;
	u8* o = combined;
	while (count--) {
		__builtin_memcpy(o, p, 2);
		p += 8;
		o += 2;
	}

	return (u32)XXH3_64bits_withSeed(combined, 32, crc);
}

