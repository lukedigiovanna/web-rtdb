#include "rtdb_wsserver.h"

#include "rtdb_logger.h"

namespace rtdb {

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WSServer::WSServer(unsigned short port) {
    try {
        // Disable websocketpp logging.
        d_server.clear_access_channels(websocketpp::log::alevel::all); 

        d_server.init_asio();
        d_server.set_reuse_addr(true);

        d_server.set_message_handler(bind(&WSServer::onMessage, this, _1, _2));
        d_server.set_close_handler(bind(&WSServer::onClose, this, _1));
        d_server.set_open_handler(bind(&WSServer::onOpen, this, _1));

        d_server.listen(port);
    }
    catch (const websocketpp::exception& e) {
        LOG_ERROR << e.what();
    }
}

WSServer::~WSServer() {
    LOG_INFO << "Shutting down server.";
    d_server.stop();
    LOG_INFO << "Server stopped.";
}

void WSServer::start() {
    try {
        d_server.start_accept();
        d_server.run();
    }
    catch (const websocketpp::exception& e) {
        LOG_ERROR << e.what();
    }
}

void WSServer::onMessage(
    websocketpp::connection_hdl handle, 
    Server::message_ptr msg) {
    LOG_INFO << "Got message: " << msg->get_payload();
    LOG_WARNING << "onMessage not implemented.";
}

void WSServer::onOpen(websocketpp::connection_hdl handle) {
    auto conn = d_server.get_con_from_hdl(handle);
    LOG_INFO << "Client connected: " << conn->get_origin();
}

void WSServer::onClose(websocketpp::connection_hdl handle) {
    auto conn = d_server.get_con_from_hdl(handle);
    LOG_INFO << "Client disconnected: " << conn->get_uri();
}

}