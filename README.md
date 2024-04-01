# RTDB (Realtime Database)

This is an implementation of a simple realtime database using key-value stores.

## Usage

1. Connect to the TCP server
2. Send messages to create, read, update, and delete key-value stores.
3. All values are stored as strings, for simplicity.

Use the client script provided in `/demo/client.js` to interface with the server from the command line.

### Commands:

*Checkmark indicates that the function has been implemented*

* [x] `PING`: Assuming the connection is open, the server will simply respond with `PONG`
* [x] `EXIT`: Used to terminate a connection
* [x] `SET <key> <value>`: Sets the given key to whatever value is on the right
* [x] `QUERY <key>`: Reports the value of the given key, if it exists.
* [ ] `DELETE <key>`: Deletes the given key value
* [ ] `SUBSCRIBE <key>`: Subscribes the client to recieve broadcasted updates to the specified key
* [ ] `UNSUBSCRIBE <key>`: Unsubscribes the client from a key's broadcast updates
* [ ] `UNSUBSCRIBE_ALL`: Unsubscribes the client from all keys' broadcast updates