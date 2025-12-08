#pragma once
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include <string>

#include "MyOledDisplay.h"
#include "MyTransport.h"
#include "UpdatedDriver.h"

using namespace daisy;

using OLEDDriver
    = UpdatedDriver<128, 64, MyTransport>;

using Display_t = MyOledDisplay<OLEDDriver>;

using namespace std;

class DisplayManager
{
public:
    static DisplayManager* GetInstance();

    void Init(DaisySeed* hw);

    void Write(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr, bool now=false);
    void WriteNow(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr);

    void Update();

private:
    DisplayManager();
    static DisplayManager* singleton;

    Display_t display;
    static constexpr int fontHeight = 18;
    bool needsUpdate = false;

    char lines[4][32] = {{0}}; // buffers internes pour chaque ligne

    void setLine(int idx, const char* str);
    void Prepare();
};
