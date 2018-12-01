#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void makeSocketPair (std::pair<int, int> &pair) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0 || client < 0) {
        return;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = 0;

    if (0 > bind(server, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) {
        close(server);
        close(client);
        return;
    }
    listen(server, 1);

    struct sockaddr_in clientaddr;
    socklen_t size = sizeof(clientaddr);
    if (0 > getsockname(server, (struct sockaddr *)&clientaddr, &size) || size != sizeof(clientaddr)) {
        close(server);
        close(client);
        return;
    }

    if (0 > connect(client, (struct sockaddr *)&clientaddr, sizeof(clientaddr))) {
        close(server);
        close(client);
        return;
    }

    int conn = accept(server, (struct sockaddr *)&serveraddr, &size);
    if (conn < 0 || size != sizeof(serveraddr)) {
        close(server);
        close(client);
        return;
    }

    //now conn <---> client!
    close(server);

    if (0 > getsockname(client, (struct sockaddr *)&clientaddr, &size)) {
        close(conn);
        close(client);
        return;
    }

    pair.first = client;
    pair.second = conn;
}

int main () {
    std::pair<int, int> sockpair({-1, -1});
    makeSocketPair(sockpair);

    if (sockpair.first == -1 || sockpair.second == -1) {
        return 0;
    }

    //begin
    if (fork()) {
        //parent
        while (1) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::string req = "parent: I'm your father";
            char buf[4096] = {0};
            send(sockpair.first, req.c_str(), req.length(), 0);
            recv(sockpair.first, buf, 4096, 0);
            std::cout << buf << std::endl;
        }
    } else {
        //child
        while (1) {
            std::string req = "child: I'm your child";
            char buf[4096] = {0};
            recv(sockpair.second, buf, 4096, 0);
            std::cout << buf << std::endl;
            send(sockpair.second, req.c_str(), req.length(), 0);
        }
    }

    return 0;
}
