#pragma once
#include "daisy.h"

using namespace daisy;

class BlockingAction
{
public:
    BlockingAction() = default;

    void Run(uint32_t duration_ms, void (*action)())
    {
        const uint32_t start = System::GetNow();

        while(System::GetNow() - start < duration_ms)
        {
            action();
        }
    }
};
