
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include <string>
#include <vector>

#include "MyOledDisplay.h"
#include "MyTransport.h"
#include "UpdatedDriver.h"

using OLEDDriver
    = UpdatedDriver<128, 64, MyTransport>;

using Display_t = MyOledDisplay<OLEDDriver>;

using namespace std;

class DisplayManager
{
protected:
    enum EMessageType {
        strings,
        parameterValue
    };

    enum EPipe {
        idle,
        ready,
        update
    };

private:
    DisplayManager() {};

    static DisplayManager* singleton;
public:
    static DisplayManager *GetInstance();


    void Init(DaisySeed *hw);
    void Write(vector<string> messages, bool now = false);
    void Prepare();

    void Update();
    
private:
    Display_t display;
    const int fontHeight = 18;

    bool needsUpdate = false;
    EMessageType lastMessageType = strings;

    vector<string> messages;

    int currentLine = 0;
    EPipe pipe = idle;

};


#endif //DISPLAY_MANAGER_H