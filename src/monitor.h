// NAME          : monitor.h
// DESCRIPTION   : Monitorize directories through inotify
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#ifndef MONITOR_H
#define MONITOR_H

// Inotify
#include "watchman.h"
#include <sys/inotify.h>
#include <limits.h>

// Utilities
#define MAX_EVENTS 128
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + NAME_MAX + 1))


int initMonitor();
void closeMonitor(int fd, int *watching, int size);
int addDirectory(int fd, Directory *dir);
void removeDirectory(int fd, int wd);
void showEvent(struct inotify_event *event);
void readEvents(int fd);

int monitorize(Directories *d);

#endif /* MONITOR_H */
