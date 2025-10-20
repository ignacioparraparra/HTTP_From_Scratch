#ifndef ROUTER_H
#define ROUTER_H

#include "httpmessage.h"
#include <functional>

class Router {
    public:      // endpoint, callback func
        Router();
        ~Router();

        using Handler = std::function<httpResponse(const httpRequest&)>;

        httpResponse lookup(httpRequest req);

        // add method, path with associated handler
        void add(string method, string uri, Handler);

        void getRoutes();

    private:
        unordered_map <string, Handler> routes;
};

#endif