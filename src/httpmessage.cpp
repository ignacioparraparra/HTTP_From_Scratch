#include "httpmessage.h"

#include <iostream>
#include <sstream>

httpRequest::httpRequest(string req) {

        istringstream stream(req); 
        string line;

        // request line = Method SP Request-URI SP HTTP-Version CRLF
        getline(stream, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t firstSpace = line.find(" ");
        string methodName {line.substr(0, firstSpace)};
        size_t secondSpace = line.find(" ", firstSpace + 1);
        string uri {line.substr(firstSpace + 1, secondSpace - firstSpace - 1)};
        size_t rest = line.find("\r\n", secondSpace + 1);
        string httpVersion (line.substr(secondSpace + 1, rest - secondSpace - 1));
        setMethod(stringToMethod(methodName));
        setUri(uri);
        setVersion(httpVersion);

        // headers
        while (getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (line.empty()) break;

            size_t colon = line.find(":");
            string key {line.substr(0, colon)};
            size_t eol = line.find("\r\n", colon + 1);
            string value {line.substr(colon + 1, eol - colon - 1)};

            header[key] = value;
            // store into a request object
        }
}

httpRequest::~httpRequest() {

}

void httpRequest::setMethod(httpMethod _method) {
    method = _method;
}

void httpRequest::setVersion(string _version) {
    version = _version;
}

void httpRequest::setUri(string _uri) {
    uri = _uri;
}

string httpRequest::getMethod() {
    return methodToString(method);
}

string httpRequest::getUri() {
    return uri;
}

string httpRequest::getVersion() {
    return version;
}

void httpRequest::getHeaders() {
    for (const auto& [key, value] : header) {
        cout << "\nkey: " << key;
        cout << ", Value: " << value;
    }
}

void httpRequest::getRequest() {
    cout << "Method: " << getMethod() << endl;
    cout << "Uri: " << getUri() << endl;
    cout << "Version: " << getVersion() << endl;
    cout << "Header: ";
    getHeaders();
}

httpMethod httpRequest::stringToMethod(string method) {
    if (method == "GET") {
        return httpMethod::GET;
    }
    else if (method == "POST") {
        return httpMethod::POST;
    }
    else {
        throw invalid_argument("Unexpected Method");
    }
}

string httpRequest::methodToString(httpMethod method) {
    switch(method) {
        case httpMethod::GET:
            return "GET";
        case httpMethod::HEAD:
            return "HEAD";
        case httpMethod::UPDATE:
            return "UPDATE";
        case httpMethod::POST:
            return "POST";
        case httpMethod::PUT:
            return "PUT";
        case httpMethod::TRACE:
            return "TRACE";
        case httpMethod::OPTIONS:
            return "OPTIONS";
        case httpMethod::CONNECT:
            return "CONNECT";
        case httpMethod::PATCH:
            return "PATCH";
        default:
            return string();
    }
}

