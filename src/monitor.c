// NAME          : monitor.c
// DESCRIPTION   : Monitorize directories through inotify
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "monitor.h"


void signalHandler(int signo) {
    if (signo == SIGTERM) {
        syslog(LOG_INFO, "Received SIGTERM");
        monitoring = 0;
    } else {
        syslog(LOG_ERR, "Received unknown signal");
        exit(EXIT_FAILURE);
    }
}


int initMonitor() {

    int fd = inotify_init();

    if (fd < 0) {
        syslog(LOG_ERR, "Failed to start inotify");
        exit(EXIT_FAILURE);
    }

    monitoring = 1;
    syslog(LOG_INFO, "Started monitor");

    return fd;
}

void closeMonitor(int fd, Directories *d) {

    for (int i=0; i<d->size; i++) {
        removeDirectory(fd, &d->dirs[i]);
    }

    if (close(fd) < 0) {
        syslog(LOG_ERR, "Failed to close inotify");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Monitor closed");
}

void addDirectory(int fd, Directory *dir) {

    int wd = inotify_add_watch(fd, dir->pathname, IN_ALL_EVENTS);

    if (wd < 0) {
        syslog(LOG_ERR, "Failed to add watch for '%s'", dir->pathname);
        exit(EXIT_FAILURE);
    }

    dir->wd = wd;
    syslog(LOG_INFO, "Added watch '%s'", dir->pathname);
}

void removeDirectory(int fd, Directory *dir) {

    if (inotify_rm_watch(fd, dir->wd) < 0) {
        syslog(LOG_ERR, "Failed to remove watch");
        exit(EXIT_FAILURE);
    }

    dir->wd = -1;
    syslog(LOG_INFO, "Removed watch '%s'", dir->pathname);
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
        // exit(EXIT_FAILURE);
        syslog(LOG_ERR, "Failed to read events");
        return;
    }

    // Show every read event
    for (char *e = buffer; e < buffer+count; ) {
        event = (struct inotify_event *)e;
        showEvent(event);
        e += EVENT_SIZE + event->len;
    }
}


int monitorize(Directories *d){

    // Register signal SIGTERM and signal handler
    signal(SIGTERM, signalHandler);

    // Initialize inotify
    int fd = initMonitor();

    // Add directories to watch
    for (int i = 0; i < d->size; i++) {
        addDirectory(fd, &d->dirs[i]);
    }

    // Read and show events
    while (monitoring) {
        readEvents(fd);
    }

    // Close and free
    closeMonitor(fd, d);

    return 0;
}
