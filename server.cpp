#include "tcpserver.h"
#include <iostream>

using namespace std;

int main() {

    using namespace http;

    tcpServer server = tcpServer("127.0.0.1" , 8000);

    server.startListen();
    
    return 0;
}