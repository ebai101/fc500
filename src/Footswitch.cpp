#include "Footswitch.h"

Footswitch::Footswitch(char letter, int footswitchPin, int ledPin) : letter(letter),
                                                                     footswitch(Bounce(footswitchPin, 10)),
                                                                     ledPin(ledPin),
                                                                     preset(-1),
                                                                     scene(-1),
                                                                     toggleCC(-1),
                                                                     toggleState(0),
                                                                     ccMsg({-1, -1}),
                                                                     pgmMsg(-1)
{
    pinMode(footswitchPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
}

void Footswitch::setPreset(int p)
{
    preset = p;
}

void Footswitch::setScene(int s)
{
    scene = s;
}

void Footswitch::setToggleCC(int cc)
{
    toggleCC = cc;
}

void Footswitch::update()
{
    // read switch position
    if (footswitch.update())
    {
        if (footswitch.fallingEdge())
        {
            Serial.printf("%c pressed\n", letter);
            if (preset > -1)
            {
                selectPreset();
            }
            else if (scene > -1)
            {
                selectScene();
            }
            else if (toggleCC > -1)
            {
                toggle();
            }
        }
        else if (footswitch.risingEdge())
        {
            Serial.printf("%c released\n", letter);
        }
    }
}
void Footswitch::selectPreset()
{
    if (preset == -1)
    {
        Serial.printf("no preset set for switch %c\n", letter);
        return;
    }

    // send 0-3 on CC#0 and pgm change 0-127 to select preset 0-511
    // fm3 manual page 142
    int cc = floor(preset / 128);
    int pgm = preset - (128 * cc);
    ccMsg = {0, cc};
    pgmMsg = pgm;

    // write LED state
    for (int i = 0; i < 4; i++)
        digitalWrite(presetLEDs[i], HIGH);
    digitalWrite(ledPin, LOW);
}

void Footswitch::selectScene()
{
    if (scene == -1)
    {
        Serial.printf("no preset set for switch %c\n", letter);
        return;
    }

    // FM3 scene change mapped to CC#1
    ccMsg = {1, scene - 1};

    // write LED state
    for (int i = 0; i < 4; i++)
        digitalWrite(sceneLEDs[i], HIGH);
    digitalWrite(ledPin, LOW);
}

void Footswitch::toggle()
{
    if (toggleCC == -1)
    {
        Serial.printf("no cc set for toggle switch %c\n", letter);
        return;
    }

    // send CC#toggleCC 127 or 0 based on state
    toggleState = !toggleState;
    int cc = toggleState ? 127 : 0;
    ccMsg = {toggleCC, cc};

    // write LED state
    digitalWrite(ledPin, !toggleState);
}

bool Footswitch::hasCC()
{
    if (ccMsg.cc == -1 && ccMsg.val == -1)
    {
        return false;
    }
    return true;
}

bool Footswitch::hasPgm()
{
    return pgmMsg != -1;
}

CCMessage Footswitch::getCC()
{
    CCMessage msg = ccMsg;
    ccMsg = {-1, -1};
    return msg;
}

int Footswitch::getPgm()
{
    int msg = pgmMsg;
    pgmMsg = -1;
    return msg;
}