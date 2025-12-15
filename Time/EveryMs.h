#include "daisy.h"
#include <functional>
using namespace daisy;

class EveryMs
{
public:
    EveryMs(uint32_t period_ms, std::function<void()> cb)
        : period(period_ms), callback(cb), last(System::GetNow())
    {}

    void Update()
    {
        uint32_t now = System::GetNow();
        if (now - last >= period)
        {
            last = now;
            if(callback)
                callback();
        }
    }

private:
    uint32_t period;
    std::function<void()> callback;
    uint32_t last;

};