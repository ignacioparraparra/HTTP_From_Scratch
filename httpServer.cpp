#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>

using namespace std;

#include "connection.h"

int main(void) {
    int                 serverFD;
    int                 clientFD;
    int                 epollFD;
    int                 rfd;
    int                 size;
    int                 ret;
    char                buffer[BUFFER_SIZE];
    bool                session = true;
    struct epoll_event  ev, events[MAX_EVENTS];
    struct addrinfo     *node, *head;
    struct addrinfo     hints;

    // set data in hints to 0 to n size
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_UNSPEC;       // use either ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;     // use TCP
    hints.ai_flags    = AI_PASSIVE;      // make port suitable for binding       
    hints.ai_protocol = 0;

    ret = getaddrinfo(NULL, PORT, &hints, &head);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    cout << "getaddrinfo success" << endl;
    
    printf("AF_INET = %i, AF_INET6 = %i, SOCK_STREAM = %i \n", AF_INET, AF_INET6, SOCK_STREAM);

    close(serverFD);
    close(clientFD);
    // try each address until we successfully bind 
    for (node = head; node != NULL; node = node->ai_next) {
       
       
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
    
        serverFD = socket(node->ai_family, node->ai_socktype, node->ai_protocol);

        // if we can't connect to socket try another address
        if (serverFD == -1) { 
            continue;
        }

        // successfully reserved address, exit
        if (bind(serverFD, node->ai_addr, node->ai_addrlen) == 0) {
            break;
        }

        // Print WHY bind failed for this candidate
        perror("bind failed");
        close(serverFD);
        serverFD = -1;
    }

    // once binded we can free up address
    freeaddrinfo(head);

    // no address was found
    if (node == NULL) {
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    // this port is open for business
    ret = listen(serverFD, 30);
    if (ret != 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listening on Port: %s\n", PORT);

    fcntl(serverFD, F_SETFD, O_NONBLOCK);
    
    // create epoll instance to track events, returns fd
    epollFD = epoll_create1(0);
    if (epollFD == -1) {
        perror("epoll_create1");
        exit(errno);
    }

    ev.events = EPOLLIN;
    ev.data.fd = serverFD;
    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, serverFD, &ev) == -1) {
        perror("epoll_ctl: serverFD");
        exit(errno);
    }

    // event loop
    cout << "Waiting for connection...\n";
    // accepting client loop
    for (;;) {

        rfd = epoll_wait(epollFD, events, MAX_EVENTS, -1);
        if (rfd == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < rfd; ++n) {
            if (events[n].data.fd == serverFD) {
                clientFD = accept(serverFD, NULL, NULL);
                if (clientFD == -1) {
                    perror("accept");
                    exit(errno);
                }
                cout << "Client connection established\n";
                
                //  setnonblocking(clientFD);
                fcntl(clientFD, F_SETFD, O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientFD;
                // add client to epoll interest list
                if (epoll_ctl(epollFD, EPOLL_CTL_ADD, clientFD, &ev) == -1) {
                        perror("epoll_ctl: clientFD");
                        exit(EXIT_FAILURE);
                }
            } else {

                printf("Currently servicing %i : %i\n", events[n].data.fd, clientFD);

                size = recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                
                if (size == EAGAIN) {
                    fprintf(stderr, "EAGAIN TRIGGER %i", errno);
                    break;
                }

                buffer[size] = '\0';

                if (strncmp(buffer, "EXIT", 5) == 0) {
                    if (epoll_ctl(epollFD, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1) {
                    perror("epoll_ctl: clientFD");
                    close(clientFD);
                    break;    
                    }
                }

                printf("Message Recieved: %s\n", buffer); 
                
                
                // do_use_fd(events[n].data.fd)
                


                /*
                // this uses client FD
                printf("events[n].data.fd != serverFD\n");

                // Handle Message
                size = recv(clientFD, buffer, sizeof(buffer), 0);
                if (size == -1) {
                    printf("Error on recv: %i\n", errno);
                    exit(errno);
                } 

                buffer[size] = '\0';

                if (strncmp(buffer, "EXIT", 5) == 0) {
                    if (epoll_ctl(epollFD, EPOLL_CTL_DEL, clientFD, &ev) == -1) {
                    perror("epoll_ctl: clientFD");
                    exit(EXIT_FAILURE);
                    close(clientFD);
                    break;    
                }
                }

                printf("Message Recieved: %s\n", buffer); 
                */                   
        }

                

     /*       
        clientFD = accept(serverFD, NULL, NULL);
        if (clientFD == -1) {
            perror("accept");
            exit(errno);
        }

        // SYN
        size = recv(clientFD, buffer, sizeof(buffer), 0);
        if (size == -1) {
            printf("Error on recv: %i\n", errno);
            close(clientFD);
            continue;           
        }

        buffer[size] = '\0';
        
        printf("%s\n", buffer);

        // SYN-ACK
        if (send(clientFD, SYN_ACK, sizeof(SYN_ACK), 0) == -1) {
            perror("send");
            close(clientFD);
            continue;
        }

        // ACK
        size = recv(clientFD, buffer, sizeof(buffer), 0);
        if (size == -1) {
            printf("Error on recv: %i\n", errno);
            close(clientFD);
            continue;           
        }        

        buffer[size] = '\0';
        
        printf("%s\n", buffer);

        cout << "Client connected\n";

        // message loop
        while (session) {
            size = recv(clientFD, buffer, sizeof(buffer), 0);
            if (size == -1) {
                printf("Error on recv: %i\n", errno);
                exit(errno);
            } 

            buffer[size] = '\0';

            if (strncmp(buffer, "EXIT", 5) == 0) {
                session = false;  
            }

            printf("Message Recieved: %s\n", buffer);
            */
        }
       
    }
     close(serverFD);
}
