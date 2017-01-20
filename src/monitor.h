// NAME          : monitor.h
// DESCRIPTION   : Monitorize directories through inotify
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#ifndef MONITOR_H
#define MONITOR_H

// Inotify
#include "watchman.h"
#include "connection.h"
#include <signal.h>
#include <sys/inotify.h>
#include <limits.h>

// Utilities
#define MAX_EVENTS 128
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + NAME_MAX + 1))

// Monitor status
static int monitoring = 0;

// Signal handling
void signalHandler(int signo);

// Inotify utils
int initMonitor();
void closeMonitor(int fd, DirectoryList *d);

void addWatch(int fd, Directory *dir);
void removeWatch(int fd, DirectoryList *d, Directory *dir);

void showEvent(int fd, int sockfd, DirectoryList *d, struct inotify_event *event);
void readEvents(int fd, int sockfd, DirectoryList *d);

// Inotify big loop
int monitorize(DirectoryList *d, char *host, int port);

#endif /* MONITOR_H */
