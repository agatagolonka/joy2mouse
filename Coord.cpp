#include "Coord.h"

Coord :: Coord()
{
  this->X = 0;
  this->Y = 0;
}

Coord :: Coord(unsigned int x, unsigned int y)
{
  this->X = x;
  this->Y = y;
}

void Coord :: setX(unsigned int x)
{
  this->X = x;
}

void Coord :: setY(unsigned int y)
{
  this->Y = y;
}

unsigned int Coord :: getX()
{
  return this->X;
}

unsigned int Coord :: getY()
{
  return this->Y;
}

Coord Coord :: operator = (Coord c)
{
  this->X = c.X;
  this->Y = c.Y;

  return *this;
}
