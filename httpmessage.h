#include <string>

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
        void setMethod(httpMethod method);
        void setUri(string);

        httpMethod getMethod();
        string getUri();

    private:
        string method;
        string uri;
};

class httpResponse {
    public:
        string status;
        string headers;
        string body;
    private:
        enum header {};
};

