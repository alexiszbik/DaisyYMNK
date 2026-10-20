#pragma once
#ifndef SdramPool_h
#define SdramPool_h

#include <cstddef>
#include <cstring>
#include <cstdint>

#define SDRAM_POOL_SIZE (512 * 1024)

extern char sdram_pool[];

inline void* sdram_alloc(size_t bytes) {
    static size_t offset = 0;
    bytes = (bytes + 7) & ~7;
    if (offset + bytes > SDRAM_POOL_SIZE) return nullptr;
    void* ptr = sdram_pool + offset;
    offset += bytes;
    memset(ptr, 0, bytes);
    return ptr;
}

#endif
