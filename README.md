# Web Realtime Database (Web-RTDB)

Simple solution for developing real time web applications without the need
of implementing a backend. Best for low throughput, persistent, realtime
applications such as chatrooms.

## Running

## Usage

Web-RTDB works over a web socket connection. 

## Building

This project depends on `websocketpp`, a header-only library for implementing
websockets in C++. This also requires `asio`. These dependencies should be
placed at the root of the workspace in `dependencies/include`. Then the 
corresponding build script in `bin` can be run to build the project.

## Client

This project contains a JavaScript/TypeScript client for super easy integration
into an existing project.