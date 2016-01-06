CC = g++
OPT = -Wall
SRC = src/
BUILD = build/
INCLUDES = /usr/local/include:/usr/lib/include
LIBS = /usr/local/lib/libNewton.so -lGL -lSDL -lGLU -lGLEW

all : executable.exe

executable.exe : Main.o 
	$(CC) $(OPT) $(BUILD)*.o -o executable.exe $(LIBS)

dMath : $(SRC)dMath/*
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)dMath/*.cpp
	mv *.o $(BUILD).

CVector.o : $(SRC)CVector.hpp $(SRC)CVector.cpp
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)CVector.cpp -o $(BUILD)CVector.o

Objet.o : $(SRC)Objet.hpp $(SRC)Objet.cpp CVector.o
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Objet.cpp -o $(BUILD)Objet.o

Boite.o : $(SRC)Boite.hpp $(SRC)Boite.cpp Objet.o
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Boite.cpp -o $(BUILD)Boite.o
Cylindre.o : $(SRC)Cylindre.hpp $(SRC)Cylindre.cpp Objet.o
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Cylindre.cpp -o $(BUILD)Cylindre.o
	
Scene.o : $(SRC)Scene.hpp $(SRC)Scene.cpp
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Scene.cpp -o $(BUILD)Scene.o
Drone.o : $(SRC)Drone.hpp $(SRC)Drone.cpp
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Drone.cpp -o $(BUILD)Drone.o
	
FlyComputerInterface.o : $(SRC)FlyComputerInterface.hpp $(SRC)FlyComputerInterface.cpp
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)FlyComputerInterface.cpp -o $(BUILD)FlyComputerInterface.o

Main.o : $(SRC)Main.cpp Boite.o Cylindre.o Scene.o Drone.o dMath FlyComputerInterface.o
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)Main.cpp -o $(BUILD)Main.o

clean:
	rm $(BUILD)*
	
mrproper:
	rm *.exe
	rm $(BUILD)*
