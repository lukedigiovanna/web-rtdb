#include "rtdb_wsserver.h"

#include "rtdb_command.h"
#include "rtdb_logger.h"
#include "rtdb_responseencoder.h"

namespace rtdb {

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WSServer::WSServer(unsigned short port) : port(port) {
    try {
        // Disable websocketpp logging.
        d_server.clear_access_channels(websocketpp::log::alevel::all);

        d_server.init_asio();
        d_server.set_reuse_addr(true);

        d_server.set_message_handler(bind(&WSServer::onMessage, this, _1, _2));
        d_server.set_close_handler(bind(&WSServer::onClose, this, _1));
        d_server.set_open_handler(bind(&WSServer::onOpen, this, _1));

        d_server.listen(port);
    } catch (const websocketpp::exception &e) {
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

        LOG_INFO << "Listening for clients on ::" << port;

        d_server.run();
    } catch (const websocketpp::exception &e) {
        LOG_ERROR << e.what();
    }
}

void WSServer::setOnMessageCallback(MessageCb cb) { d_messageCb = cb; }

void WSServer::setOnCloseCallback(CloseCb cb) { d_closeCb = cb; }

void WSServer::onMessage(websocketpp::connection_hdl handle,
                         Server::message_ptr msg) {
    LOG_INFO << "Got message: " << msg->get_payload();
    if (d_messageCb) {
        auto connection = d_server.get_con_from_hdl(handle);
        try {
            Command command(msg->get_payload());
            d_messageCb(connection, command);
        } catch (CommandParseError &err) {
            LOG_ERROR << err.what();
            connection->send(ResponseEncoder::encodeError(err.what()));
        } catch (websocketpp::exception &err) {
            LOG_ERROR << err.what();
        }
    } else {
        LOG_WARNING << "WebSocket server received a message but no message "
                       "callback is set";
    }
}

void WSServer::onOpen(websocketpp::connection_hdl handle) {
    auto conn = d_server.get_con_from_hdl(handle);
    LOG_INFO << "Client connected: " << conn->get_origin();
}

void WSServer::onClose(websocketpp::connection_hdl handle) {
    auto conn = d_server.get_con_from_hdl(handle);
    LOG_INFO << "Client disconnected: " << conn->get_uri();
    if (d_closeCb) {
        d_closeCb(conn);
    }
}

} // namespace rtdb