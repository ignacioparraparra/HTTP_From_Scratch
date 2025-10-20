#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <string>
#include <unordered_map>

using namespace std;

enum class httpMethod {
    GET,
    HEAD,
    UPDATE,
    POST,
    PUT,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH
};

enum class HttpStatusCode {
    Continue = 100,
    SwitchingProtocols = 101,
    EarlyHints = 103,
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    ImATeapot = 418,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnvailable = 503,
    GatewayTimeout = 504,
    HttpVersionNotSupported = 505
};

class httpRequest {
    public:
        httpRequest(string);
        ~httpRequest();
        
        // Setters 
        void       setUri(string);
        void       setMethod(httpMethod);
        void       setVersion(string);
        
        // Getters
        string     getMethod();
        void       getHeaders();
        string     getUri();
        string     getVersion();
        void       getRequest();

        // Utils
        httpMethod stringToMethod(string method);
        string     methodToString(httpMethod method);

    private:
        httpMethod                    method;
        string                        version;
        string                        uri;
        unordered_map<string, string> header;
};

class httpResponse {
    public:
        string status;
        string headers;
        string body;
        string version;
        string reason;
    private:
        
};

#endif