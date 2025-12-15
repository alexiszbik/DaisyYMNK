#include "DisplayManager.h"

DisplayManager* DisplayManager::singleton = nullptr;

DisplayManager* DisplayManager::GetInstance()
{
    if(!singleton)
        singleton = new DisplayManager();
    return singleton;
}

DisplayManager::DisplayManager() {
    uint8_t l = lineCount;
    while(l--) {
        updatedLine[l] = false;
    }
}

void DisplayManager::Init(DaisySeed* hw)
{
    Display_t::Config disp_cfg;
    disp_cfg.driver_config.transport_config.Defaults();
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = hw->GetPin(11);
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = hw->GetPin(12);

    display.Init(disp_cfg);
    display.Fill(false);
    display.Update();
}

void DisplayManager::Write(const char* l0, const char* l1, const char* l2, const char* l3, bool now)
{
    setLine(0, l0);
    setLine(1, l1);
    setLine(2, l2);
    setLine(3, l3);

    if(now)
    {
        Prepare();
        display.Update();
    }
    else
    {
        needsUpdate = true;
    }
}

void DisplayManager::WriteNow(const char* l0, const char* l1, const char* l2, const char* l3)
{
    Write(l0, l1, l2, l3, true);
}

void DisplayManager::WriteLine(uint8_t lineIdx, const char* str) {
    setLine(lineIdx, str);
    needsUpdate = true;
}

void DisplayManager::Update() {
    switch (pipe) {
        case idle :
        {
            currentPixBlock = 0;
            if (needsUpdate) {
                for (uint8_t l = 0; l < lineCount; l++) {
                    if (updatedLine[l]) {
                        currentLine = l;
                        pipe = ready;
                        break;
                    }
                }
            }
            
        }
        break;
        case ready :
        {
            Prepare();
            pipe = update;
        }
        break;
        case update : {
            uint8_t actionCount = 8;
            while(actionCount--) {

                display.Update(currentLine*2*scr_w + currentPixBlock);

                currentPixBlock++;

                if (currentPixBlock >= 2*scr_w) {
                    currentPixBlock = 0;
                    updatedLine[currentLine] = false;

                    do {
                        currentLine++;
                        if (currentLine >= lineCount) {
                            pipe = idle;
                            return;
                        }
                    } while (!updatedLine[currentLine]);
                }
            }
        }
        break;
    }
    
}


void DisplayManager::setLine(int idx, const char* str)
{
    if(!str) { lines[idx][0] = 0; return; }
    strncpy(lines[idx], str, sizeof(lines[idx])-1);
    updatedLine[idx] = true;
    lines[idx][sizeof(lines[idx])-1] = 0;
}

void DisplayManager::Prepare()
{
    display.Fill(false);
    for(int i=0;i<lineCount;i++)
    {
        if(lines[i][0] != 0)
        {
            display.SetCursor(0, i*fontHeight);
            display.WriteString(lines[i], Font_7x10, true);
        }
    }
    needsUpdate = false;
}
