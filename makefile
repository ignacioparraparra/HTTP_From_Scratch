OBJS = tcpserver.o
C++ = g++
PORT = 8000
PROTOCOL = tcp

all:         server

server:        server.o tcpserver.o httpmessage.o router.o
			   $(C++) -o server server.o tcpserver.o httpmessage.o router.o

server.o:      server.cpp
			   $(C++) -c server.cpp -o server.o

tcpserver.o:   tcpserver.cpp
			   $(C++) -c tcpserver.cpp -o tcpserver.o

httpmessage.o: httpmessage.cpp
			   $(C++) -c httpmessage.cpp -o httpmessage.o

router.o:      router.cpp
			   $(C++) -c router.cpp -o router.o

clean:
	rm *.o
	rm server
	lsof -iTCP:$(PORT) -sTCP:LISTEN
	fuser -k $(PORT)/$(PROTOCOL)

