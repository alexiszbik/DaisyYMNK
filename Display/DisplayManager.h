#pragma once

#include <string>

#include "IDisplayManager.h"
#include "MyOledDisplay.h"
#include "MyTransport.h"
#include "UpdatedDriver.h"

using namespace daisy;

static constexpr uint8_t scr_w = 128;

using OLEDDriver
    = UpdatedDriver<scr_w, 64, MyTransport>;

using Display_t = MyOledDisplay<OLEDDriver>;

using namespace std;

class DisplayManager : public IDisplayManager
{
    enum EPipe {
        idle,
        ready,
        update
    };

public:
    static DisplayManager* GetInstance();

    void Init(DaisySeed* hw);

    void Write(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr, bool now=false) override;
    void WriteLine(uint8_t lineIdx, const char* str) override;
    void WriteNow(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr) override;

    void Update();

private:
    DisplayManager();
    static DisplayManager* singleton;

    Display_t display;
    static constexpr int fontHeight = 16;
    static constexpr uint8_t lineCount = 4;

    bool needsUpdate = false;
    bool updatedLine[lineCount];

    char lines[lineCount][32] = {{0}};

    void setLine(int idx, const char* str);
    void Prepare();

    int currentPixBlock = 0;
    int currentLine = 0;
    EPipe pipe = idle;
};
