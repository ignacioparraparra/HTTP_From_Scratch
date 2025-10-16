OBJS = httpServer.o
C++ = g++
PORT = 8000
PROTOCOL = tcp

all: server

server: httpServer.o
			  $(C++) -o server httpServer.o

httpServer.o: httpServer.cpp
			  $(C++) -c httpServer.cpp -o httpServer.o

clean:
	rm *.o
	rm server
	fuser -k $(PORT)/$(PROTOCOL)