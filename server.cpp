#include "tcpserver.h"
#include <iostream>

using namespace std;

int main() {

    using namespace http;

    Router router;

    auto rootHandler = [](const httpRequest& req) -> httpResponse {
    httpResponse res;
    res.status = " 200";
    res.reason = " OK";
    res.version = "HTTP/1.1";
    res.body    = "<h1>hello world<h1>";
    res.headers = "Content-Type: text/html; charset=utf-8\r\nConnections: keep-alive\r\nContent-Length: " + to_string(res.body.size()) + "\r\n\r\n";
    return res;};

    auto aboutHandler = [](const httpRequest& req) -> httpResponse {
    httpResponse res;
    res.status = " 200";
    res.reason = " OK";
    res.version = "HTTP/1.1";
    res.body    = "<div><h1>about page</h1></div><div><button>Submit</button></div>";
    res.headers = "Content-Type: text/html; charset=utf-8\r\nConnections: keep-alive\r\nContent-Length: " + to_string(res.body.size()) + "\r\n\r\n";
    return res;};

    router.add("GET", "/", rootHandler);

    router.add("GET", "/about", aboutHandler);

    router.getRoutes();

    tcpServer server = tcpServer("127.0.0.1" , 8000, router);

    server.startListen();
    
    return 0;
}