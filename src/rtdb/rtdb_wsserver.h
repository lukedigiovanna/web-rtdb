#ifndef RTDB_WSSERVER_H
#define RTDB_WSSERVER_H

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>

#include <iostream>

namespace rtdb {

class WSServer {
public:
    typedef std::function<void(void)> MessageCb;
    typedef websocketpp::server<websocketpp::config::asio> Server;

private:
    Server d_server;
    MessageCb d_messageCb;

    void onMessage(websocketpp::connection_hdl handle, Server::message_ptr msg);
    void onOpen(websocketpp::connection_hdl handle);
    void onClose(websocketpp::connection_hdl handle);

public:
    // Disable the default and copy constructor
    WSServer() = delete;
    WSServer(const WSServer& s) = delete;

    WSServer(unsigned short port);
    ~WSServer();

    void setOnMessageCallback(MessageCb cb);
    
    // Starts listening to connections on the current thread.
    void start();
};

}

#endif