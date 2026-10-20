#include "SdramPool.h"

#if defined(__APPLE__)
char sdram_pool[SDRAM_POOL_SIZE];
#else
#include "daisy_seed.h"
DSY_SDRAM_BSS char sdram_pool[SDRAM_POOL_SIZE];
#endif
