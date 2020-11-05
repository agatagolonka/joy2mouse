CC = g++
OBJ = joy2mouse.o Mouse.o Coord.o JoyDevice.o
CLINK = -L/usr/X11R6/lib -lXtst -lX11 
joy2mouse: $(OBJ)
	$(CC) -o joy2mouse $(OBJ) $(CLINK)
joy2mouse.o: joy2mouse.cpp JoyDevice.h Mouse.h
	$(CC) -c joy2mouse.cpp
JoyDevice.o: JoyDevice.cpp JoyDevice.h Mouse.h
	$(CC) -c JoyDevice.cpp
Mouse.o: Mouse.cpp Mouse.h
	$(CC) -c Mouse.cpp
Coord.o: Coord.cpp Coord.h
	$(CC) -c Coord.cpp
.PHONY : clean
clean:
	$(RM) *.o joy2mouse