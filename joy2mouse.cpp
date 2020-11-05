#include "JoyDevice.h"

Display *display;

int main(int argc, char **argv)
{
  if(argc != 2)
  {  
    std :: cout << "Niepoprawna ilosc argumentow!\n";
    std :: cout << "Poprawne uzycie: joy2mouse /dev/input/jsX\n";
    exit(EXIT_FAILURE);
  }
  
  display = XOpenDisplay(0);

  if(display == NULL)
  {
    std :: cout << "Nie mozna polaczyc sie z serwerem X!\n";
    exit(EXIT_FAILURE);
  }

  JoyDevice joydevice(argv[1]);

  joydevice.printDeviceInfo();

  joydevice.toMouse();

  XCloseDisplay(display);

  return 0;
}