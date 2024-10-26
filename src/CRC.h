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

static inline u32 CRC_CalculatePalette(u32 crc, const void* buffer, u32 count)
{
	u8* p = (u8*)buffer;
	while (count--) {
		crc = (u32)XXH3_64bits_withSeed(p, 2, crc);
		p += 8;
	}
	return crc;
}

