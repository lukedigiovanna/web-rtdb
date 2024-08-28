# Web Realtime Database (Web-RTDB)

Simple solution for developing real time web applications without the need
of implementing a backend. Best for low throughput, persistent, realtime
applications such as chatrooms.

## Usage

Web-RTDB functions on the concept of "stores". A store can be thought of as a
time ordered table, where each message stores five main pieces of information:

1. Message GUID
   1. Each message is uniquely defined my a global identifer (unique across stores) and persistent across updates.
2. Created Timestamp
   1. The time that this message was created
3. Updated Timestamp
   1. The time of the latest update to this message
4. TTL
   1. The length of time this message should persist before being marked "dead".
   2. Once a message is dead it's value can no longer be read and won't turn up in queries.
   3. The age of a message is calculated from the updated timestamp.
5. Payload
   1. The raw data stored in this message. Internally this is 

### Pub/Sub Model

Web-RTDB works over persistent web socket connections and a publisher/subscriber model.

**Low Latency Implementation**: When a client subscribes to a store they will receive a websocket message every time a message in that store is created/deleted/updated.

**High Throughput Implementation**: A future implementation that can support higher throughput may be to utilize a configurable buffer period (e.g. 50ms) in which we collect message updates during that period and send them in batches to subscribed clients. This of course increases latency of messages, but the message timeline can be preserved via update timestamps. 

In both implementations the client will only receive "delta updates", i.e. only the changes that occurred.

Ideally, the client can configure each store to use the appropriate implementation for their purposes.

### Logging and Recovery

Every write/update/deletion operation is written to an on-disk ledger which can then be used to reconstruct the database in the event the server goes down. This ensures the state of the application is never completely lost.

## Building

This project depends on `websocketpp`, a header-only library for implementing
websockets in C++. This also requires `asio`. These dependencies should be
placed at the root of the workspace in `dependencies/include`. Then the 
corresponding build script for your OS in `bin` can be ran to build the project.

## Running

After building the binary can be run directly on your machine. Currently there
are no environment parameters or configuration, the program is agnostic to its
use-case until it starts getting probed by clients.

## Client

This project contains a JavaScript/TypeScript client for super easy integration
into an existing project.