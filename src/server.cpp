#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 1024

std::string formatIPAddress(const sockaddr_in& socketAddress) {
    int port = ntohs(socketAddress.sin_port);
    int addr = socketAddress.sin_addr.s_addr;
    std::stringstream ss;
    ss << addr << ":" << port;
    return ss.str(); 
}

// Marks a given socket file descriptor as non-blocking
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) return;
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
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

    setNonBlocking(sockfd);

    if (bind(sockfd, (const sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Error binding socket");
    }

    // extract the IP address
    std::cout << "Listening on: " << formatIPAddress(serverAddress) << std::endl;

    std::cout << "Waiting for clients..." << std::endl;

    // listen for up to 5 clients at a time
    listen(sockfd, 5);

    std::vector<pollfd> fds;
    fds.push_back({sockfd, POLLIN, 0});

    while (true) {
        if (poll(fds.data(), fds.size(), -1) < 0) {
            std::cerr << "Error polling" << std::endl;
            continue;
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) { // read event
                if (fds[i].fd == sockfd) { // on server socket (i.e. new connection)
                    sockaddr_in clientAddress;
                    socklen_t addrlen = sizeof(clientAddress);
                    int clientfd = accept(sockfd, (sockaddr*) &clientAddress, &addrlen);
                    if (clientfd >= 0) {
                        fds.push_back({clientfd, POLLIN, 0});
                        std::cout << "A new client connected!\n";
                    }
                }
                else { // Data from client
                    memset(&msg, 0, sizeof(msg)); // clear message buffer
                    int n = recv(fds[i].fd, &msg, sizeof(msg), 0);
                    if (n > 0) {
                        if (!strcmp(msg, "EXIT")) {
                            close(fds[i].fd);
                            fds.erase(fds.begin() + i);
                            std::cout << "Client disconected" << std::endl;
                            i--;
                        }
                        else if (!strcmp(msg, "PING")) {
                            strcpy(msg, "PONG");
                            send(fds[i].fd, msg, 5, 0);
                            continue;
                        }
                        std::string data = "ECHO: " + std::string(msg);
                        send(fds[i].fd, data.c_str(), data.length() + 1, 0);
                    }
                    else {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        std::cout << "Client disconected" << std::endl;
                        i--;
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}