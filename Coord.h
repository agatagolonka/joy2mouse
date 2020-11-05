#ifndef COORD_H
#define COORD_H

#include <iostream>

class Coord {
  private:
    unsigned int X;
    unsigned int Y;
  public:
    Coord();
    Coord(unsigned int x, unsigned int y);
    void setX(unsigned int x);
    void setY(unsigned int y);
    unsigned int getX();
    unsigned int getY();
    Coord operator = (Coord c);
    friend std :: ostream& operator << (std :: ostream& out, Coord c)
    {
      out << c.X << ' ' << c.Y;

      return out;
    }
};

#endif