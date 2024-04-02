#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>
#include <set>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 1024
#define NUM_FUNCTIONS 7

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

// str must be zero-terminated.
// splits string by spaces. treats quotationed values as whole arguments.
int splitArgs(const char* str, std::vector<std::string>& args) {
    size_t len = strlen(str);
    size_t lastTokenIndex = 0;
    bool inQuote = false;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\"') {
            inQuote = !inQuote;
            if (!inQuote) {
                // This terminated a quote, so add this argument
                if (i + 1 >= len || str[i + 1] == ' ') {
                    args.push_back(std::string(str + lastTokenIndex + 1, i - lastTokenIndex));
                    lastTokenIndex = i + 2;
                    i++;
                }
                else {
                    return -1; // Cannot have a non-space character after a terminating quote
                }
            }
        }
        else if (!inQuote && str[i] == ' ') {
            args.push_back(std::string(str + lastTokenIndex, i - lastTokenIndex));
            lastTokenIndex = i + 1;
        }
    }
    if (len > lastTokenIndex) {
        args.push_back(std::string(str + lastTokenIndex));
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: port" << std::endl;
        return -1;
    }

    std::map<std::string, std::string> store;
    std::map<std::string, std::set<int>> subscribers;

    std::pair<std::string, std::function<int(std::vector<std::string>&, int, char*, size_t&)>> functions[NUM_FUNCTIONS] = {
        {"PING", [](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            strcpy(buffer, "PONG");
            length = 5;
            return 0;
        }},
        {"SET", [&store, &subscribers](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            if (args.size() != 3) {
                return -1;
            }
            std::string key = args[1];
            std::string value = args[2];
            store[key] = value;
            std::string msg = "+OK " + key + ":" + value; 
            strcpy(buffer, msg.c_str());
            length = msg.size() + 1;
            for (int subscriber : subscribers[key]) {
                // std::cout << "Sending: " << buffer << " to subscriber: " << subscriber << std::endl;
                if (sockfd != subscriber) {
                    send(subscriber, buffer, length, 0);
                }
            }
            return 0;
        }},
        {"QUERY", [&store](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            if (args.size() != 2) {
                return -1;
            }
            std::string key = args[1];
            std::string value = store[key];
            std::string msg = "+OK " + key + ":" + value;
            strcpy(buffer, msg.c_str());
            length = msg.size() + 1;
            return 0;
        }},
        {"DELETE", [](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            strcpy(buffer, "unimplemented");
            length = 14;
            return 1;
        }},
        {"SUBSCRIBE", [&store, &subscribers](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            if (args.size() != 2) {
                return -1;
            }
            std::string key = args[1];
            if (store.find(key) == store.end()) {
                store[key] = "";
            }
            subscribers[key].insert(sockfd);
            std::string value = store[key];
            std::string msg = "+OK " + key + ":" + value;
            strcpy(buffer, msg.c_str());
            length = msg.size() + 1;
            return 0;
        }},
        {"UNSUBSCRIBE", [](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            strcpy(buffer, "unimplemented");
            length = 14;
            return 1;
        }},
        {"UNSUBSCRIBE_ALL", [](std::vector<std::string>& args, int sockfd, char* buffer, size_t& length) -> int {
            strcpy(buffer, "unimplemented");
            length = 14;
            return 1;
        }},
    };

    int port = atoi(argv[1]);

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
                            continue;
                        }

                        std::vector<std::string> args;
                        if (splitArgs(msg, args) < 0) {
                            std::cerr << "Error parsing message: " << msg << std::endl;
                            send(fds[i].fd, "-ERROR error parsing command", 29, 0);
                            continue;
                        }

                        if (args.size() == 0) {
                            std::cerr << "Error: Got empty message" << std::endl;
                            send(fds[i].fd, "-ERROR empty command", 22, 0);
                            continue;
                        }

                        bool foundFunction = false;

                        for (size_t j = 0; j < NUM_FUNCTIONS; j++) {
                            if (functions[j].first == args[0]) {
                                char buffer[MAX_BUFFER_SIZE];
                                memset(buffer, 0, sizeof(buffer));
                                size_t len;
                                if (functions[j].second(args, fds[i].fd, buffer, len) < 0) {
                                    std::cerr << "Error occurred" << std::endl;
                                    send(fds[i].fd, "-ERROR unknown error occurred", 30, 0);
                                }
                                else {
                                    std::cout << "Sending: " << buffer << " len(" << len << ")" << std::endl;
                                    send(fds[i].fd, buffer, len, 0);
                                }
                                foundFunction = true;
                                break;
                            }
                        }

                        if (foundFunction) {
                            continue;
                        }

                        std::string data = "-ERROR unrecognized command: " + std::string(msg);
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