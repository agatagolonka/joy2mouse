#include "Mouse.h"


/********************************************************************/
/*								    */
/*  Konstruktor ustawiajacy inicjujacy obiekt Mouse nadajacy polu   */
/*  position wartosc aktualnych wspólrzednych kursora               */
/*								    */
/********************************************************************/

Mouse :: Mouse()
{
  this->position = *this->getPosition();
}


/********************************************************************/
/*                                                                  */
/*  Coord* getPosition()                                       	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      Odczyt aktualnej pozycji kursora                    	    */
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	void							    */
/*                                                                  */
/*  Funkcja zwraca:                                                 */
/*      Wskaznik do obiektu typu Coord				    */
/*	Zawiera wspolrzedne kursora				    */
/*								    */
/*  Uzywane funkcje:                                                */
/*      XQueryPointer - funkcja udostepniana przez serwer X         */
/*                      sluzy do odczytania wspolrzednych kursora   */
/*                                                                  */
/*  Uzywane zmienne:                                                */
/*      Display *display - zmienna globalna pozwalajaca na polacznie*/
/*      z serwerem x						    */
/*	XEvent event - zmienna lokalna, jest argumentem funkcji	    */
/*	XQueryPointer, sa w niej zapisywane wspolrzedne kursora	    */	
/*                                                                  */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

Coord* Mouse :: getPosition()
{
  extern Display *display;
  
  XEvent event;
  XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

  return (new Coord(event.xbutton.x, event.xbutton.y));
}

/********************************************************************/
/*                                                                  */
/*  void setPosition(Coord *pos)                              	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      Ustawienie kursora na pozycji o wspolrzednych         	    */
/*	przechowywanych w pos					    */
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	(I) Coord *pos - wskaznik do typu Coord, przechowuje 	    */
/*	wspolrzedne wedlug ktorych zostanie ustawiony kursor        */
/*								    */
/*  Funkcja zwraca:                                                 */
/*	void							    */
/*								    */
/*  Uzywane funkcje:                                                */
/*      XWarpPointer - funkcja udostepniana przez serwer X          */
/*      sluzy do ustawiania pozycji kursora wzgledem aktualnej      */
/*	pozycji;						    */
/*	getPosition - odjecie od pozycji docelowejaktualnej pozycji */
/*	pozwala ustawic kursor w dowolnym miejscu przy pomocy	    */
/*	funkcji XWarpPointer					    */
/*								    */
/*  Uzywane zmienne:                                                */
/*      Display *display - zmienna globalna pozwalajaca na polacznie*/
/*      z serwerem x						    */
/*                                                                  */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

void Mouse :: setPosition(Coord *pos)
{
  extern Display *display;
  
  XWarpPointer(display, None, None, 0, 0, 0, 0, pos->getX() - getPosition()->getX(), pos->getY() - getPosition()->getY());
  
  this->position = *pos;
}

/********************************************************************/
/*                                                                  */
/*  void click(char button)                                   	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      Symulowanie klikniecia       		             	    */
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	(I) char button - wcisniecie ktorego przycisku mysze ma byc */
/*	symulowane; 1 - lewy, 3 - prawy				    */
/*                                                                  */
/*  Funkcja zwraca:                                                 */
/*	void							    */
/*								    */
/*  Uzywane funkcje:                                                */
/*      XTestFakeButtonEvent - funkcja udostepniana przez serwer X  */
/*      sluzy do testowania dzialania roznych zdarzen, w naszym	    */
/*	przypadku - wcisniecia klawiszy mysze			    */
/*	XFlush - wymusza przeplyw danych w buforze wyjsciowym	    */
/*                                                                  */
/*  Uzywane zmienne:                                                */
/*      Display *display - zmienna globalna pozwalajaca na polacznie*/
/*      z serwerem x						    */
/*                                                                  */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

void Mouse :: click(char button)
{
  extern Display *display;

  XTestFakeButtonEvent(display, button, True, CurrentTime);
  XTestFakeButtonEvent(display, button, False, CurrentTime);
  
  XFlush(display);
}