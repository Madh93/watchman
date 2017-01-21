// NAME          : client.h
// DESCRIPTION   : Client connection management
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#ifndef CONNECTION_H
#define CONNECTION_H

// Client socket connection
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>


// Client utils
int initConnection(char *host, int port);
void closeConnection(int fd);

void listenServer(int fd);
void sendMessage(int fd, char *message);

#endif /* CONNECTION_H */
