# RTDB (Realtime Database)

This is an implementation of a simple realtime database using key-value stores.

## Usage

1. Connect to the TCP server
2. Send messages to create, read, update, and delete key-value stores.
3. All values are stored as strings, for simplicity.

Use the client script provided in `/demo/client.js` to interface with the server from the command line.

### Commands:

* `PING`: Assuming the connection is open, the server will simply respond with `PONG`
* `EXIT`: Used to terminate a connection
* `SET <key> <value>`: Sets the given key to whatever value is on the right
* `QUERY <key>`: Reports the value of the given key, if it exists.