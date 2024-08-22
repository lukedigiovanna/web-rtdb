const WebSocket = require("ws");
const net = require("net");

const wsServer = new WebSocket.Server({port: 8090});

wsServer.on('connection', (ws) => {
    // create a tcp client for this connection
    const tcpClient = new net.Socket();
    tcpClient.connect({ port: 8080, host: "localhost" }, () => {
        console.log('Connected to TCP server');
    });

    ws.on('message', function incoming(message) {
        console.log('received: %s', message);
        tcpClient.write(message); // Relay from WebSocket to TCP
    });

    tcpClient.on('data', (data) => {
        console.log('Received: ' + data);
        ws.send(data); // Relay from TCP to WebSocket
    });
    
    tcpClient.on('close', () => {
        console.log('Connection to TCP server closed');
        ws.close();
    });

    ws.on('close', () => {
        console.log('WebSocket client disconnected');
        tcpClient.end();
    });
});