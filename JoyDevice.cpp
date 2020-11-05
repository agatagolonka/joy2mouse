#include "JoyDevice.h"


/********************************************************************/
/*                                                                  */
/*  Konstruktor inicjujacy obiekt JoyDevice, otwiera plik     	    */
/*  urzadzenia, odczytuje wszystkie dane o urzadzeniu		    */
/*                                                                  */
/********************************************************************/

JoyDevice :: JoyDevice(char *device)
{
  this->file_descriptor = open(device, O_RDONLY);

  if(this->file_descriptor <= 0)
  {
    std :: cout << "Nie udalo sie otworzyc " << device << std :: endl;
    exit(EXIT_FAILURE);
  }

  ioctl(file_descriptor, JSIOCGAXES, &this->axisCount);
  ioctl(file_descriptor, JSIOCGBUTTONS, &this->buttonCount);
  ioctl(file_descriptor, JSIOCGVERSION, &this->version);
  ioctl(file_descriptor, JSIOCGNAME(83), &this->devName);
  fcntl(file_descriptor, F_SETFL, O_NONBLOCK);

  this->axisPosition = new int[axisCount];
  this->buttonState = new int[buttonCount];

  int i = (int)this->axisCount;

  while(i--)
    this->axisPosition[i] = 0;

  i = (int)this->buttonCount;

  while(i--)
    this->buttonState[i] = 0;
}


/********************************************************************/
/*                                                                  */
/*  void printDeviceInfo()                                   	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      wypisywanie informacji (ilosc osi, przyciskow, wersja,	    */
/*	nazwa) o urzadzeniu					    */
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	void							    */
/*                                                                  */
/*  Funkcja zwraca:                                                 */
/*	void							    */
/*								    */
/*  Uzywane funkcje:                                                */
/*	brak							    */
/*								    */
/*  Uzywane zmienne:                                                */
/*      int axisCount - ilosc osi				    */
/*      int buttonCount - ilosc przyciskow			    */
/*	int version - wersja					    */
/*	char *devName - nazwa					    */
/*                                                                  */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

void JoyDevice :: printDeviceInfo()
{
  std :: cout << "Ilosc osi: " << (int)this->axisCount << std :: endl;
  std :: cout << "Ilosc przyciskow: " << (int)this->buttonCount << std :: endl;
  std :: cout << "Wersja: " << this->version << std :: endl;
  std :: cout << "Nazwa: " << this->devName << std :: endl;
}

/********************************************************************/
/*                                                                  */
/*  void printStates()                                   	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      wypisywanie informacji o aktualbym stanie osi i przyciskow  */
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	void							    */
/*                                                                  */
/*  Funkcja zwraca:                                                 */
/*	void							    */
/*								    */
/*  Uzywane funkcje:                                                */
/*	brak							    */
/*								    */
/*  Uzywane zmienne:                                                */
/*      int *axisPosition - stany osi				    */
/*      int *buttonState - staby przyciskow			    */
/*								    */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

void JoyDevice :: printStates()
{
  int i,j;

  for(i = 0; i < (int)this->axisCount; i++)
    printf("Axis %d:  %.5d  ", i, this->axisPosition[i]);
  putchar('\n');

  for(j = 0; j < (int)this->buttonCount; j++)
    printf("Button %d:  %d  ", j, this->buttonState[j]);
  putchar('\n');
}

/********************************************************************/
/*                                                                  */
/*  void toMouse()                                   	    	    */
/*                                                                  */
/*  Przeznaczenie:                                                  */
/*      odczyt polozen osi i stanow przyciskow, obliczanie polozenia*/
/*	kursora, zawiera petle glowna programu
/*                                                                  */
/*  Argumenty funkcji:                                              */
/*	void							    */
/*                                                                  */
/*  Funkcja zwraca:                                                 */
/*	void							    */
/*								    */
/*  Uzywane funkcje:                                                */
/*	setPosition						    */
/*	click							    */
/*	read - odczytuje informacje z pliku			    */
/*								    */
/*  Uzywane zmienne:                                                */
/*      int result - wartosc zwracana przez funkcje read	    */
/*      bool isRunning - gdy ustawiona na false program konczy	    */
/*	dzialanie						    */
/*	clock_t pause_L - wykorzystywana przy blokowaniu ponownego  */
/*	nieswiadomego klikniecia lewego przyciku myszy, dzieki temu */
/*	uzywanie joysticka jest latwe i praktycznie nie wymaga	    */
/*	wprawy							    */
/*	clock_t pause_R - jw. ale dla praweg przycisku		    */
/*	Coord *coord - wskaznik do typu Coord, sa tu przechowywane  */
/*	kolejne pozycje kursora					    */
/*	bool changed - ustawiana na true gdy w urzadzeniu doszlo do */
/*	zdarzenia						    */
/*	jsevent - struktura przechowujacy wartosci i typy zdarzen   */
/*	zachodzacych w urzadzeniu				    */
/*								    */
/*  Autor:                                                          */
/*      Dawid Stelmaszek, Informatyka Stosowana, III rok            */
/*      Mieszko Zakrzewski, Informatyka Stosowana, III rok          */
/*                                                                  */
/*  Ostatnia modyfikacja:                                           */
/*      7.06.2009						    */
/*                                                                  */
/********************************************************************/

void JoyDevice :: toMouse()
{
  int result;
  Mouse mouse;
  bool isRunning = true;
  clock_t pause_L = clock();
  clock_t pause_R = clock();
  Coord *coord = new Coord();
  bool changed = false;

  while(isRunning)
  {
    result = read(this->file_descriptor, &this->jsevent, sizeof(this->jsevent));
    
    if(result > 0)
    {
      switch (this->jsevent.type)
      {
	case JS_EVENT_INIT: 
	case JS_EVENT_INIT | JS_EVENT_AXIS: 
	case JS_EVENT_INIT | JS_EVENT_BUTTON:
	     break;
	case JS_EVENT_AXIS:
	     this->axisPosition[this->jsevent.number] = this->jsevent.value;
	     changed = true;
	     break;
	case JS_EVENT_BUTTON:
	     this->buttonState[this->jsevent.number] = this->jsevent.value;
	     changed = true;
	     break;
	default:
	     std :: cout << "Nieobslugiwany event.\n";
	     std :: cout << "Typ = " << this->jsevent.type << std :: endl;
	     std :: cout << "Numer = " << this->jsevent.number << std :: endl;
	     std :: cout << "Wartosc = " << this->jsevent.value << std :: endl;
	     break;
      }
    }
  
    if(changed)
    {
      if(!strcmp(this->devName, "Cypress Gamepad"))
      {
	coord->setX((axisPosition[1]) / (65534 / XRES));
	if(coord->getX() >= XRES)
	  coord->setX(XRES - 1);

      	coord->setY((axisPosition[0]) / (65534 / YRES));
	if(coord->getY() >= YRES)
	  coord->setY(YRES - 1);
      }
      else if(!strcmp(this->devName, "Twin USB Joystick"))
      {
	coord->setX((axisPosition[0] + 32767) / (65534 / XRES));
	if(coord->getX() >= XRES)
	  coord->setX(XRES - 1);

	coord->setY((axisPosition[1] + 32767) / (65534 / YRES));
	if(coord->getY() >= YRES)
	  coord->setY(YRES - 1);
      }

      this->printStates();

      mouse.setPosition(coord);

      if(this->buttonState[0] == 1)
      {
	if(pause_L < clock())
	{
	  pause_L = clock() + CLOCKS_PER_SEC / 350;
	  mouse.click(LEFT_BUTTON);
	}
      }

      if(this->buttonState[1] == 1)
      {
	if(pause_R < clock())
	{
	  pause_R = clock() + CLOCKS_PER_SEC / 350;
	  mouse.click(RIGHT_BUTTON);
	}
      }

      if(buttonState[6] == 1 && buttonState[7] == 1)
	isRunning = false;
    }
  }

  close(this->file_descriptor);
}

