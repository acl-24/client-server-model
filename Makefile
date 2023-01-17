all: server client

server: serverMain.o server.o tands.o
	g++ serverMain.o server.o tands.o -o server

client: clientMain.o client.o tands.o
	g++ clientMain.o client.o tands.o -o client

serverMain.o: serverMain.cpp
	g++ -c serverMain.cpp

clientMain.o: clientMain.cpp
	g++ -c clientMain.cpp

server.o: server.cpp server.h
	g++ -c server.cpp server.h

client.o: client.cpp client.h
	g++ -c client.cpp client.h

tands.o: tands.cpp tands.h
	g++ -c tands.cpp tands.h

clean:
	rm -f *.o
	rm -f *.gch