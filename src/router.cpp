#include "router.h"
#include <iostream>

using namespace std;

Router::Router() {

}

Router::~Router() {

}

// performs lookup on req, returns res from handler
httpResponse Router::lookup(httpRequest req) {
    const string route = req.getMethod() + ' ' + req.getUri();
//   cout << "\nRoute: " << route ;

    auto it = routes.find(route);
    if (it == routes.end() || !it->second) {
            // 404 fallback (set a complete response)
            httpResponse res;
            res.version = "HTTP/1.1\r\n";
            res.status  = "404\r\n";
            res.reason  = "Not Found";
            res.headers ="Content-Type: text/plain; charset=utf-8";
            res.body = "Not Found\r\n\r\n";
            // leave Content-Length/Connection for the server layer to add uniformly
            return res;
    } else {
        return it->second(req);
    }

}

void Router::add(string method, string uri, Handler handler) {
    string key = method + " " + uri;
//    cout << key << endl;

    routes[key] = move(handler);
}


void Router::getRoutes() {
    for (const auto& [key, value] : routes) {
        cout << "Uri: " << key << endl;
    }
}

