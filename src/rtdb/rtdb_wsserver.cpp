#include "rtdb_wsserver.h"

namespace rtdb {

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WSServer::WSServer(WSServerConfig config) :
    d_config(config) {
    try {
        // Disable logging frames
        d_server.clear_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload); 

        d_server.init_asio();
        d_server.set_reuse_addr(true);

        d_server.set_message_handler(bind(&WSServer::onMessage, this, _1, _2));
        d_server.set_close_handler(bind(&WSServer::onClose, this, _1));

        d_server.listen(9002);
    }
    catch (const websocketpp::exception& e) {
        // LOG
    }
}

WSServer::~WSServer() {

}

void WSServer::run() {
    try {
        d_server.run();
    }
    catch (const websocketpp::exception& e) {
        // LOG
    }
}

void WSServer::onMessage(
    websocketpp::connection_hdl handle, 
    Server::message_ptr msg) {
    std::cout << "got message\n";
}

void WSServer::onClose(websocketpp::connection_hdl handle) {
    
}

}