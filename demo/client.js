// direct client to the C++ TCP/IP socket server

const net = require("net");

const tcpClient = new net.Socket();
tcpClient.connect({ port: 8080, host: "localhost" }, () => {
    console.log('Connected to TCP server');
});

tcpClient.on('data', (data) => {
    console.log('Received: ' + data);
});

tcpClient.on('close', () => {
    console.log('Connection to TCP server closed');
});