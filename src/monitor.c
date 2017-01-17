// NAME          : monitor.c
// DESCRIPTION   : Monitorize directories through inotify
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "monitor.h"


int initMonitor() {

    int fd = inotify_init();

    if (fd < 0) {
        syslog(LOG_ERR, "Failed to start inotify");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Started monitor");

    return fd;
}

void closeMonitor(int fd, int *watching, int size) {

    for (int i=0; i<size; i++) {
        removeDirectory(fd, watching[i]);
    }

    if (close(fd) < 0) {
        syslog(LOG_ERR, "Failed to close inotify");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Monitor closed");

}

int addDirectory(int fd, Directory *dir) {

    int wd = inotify_add_watch(fd, dir->pathname, IN_ALL_EVENTS);

    if (wd < 0) {
        syslog(LOG_ERR, "Failed to add watch for '%s'", dir->pathname);
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Added watch '%s'", dir->pathname);

    return wd;
}

void removeDirectory(int fd, int wd) {

    if (inotify_rm_watch(fd, wd) < 0) {
        syslog(LOG_ERR, "Failed to remove watch");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Removed watch '%d'", wd);
}

void showEvent(struct inotify_event *event) {

    if (event->mask & IN_CREATE)
        syslog(LOG_INFO, "Created '%s'", (char *)event->name);
}

void readEvents(int fd) {

    struct inotify_event *event;
    char buffer[BUF_LEN];
    ssize_t count = read(fd, buffer, BUF_LEN);

    if (count < 0) {
        syslog(LOG_ERR, "Failed to read events");
        exit(EXIT_FAILURE);
    }

    // Show every read event
    for (char *e = buffer; e < buffer+count; ) {
        event = (struct inotify_event *)e;
        showEvent(event);
        e += EVENT_SIZE + event->len;
    }
}


int monitorize(Directories *d){

    // Initialize inotify
    int fd = initMonitor();

    // List of watching directories
    int *watching = malloc(d->size * sizeof(int));

    // Add directories to watch
    for (int i = 0; i < d->size; i++) {
        watching[i] = addDirectory(fd, &d->dirs[i]);
    }

    // Read and show events
    while (1) {
        readEvents(fd);
    }

    // Close and free
    closeMonitor(fd, watching, d->size);
    free(watching);

    return 0;
}