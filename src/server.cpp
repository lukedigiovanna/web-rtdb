#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

std::string formatIPAddress(const sockaddr_in& socketAddress) {
    int port = ntohs(socketAddress.sin_port);
    int addr = socketAddress.sin_addr.s_addr;
    std::stringstream ss;
    ss << addr << ":" << port;
    return ss.str(); 
}

int main(int argc, char* args[]) {
    if (argc != 2) {
        std::cout << "Usage: port" << std::endl;
        return -1;
    }

    int port = atoi(args[1]);

    char msg[MAX_BUFFER_SIZE];
    sockaddr_in serverAddress;
    int sockfd;

    memset((void*) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; // ipv4
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("Error creating socket");
    }

    if (bind(sockfd, (const sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Error binding socket");
    }

    // extract the IP address
    std::cout << "Listening on: " << formatIPAddress(serverAddress) << std::endl;

    std::cout << "Waiting for clients..." << std::endl;

    // listen for up to 5 clients at a time
    listen(sockfd, 5);

    sockaddr_in clientAddress;
    socklen_t sockSize = sizeof(clientAddress);

    int clientSocket;
    if ((clientSocket = accept(sockfd, (sockaddr*) &clientAddress, &sockSize)) < 0) {
        throw std::runtime_error("Error accepting request from client");
    }

    std::cout << "Connected a client" << std::endl;

    int n;
    while (true) {
        std::cout << "Awaiting client response" << std::endl;
        memset(&msg, 0, sizeof(msg)); // clear message buffer
        n = recv(clientSocket, (char*) &msg, sizeof(msg), 0);
        std::cout << "Client: " << msg;
        std::string data;
        send(clientSocket, data.c_str(), data.length() + 1, 0);
    }

    close(sockfd);
    close(clientSocket);

    return 0;
}