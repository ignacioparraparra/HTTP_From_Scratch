# HTTP/1.1 Server From Scratch
## Motivation
I was constantly working with abstraction after abstraction when doing web development. However I kept wondering how all of it worked.
What does Nodejs do under the hood? How does Express do routes? How does the wizard box we call a web browser display things? All that and more
has been cleared up tremendously with this project.

## About
This HTTP server creates a TCP socket to then, 

- Listen for incoming connections
- Parse and determine how to respond to request
- Send response to client
- Repeat

## Features
- Support basic HTTP/1.1 request and responses
- Support creating routes

## TODO
- Add testing functionality
- Clean up message parsing
- Add worker threads

## Design

## Testing