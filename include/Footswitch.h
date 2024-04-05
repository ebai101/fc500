#ifndef FOOTSWITCH_H
#define FOOTSWITCH_H
#include <Arduino.h>
#include <Bounce.h>

struct CCMessage
{
    int cc;
    int val;
};

class Footswitch
{
public:
    Footswitch(char letter, int footswitchPin, int ledPin);
    void setPreset(int p);
    void setScene(int s);
    void setToggleCC(int cc);
    void update();
    bool hasCC();
    bool hasPgm();
    CCMessage getCC();
    int getPgm();

    char letter;
    Bounce footswitch;
    int ledPin;

    const int presetLEDs[4] = {7, 8, 9, 10};
    const int sceneLEDs[4] = {2, 3, 4, 5};

private:
    int preset;
    int scene;
    int toggleCC;
    bool toggleState;
    CCMessage ccMsg;
    int pgmMsg;

    void selectPreset();
    void selectScene();
    void toggle();
};

#endif // !FOOTSWITCH_H