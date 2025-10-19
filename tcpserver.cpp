#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>

#include "tcpserver.h"

namespace http {

    tcpServer::tcpServer(string ip, int port) {

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

        istringstream stream(request); 
        string line;

        // request line
        getline(stream, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t firstSpace = line.find(' ');
        string methodName {line.substr(0, firstSpace)};
        size_t secondSpace = line.find(' ', firstSpace + 1);
        string path {line.substr(firstSpace + 1, secondSpace - firstSpace - 1)};
        size_t rest = line.find('\r\n', secondSpace + 1);
        string httpVersion (line.substr(secondSpace + 1, rest - secondSpace - 1));
        cout << "Method: " << methodName << " URI: " << path << " Version: " << httpVersion << endl;

        // headers
        while (getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (line.empty()) break;

            size_t colon = line.find(': ');
            string key {line.substr(0, colon)};
            size_t eol = line.find('\r\n', colon + 1);
            string value {line.substr(colon + 1, eol - colon - 1)};

            cout << "Key: " << key << "\n Value: " << value << endl;
            // store into a request object
        }
    
        /*
        // Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
        size_t firstSpace = request.find(' ');
        string methodName {request.substr(0, firstSpace)};
        size_t secondSpace = request.find(' ', firstSpace + 1);
        string path {request.substr(firstSpace + 1, secondSpace - firstSpace - 1)};
        size_t rest = request.find('\r\n', secondSpace + 1);
        string httpVersion (request.substr(secondSpace + 1, rest - secondSpace - 1));
        
        cout << "Method: " << methodName << " path: " << path << endl
        << "Version: " << httpVersion << endl;


        // whatever remains of the remaining header instructions, key value pairs
        size_t headerBody = request.find('\r\n\r\n');
        string httpHash {request.substr(rest + 1, headerBody - rest - 1)};


        size_t colon = httpHash.find(':');
        string key {httpHash.substr(0, colon)};
        size_t eol = httpHash.find('\r\n', colon + 1);
        string value {httpHash.substr(colon + 1, eol - colon - 1)};

        cout << "Parsed first line key: " << key << " value: " << value << endl;
        */


        // request header fields

        /*
        size_t firstSpace = request.find(' ');
        string methodName {request.substr(0, firstSpace)};
        size_t secondSpace = request.find(' ', firstSpace + 1);
        string path {request.substr(firstSpace + 1, secondSpace - firstSpace - 1)};

        cout << "Method: " << methodName << " path: " << path << endl;
         */

        // Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
        

        string clientResponse {};

     //   if (path != "/") {
    //        clientResponse = "HTTP/1.1 404 Not Found\r\n";
    //    }
      //  else {
            string body = "<h1>hello world<h1>";
            clientResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + to_string(body.size()) + "\r\nConnections: keep-alive\r\n\r\n" + body;
     //   }

       

        sendResponse(clientResponse);

        return 1;
    }

    int tcpServer::sendResponse(string message) { 

        if (send(clientFD, message.c_str(), message.size(), 0) < 0) {
            perror("send");
            handleError();
        }

        return 1;
    }


    
}