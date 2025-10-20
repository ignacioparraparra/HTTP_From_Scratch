#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>

#include "tcpserver.h"
#include "httpmessage.h"

namespace http {

    tcpServer::tcpServer(string ip, int port, Router& _router) {

        this->router = &_router;

        memset(&socketAddress, 0, sizeof(socketAddress));
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_port = htons(port);
        socketAddress.sin_addr.s_addr =  htonl(INADDR_ANY);
        socketAddressLen = sizeof(socketAddress);

        if (start() < 0) {
            printf("Could not start server on %i\n", ntohs(socketAddress.sin_port));
            exit(EXIT_FAILURE);
        }
    }

    tcpServer::~tcpServer() {
        shutdown();
    }

    void tcpServer::handleError() {
        exit(EXIT_FAILURE);
    }
    

    int tcpServer::start() {

       serverFD = socket(AF_INET, SOCK_STREAM, 0);
      
        if (serverFD < 0) {
            perror("socket");
            handleError();
        }

        if (bind(serverFD, (sockaddr*)&socketAddress, socketAddressLen) < 0) {
            perror("bind");
            handleError();
        }

        printf("Server started successfully\n");

        return 1;
    }

    int tcpServer::shutdown() {
        if (close(serverFD) < 0) {
            perror("close");
            handleError();
        }
        printf("Server has shutdown\n");
        return 1;
    
    }

    int tcpServer::startListen() {
        if (listen(serverFD, MAXQUEUE) < 0) {
            perror("listen");
            handleError();
        }

        ostringstream ss;
        ss << "--- Listening on "
           << inet_ntoa(socketAddress.sin_addr) 
           << "---\n"
           << "--- on Port "
           << ntohs(socketAddress.sin_port)
           << "---\n";

        string message = ss.str();
        cout << message;

        while(true) {
            acceptConnection();

            handleClient();

        }

        return 1;
    }   

    int tcpServer::acceptConnection() {
        clientFD = accept(serverFD, NULL, NULL);
        if (clientFD < 0) {
            perror("accept");
            handleError();
        }

        printf("---Client Connected---\n");

        return 1;
    }

    int tcpServer::handleClient() {
        memset(buffer, 0, BUFFERLEN);
        ssize_t bytes_recv = recv(clientFD, buffer, sizeof(buffer), 0);
        if (ret < 0) {
            perror("recv");
            close(clientFD);
            return -1;
        }

        string request(buffer, bytes_recv);

    //    cout << "here\n"; 

        httpRequest req = httpRequest(request);

    //    req.getRequest();  // main made it here

        httpResponse res = router->lookup(req);

    //    cout << "Test\n";

        string Response = buildResponse(res);
       
        sendResponse(Response);

        return 1;
    }

    int tcpServer::sendResponse(string message) { 

        if (send(clientFD, message.c_str(), message.size(), 0) < 0) {
            perror("send");
            handleError();
        }

        return 1;
    }

    string tcpServer::buildResponse(httpResponse res) {
        string _res = res.version + res.status + res.reason + "\r\n" + res.headers + "\r\n" + res.body;

        return _res;

    }
    
}