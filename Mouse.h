#ifndef MOUSE_H
#define MOUSE_H

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include "Coord.h"

#define XRES 1280
#define YRES 800
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 3


class Mouse {
  private:
    Coord position;
  public:
    Mouse();
    Coord* getPosition();
    void setPosition(Coord *pos);
    void click(char button);
};

#endif
