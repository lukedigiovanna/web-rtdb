// direct client to the C++ TCP/IP socket server

const net = require("net");
const prompt = require('prompt-sync')();


const tcpClient = new net.Socket();
tcpClient.connect({ port: 8081, host: "localhost" }, () => {
    console.log('Connected to TCP server');
    tcpClient.write("PING");
});

tcpClient.on('data', (data) => {
    console.log(`${data}`);
    const message = prompt("> ");
    tcpClient.write(message);
});

tcpClient.on('close', () => {
    console.log('Connection to TCP server closed');
});
