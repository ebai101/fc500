/*
mode 1 - "preset mode"
sends CC/PGM combination for preset select, and CC for scene select
1-4: CC#1 (scene select 1-4)
5-8: CC#0/PGM preset select 1-4

mode 2 - "stomp mode"
sends CCs for block bypass/other switch functions
1-8: CC#3-10

both modes
b: CC#11 (tap tempo)
c: CC#2 (fx return bypass)
*/

#include <Arduino.h>
#include <MIDI.h>
#include "Footswitch.h"
#include "ModeFootswitch.h"

#define SWITCH_COUNT 10

MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI);
Footswitch *switchesPresetMode[SWITCH_COUNT];
Footswitch *switchesStompMode[SWITCH_COUNT];
ModeFootswitch *modeFootswitch;

void setup()
{
  // PRESET MODE
  switchesPresetMode[0] = new Footswitch('1', 24, 2);  // scene 1
  switchesPresetMode[1] = new Footswitch('2', 25, 3);  // scene 2
  switchesPresetMode[2] = new Footswitch('3', 26, 4);  // scene 3
  switchesPresetMode[3] = new Footswitch('4', 27, 5);  // scene 4
  switchesPresetMode[4] = new Footswitch('B', 29, -1); // tap tempo
  switchesPresetMode[5] = new Footswitch('C', 28, 6);  // fx return
  switchesPresetMode[6] = new Footswitch('5', 23, 7);  // n/a
  switchesPresetMode[7] = new Footswitch('6', 22, 8);  // preset 1
  switchesPresetMode[8] = new Footswitch('7', 21, 9);  // preset 2
  switchesPresetMode[9] = new Footswitch('8', 20, 10); // preset 3
  switchesPresetMode[0]->setScene(1);
  switchesPresetMode[1]->setScene(2);
  switchesPresetMode[2]->setScene(3);
  switchesPresetMode[3]->setScene(4);
  switchesPresetMode[4]->setTapCC(11);
  switchesPresetMode[5]->setToggleCC(2);
  switchesPresetMode[7]->setPreset(506);
  switchesPresetMode[8]->setPreset(509);
  switchesPresetMode[9]->setPreset(507);

  // STOMP MODE
  switchesStompMode[0] = new Footswitch('1', 24, 2);  // drive 1
  switchesStompMode[1] = new Footswitch('2', 25, 3);  // drive 2
  switchesStompMode[2] = new Footswitch('3', 26, 4);  // chorus
  switchesStompMode[3] = new Footswitch('4', 27, 5);  // flanger
  switchesStompMode[4] = new Footswitch('B', 29, -1); // tap tempo
  switchesStompMode[5] = new Footswitch('C', 28, 6);  // fx return
  switchesStompMode[6] = new Footswitch('5', 23, 7);  // n/a
  switchesStompMode[7] = new Footswitch('6', 22, 8);  // phaser
  switchesStompMode[8] = new Footswitch('7', 21, 9);  // delay
  switchesStompMode[9] = new Footswitch('8', 20, 10); // auto swell
  switchesStompMode[0]->setToggleCC(3);
  switchesStompMode[1]->setToggleCC(4);
  switchesStompMode[2]->setToggleCC(5);
  switchesStompMode[3]->setToggleCC(6);
  switchesStompMode[4]->setTapCC(11);
  switchesStompMode[5]->setToggleCC(2);
  switchesStompMode[6]->setToggleCC(7);
  switchesStompMode[7]->setToggleCC(8);
  switchesStompMode[8]->setToggleCC(9);
  switchesStompMode[9]->setToggleCC(10);

  modeFootswitch = new ModeFootswitch('G', 19, 11, 12);

  MIDI.begin();
}

void loop()
{
  modeFootswitch->update();

  // if in preset mode, update preset mode switches
  if (!modeFootswitch->getCurrentMode())
  {
    for (int i = 0; i < SWITCH_COUNT; i++)
    {
      switchesPresetMode[i]->update();
      if (switchesPresetMode[i]->hasCC())
      {
        CCMessage msg = switchesPresetMode[i]->getCC();
        Serial.printf("sending cc %d %d\n", msg.cc, msg.val);
        MIDI.sendControlChange(msg.cc, msg.val, 1);
      }
      if (switchesPresetMode[i]->hasPgm())
      {
        int msg = switchesPresetMode[i]->getPgm();
        Serial.printf("sending pgm %d %d\n", msg);
        MIDI.sendProgramChange(msg, 1);
      }
    }
  }
  // otherwise update stomp mode switches
  else
  {
    for (int i = 0; i < SWITCH_COUNT; i++)
    {
      switchesStompMode[i]->update();
      if (switchesStompMode[i]->hasCC())
      {
        CCMessage msg = switchesStompMode[i]->getCC();
        Serial.printf("sending cc %d %d\n", msg.cc, msg.val);
        MIDI.sendControlChange(msg.cc, msg.val, 1);
      }
    }
  }
}