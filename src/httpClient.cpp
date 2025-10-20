#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sstream>

#include "connection.h"

using namespace std;

int main() {
    char                    buffer[BUFFER_SIZE];
    int                     size;
    int                     clientFD;
    bool                    session = true;
    struct addrinfo         *node, *head;
    struct addrinfo         hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = 0;
    hints.ai_protocol = 0;


    int ret = getaddrinfo("0.0.0.0", PORT, &hints, &head);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    close(clientFD);
    clientFD = -1;

    for (node = head; node != NULL; node = node->ai_next) {
        clientFD = socket(node->ai_family, node->ai_socktype, node->ai_protocol);




                printf("ai_family = %i, ai_socktype = %i, ai_flags = %i\n",
               node->ai_family, node->ai_socktype, node->ai_flags);

        char ip[32];

        if (node->ai_family == AF_INET) {
                struct sockaddr_in* ipdata = (struct sockaddr_in*)node->ai_addr;
                inet_ntop(node->ai_family, &ipdata->sin_addr, ip, 32);
                int port = ntohs(ipdata->sin_port);
                printf("IP = %s, PORT = %i\n", ip, port);
        }
        else if (node->ai_family == AF_INET6) {
                struct sockaddr_in6* ipdata = (struct sockaddr_in6*)node->ai_addr;
                inet_ntop(node->ai_family, &ipdata->sin6_addr, ip, 32);
                int port = ntohs(ipdata->sin6_port);
                printf("IP = %s, PORT = %i\n", ip, port);
        }

        if (clientFD == -1) {
            continue;
        }

        if (connect(clientFD, node->ai_addr, node->ai_addrlen) == -1) {
            printf("Connection failed, retrying...\n");
            node = head;
        }

        if (connect(clientFD, node->ai_addr, node->ai_addrlen) == 0) {
            break;
        }

        if (errno == EISCONN) {
            break;
        }

        perror("connect failed");
        printf("Error no: %i\n", errno);
        close(clientFD);
        clientFD = -1;
    }   

    freeaddrinfo(head);

    if (node == NULL) {
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }


    /*
    // SYN
    if (send(clientFD, SYN, sizeof(SYN), 0) == -1) {
        perror("send");
        close(clientFD);
        exit(errno);       
    }

    // SYN-ACK
    size = recv(clientFD, buffer, sizeof(buffer), 0);
    if (size == -1) {
        printf("Error on recv: %i\n", errno);
        close(clientFD);
        exit(errno);      
    }

    buffer[size] = '\0';
        
    printf("%s\n", buffer);

    // ACK
    if (send(clientFD, ACK, sizeof(ACK), 0) == -1) {
        perror("send");
        close(clientFD);
        exit(errno);        
    }
    */
    // message loop
 
   //     cout << "COMMANDS: EXIT" << endl;
     //   cout << "What would you like to send to the server?\n";
       // cin.getline(buffer, sizeof(buffer));

            // method -> builds request based on path
       // server.get(path)

        ostringstream ss;
        ss << "GET / HTTP/1.1\r\n"
           << "Host: developer.mozilla.org\r\n"
           << "Accept: text/html\r\n"
           << "Accept-Language: fr\r\n\r\n";

        string request = ss.str();

        size_t len = strlen(buffer);

        ssize_t sent = send(clientFD, request.c_str(), request.size(), 0);
        if (sent == -1) {
            perror("send");
            close(clientFD);
            printf("Error sending message, disconnected from server...");
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, "EXIT", 5) == 0) {
            session = false;
        }

        memset(buffer, 0, 1024);
        
        ret = recv(clientFD, buffer, sizeof(buffer), 0);
        buffer[ret] = '\0';

        printf("Recieved: %s\n", buffer);
    

    printf("Disconnecting...\n");
    close(clientFD);
    exit(EXIT_SUCCESS);
}