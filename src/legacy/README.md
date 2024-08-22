This folder contains the original implementation of RTDB, which targeted a 
TCP interface.

Since I primarily wanted to use this as a backend to frontends built in a 
browser, a TCP interface is inconvenient. 

------------------------------------------------------------------------

# RTDB (Realtime Database)

This is an implementation of a simple realtime database using key-value stores.

## Usage

1. Connect to the TCP server
2. Send messages to create, read, update, and delete key-value stores.
3. All values are stored as strings, for simplicity.

Use the client script provided in `/demo/client.js` to interface with the server from the command line.

Alternatively, interface with an instance through the web client:

1. Run the server on a port: `./build/server 8080`
2. Run the proxy using node.js (this sets up a WebSocket that forwards requests to the regular TCP client): `node demo/proxy.js`
3. Open `demo/index.html` and interface with the database

### Commands:

*Checkmark indicates that the function has been implemented*

* [x] `PING`: Assuming the connection is open, the server will simply respond with `PONG`
* [x] `EXIT`: Used to terminate a connection
* [x] `SET <key> <value>`: Sets the given key to whatever value is on the right
* [x] `QUERY <key>`: Reports the value of the given key, if it exists.
* [ ] `DELETE <key>`: Deletes the given key value
* [x] `SUBSCRIBE <key>`: Subscribes the client to recieve broadcasted updates to the specified key
* [ ] `UNSUBSCRIBE <key>`: Unsubscribes the client from a key's broadcast updates
* [ ] `UNSUBSCRIBE_ALL`: Unsubscribes the client from all keys' broadcast updates