#ifndef TCPSERVER_H
#define TCPSERVER_H

#define PORT "8000"
#define MAXQUEUE 30
#define BUFFERLEN 4096
#define CRLF '\r\n\r\n'

#include "router.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

using namespace std;

namespace http
{

    class tcpServer {
    public:
        tcpServer(string, int, Router&);

        ~tcpServer();

        // getters
        int getPort();

        int getSocket();

        // setters

        // methods
        int start();

        int shutdown();

        int startListen();

        int acceptConnection();

        int handleClient();

        int sendResponse(string);

        string buildResponse(httpResponse res);

        void handleError();

        
    private:
        // variables 
        string ip;
        int port;
        int ret;
        char buffer[BUFFERLEN];
        int serverFD;
        int clientFD;
        Router* router;
        struct sockaddr_in socketAddress;
        socklen_t socketAddressLen;
        


        


    };

}

#endif