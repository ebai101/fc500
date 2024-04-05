#include "ModeFootswitch.h"

ModeFootswitch::ModeFootswitch(char letter, int footswitchPin, int ledPin, int ledPin2) : letter(letter),
                                                                                          footswitch(Bounce(footswitchPin, 10)),
                                                                                          ledPin(ledPin),
                                                                                          ledPin2(ledPin2),
                                                                                          currentMode(0)
{
    pinMode(footswitchPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
}
void ModeFootswitch::update()
{
    // read switch position
    if (footswitch.update())
    {
        if (footswitch.fallingEdge())
        {
            Serial.printf("%c pressed\n", letter);
            for (int i = 0; i < 4; i++)
            {
                digitalWrite(presetLEDs[i], HIGH);
                digitalWrite(sceneLEDs[i], HIGH);
            }
            if (currentMode)
            {
                presetMode();
                digitalWrite(ledPin, LOW);
                digitalWrite(ledPin2, HIGH);
                currentMode = 0;
            }
            else
            {
                stompMode();
                digitalWrite(ledPin, HIGH);
                digitalWrite(ledPin2, LOW);
                currentMode = 1;
            }
        }
        else if (footswitch.risingEdge())
        {
            Serial.printf("%c released\n", letter);
        }
    }
}

bool ModeFootswitch::getCurrentMode()
{
    return currentMode;
}

void ModeFootswitch::presetMode()
{
}

void ModeFootswitch::stompMode()
{
}