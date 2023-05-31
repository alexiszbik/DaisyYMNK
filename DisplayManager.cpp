
#include "DisplayManager.h"

DisplayManager* DisplayManager::singleton = nullptr;

DisplayManager *DisplayManager::GetInstance()
{
    if(singleton == nullptr){
        singleton = new DisplayManager();
    }
    return singleton;
}

void DisplayManager::Init(DaisySeed *hw) {
    /** Configure the Display */
    Display_t::Config disp_cfg;
    disp_cfg.driver_config.transport_config.Defaults();
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = hw->GetPin(11);
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = hw->GetPin(12);
    
    /** And Initialize */
    display.Init(disp_cfg);
}

void DisplayManager::Write(vector<string> messages, bool now) {
    this->messages = messages;
    if (now) {
        Prepare();
        display.Update();
    } else {
        needsUpdate = true;
    }
    lastMessageType = strings;
}

void DisplayManager::Prepare() {

    display.Fill(false);
    if (lastMessageType == parameterValue) {
        /*
        char buffer[8];
        parameter->getValueToStr(buffer, sizeof buffer);
        
        display.SetCursor(0, 0);
        display.WriteString(parameter->name, Font_11x18, true);
        display.SetCursor(0, fontHeight);
        display.WriteString(buffer, Font_7x10, true);
        */
    } else {
        short iterator = 0;
        for (auto str : messages) {
            if (iterator <= 2) {
                display.SetCursor(0, fontHeight * iterator);
                display.WriteString(str.c_str(), Font_11x18, true);
            }
        iterator++;
        }
    }
    needsUpdate = false;
}

void DisplayManager::Update() {
    switch (pipe) {
        case idle :
        {
            currentLine = 0;
            if (needsUpdate) {
                pipe = ready;
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
            display.Update(currentLine++);
            if (currentLine >= 8*128) {
                pipe = idle;
                
            }
        }
        break;
    }
}

