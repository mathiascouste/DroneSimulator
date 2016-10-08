CC = g++-5 -std=c++1y
OPT = -Wall
SRC = src/
BUILD = build/
INCLUDES = /usr/local/include:/usr/lib/include

all : simulator.exe server.exe

simulator.exe : $(SRC)Main.cpp Boite.o Cylindre.o Scene.o Drone.o dMath FlyComputerInterface.o
	$(CC) $(OPT) $(BUILD)*.o $(SRC)Main.cpp -o simulator.exe /usr/local/lib/libNewton.so -lGL -lSDL -lGLU -lGLEW
	
server.exe : $(SRC)Server.cpp libhttpdrest
	$(CC) $(OPT) $(BUILD)*.o $(SRC)Server.cpp -o server.exe
	

#### compilation de lib dMath ####
dMath : $(SRC)dMath/*
	$(CC) $(OPT) -I $(INCLUDES) -c $(SRC)dMath/*.cpp
	mv *.o $(BUILD).

#### compilation de lib httpdrest ####
libhttpdrest : method.o path.o request.o response.o serverPort.o service.o
method.o : $(SRC)httpdrest/method.cpp $(SRC)httpdrest/method.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/method.cpp -o $(BUILD)method.o
path.o : $(SRC)httpdrest/path.cpp $(SRC)httpdrest/path.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/path.cpp -o $(BUILD)path.o
request.o : $(SRC)httpdrest/request.cpp $(SRC)httpdrest/request.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/request.cpp -o $(BUILD)request.o
response.o : $(SRC)httpdrest/response.cpp $(SRC)httpdrest/response.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/response.cpp -o $(BUILD)response.o
serverPort.o : $(SRC)httpdrest/serverPort.cpp $(SRC)httpdrest/serverPort.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/serverPort.cpp -o $(BUILD)serverPort.o
service.o : $(SRC)httpdrest/service.cpp $(SRC)httpdrest/service.hpp
	$(CC) $(OPT) -c $(SRC)httpdrest/service.cpp -o $(BUILD)service.o
	
#### compilation du metier ####

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


clean:
	rm $(BUILD)*
	
mrproper:
	rm *.exe
	rm $(BUILD)*
