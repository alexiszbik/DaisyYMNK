#pragma once
#include "daisy_seed.h"

#define PRESET_FLASH_ADDR  0x90000000
#define PRESET_SECTOR_SIZE 4096
#define MAX_PRESET_SIZE    64
#pragma once
#include "daisy_seed.h"

using namespace daisy;

class PresetManager
{
public:
    void Init(DaisySeed* hw)
    {
        this->hw = hw;
    }

    bool Save(const float* preset, uint8_t size) //Return true if success !!
    {
        if(!hw)
            return false;

        if(size > MAX_PRESET_SIZE)
            size = MAX_PRESET_SIZE;

        const size_t byte_size = size * sizeof(float);


        if (hw->qspi.Erase(PRESET_FLASH_ADDR, PRESET_FLASH_ADDR + PRESET_SECTOR_SIZE) != QSPIHandle::Result::OK) {
            return false;
        }

        return hw->qspi.Write(PRESET_FLASH_ADDR, byte_size, (uint8_t*)preset) == QSPIHandle::Result::OK;
    }

    const float* Load() const
    {
        // Lecture directe memory-mapped
        return reinterpret_cast<const float*>(PRESET_FLASH_ADDR);
    }

private:
    DaisySeed* hw = nullptr;
};
