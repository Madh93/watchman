// NAME          : client.h
// DESCRIPTION   : Client connection management
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "connection.h"

// Client utils

int initConnection(char *host, int port) {

    // Create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        syslog(LOG_ERR, "Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Server info
    struct hostent *server = gethostbyname(host);

    if (!server) {
        syslog(LOG_ERR, "Failed to get server information '%s'", host);
        exit(EXIT_FAILURE);
    }

    // Add information
    struct sockaddr_in info;

    memset(&info, 0, sizeof(info));
    info.sin_family = AF_INET;
    bcopy(
        (char*)server->h_addr,
        (char*)&info.sin_addr.s_addr,
        server->h_length
    );
    info.sin_port = htons(port);

    // Start connection
    if (connect(fd, (struct sockaddr*)&info, sizeof(info)) < 0) {
        syslog(LOG_ERR, "Failed to connect to server '%s:%d'", host, port);
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Started connection");

    sendMessage(fd, (char*)"Hello world from Watchman client!");
    listenServer(fd);

    return fd;
}


void closeConnection(int fd) {

    if (close(fd) < 0) {
        syslog(LOG_ERR, "Failed to close connection");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Connection closed");
}


void listenServer(int fd) {

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    ssize_t count = read(fd, buffer, sizeof(buffer));

    if (count < 1) {
        syslog(LOG_ERR, "Failed to read message from server");
    } else if (count == 0) {
        syslog(LOG_INFO, "Server has disconnected");
    } else {
        syslog(LOG_INFO, "Server says: %s", buffer);
    }
}


void sendMessage(int fd, char *message) {

    if (write(fd, message, strlen(message)) < 0) {
        syslog(LOG_ERR, "Failed to send message to server");
        exit(EXIT_FAILURE);
    }
}
