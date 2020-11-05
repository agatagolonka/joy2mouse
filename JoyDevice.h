#ifndef JOYDEVICE_H
#define JOYDEVICE_H


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <cstdlib>
#include <cstring>
#include "Mouse.h"

class JoyDevice {
  private:
    unsigned char axisCount;
    unsigned char buttonCount;
    int file_descriptor;
    int version;
    char devName[83];
    int *axisPosition;
    int *buttonState;
    js_event jsevent;
    int normalize(int pos);
  
  public:
    JoyDevice(char *device);
    void printDeviceInfo();
    void printStates();
    void toMouse();
};

#endif
  
