#ifndef MODEFOOTSWITCH_H
#define MODEFOOTSWITCH_H
#include <Arduino.h>
#include "Footswitch.h"

class ModeFootswitch
{
public:
    ModeFootswitch(char letter, int footswitchPin, int ledPin, int ledPin2);
    void update();
    bool getCurrentMode();

    char letter;
    Bounce footswitch;
    int ledPin;
    int ledPin2;

private:
    const int presetLEDs[4] = {7, 8, 9, 10};
    const int sceneLEDs[4] = {2, 3, 4, 5};

    bool currentMode;
    void presetMode();
    void stompMode();
};

#endif // !MODEFOOTSWITCH_H