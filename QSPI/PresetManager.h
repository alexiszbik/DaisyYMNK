#pragma once

#include "daisy_seed.h"
#include "IPresetManager.h"

#define PRESET_FLASH_ADDR  0x90000000
#define PRESET_SECTOR_SIZE 4096
#define PRESET_MAGIC       0xDEADBEEF

//Erasures will happen by 4K, 32K or 64K increments.

using namespace daisy;

class PresetManager : public IPresetManager
{
public:
    void Init(DaisySeed* hw)
    {
        this->hw = hw;
    }

    bool Save(const float* preset, uint8_t size, uint8_t index) override
    {
        if(!hw || index >= MAX_PRESETS)
            return false;

        if(size > MAX_PRESET_SIZE - 1)
            size = MAX_PRESET_SIZE - 1; //Size minus magic

        const uint32_t addr = PRESET_FLASH_ADDR + index * PRESET_SECTOR_SIZE;

        if(hw->qspi.Erase(addr, addr + PRESET_SECTOR_SIZE) != QSPIHandle::Result::OK)
            return false;

        // Write magic number
        uint32_t magic = PRESET_MAGIC;
        if(hw->qspi.Write(addr, sizeof(magic), (uint8_t*)&magic) != QSPIHandle::Result::OK)
            return false;

        // Write floats
        return hw->qspi.Write(addr + sizeof(magic), size * sizeof(float), (uint8_t*)preset) == QSPIHandle::Result::OK;
    }

    const float* Load(uint8_t index) const override
    {
        if(!hw || index >= MAX_PRESETS)
            return nullptr;

        const uint32_t addr = PRESET_FLASH_ADDR + index * PRESET_SECTOR_SIZE;
        const uint32_t* magicPtr = reinterpret_cast<const uint32_t*>(addr);

        if(*magicPtr != PRESET_MAGIC)
            return nullptr; // prevent from reading bad values

        return reinterpret_cast<const float*>(addr + sizeof(uint32_t));
    }

private:
    DaisySeed* hw = nullptr;
};
