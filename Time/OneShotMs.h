#pragma once
#include "daisy.h"

using namespace daisy;

/** One-shot callback after a delay; poll Update() from the main loop (non-blocking). */
class OneShotMs
{
public:
    OneShotMs() = default;

    void Start(uint32_t delay_ms, void (*callback)())
    {
        if (!callback)
            return;
        active_    = true;
        delay_ms_  = delay_ms;
        callback_  = callback;
        start_ms_  = System::GetNow();
    }

    void Cancel()
    {
        active_   = false;
        callback_ = nullptr;
    }

    void Update()
    {
        if (!active_ || !callback_)
            return;
        const uint32_t now = System::GetNow();
        if (now - start_ms_ >= delay_ms_)
        {
            void (*cb)() = callback_;
            active_        = false;
            callback_      = nullptr;
            cb();
        }
    }

    bool IsActive() const { return active_; }

private:
    bool     active_   = false;
    uint32_t delay_ms_ = 0;
    uint32_t start_ms_ = 0;
    void (*callback_)() = nullptr;
};
