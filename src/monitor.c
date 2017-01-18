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

void closeMonitor(int fd, DirectoryList *d) {

    int size = d->size;
    for (int i=0; i<size; i++) {
        removeDirectory(fd, findByIndex(d, i));
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
        syslog(LOG_ERR, "Failed to add watch '%s", dir->pathname);
        exit(EXIT_FAILURE);
    }

    dir->wd = wd;
    syslog(LOG_INFO, "Added watch '%s' (wd: %d)", dir->pathname, dir->wd);
}

void removeDirectory(int fd, Directory *dir) {

    if (inotify_rm_watch(fd, dir->wd) < 0) {
        syslog(LOG_ERR, "Failed to remove watch");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Removed watch '%s' (wd: %d)", dir->pathname, dir->wd);
    dir->wd = -1;
}

void showEvent(struct inotify_event *event) {

    int wd = event->wd;
    char *name = NULL;
    char *type = NULL;

    // Get filename
    if (event->len > 0) {
        name = event->name;
    }

    // Check filetype
    if (event->mask & IN_ISDIR) {
        type = (char*)"directory";
    } else {
        type = (char*)"file";
    }

    // Check event
    switch (event->mask & IN_ALL_EVENTS) {
        case IN_ACCESS: // File was accessed (read)
            syslog(LOG_INFO, "IN_ACCESS: %s '%s' was accessed (wd: %d)",
                                type, name, wd);
            break;

        case IN_ATTRIB: // File metadata changed
            syslog(LOG_INFO, "IN_ATTRIB: %s '%s' metadata changed (wd: %d)",
                                type, name, wd);
            break;

        case IN_CLOSE_WRITE: // File opened for writing was closed
            syslog(LOG_INFO, "IN_CLOSE_WRITE: %s '%s' opened for writing was closed (wd: %d)",
                                type, name, wd);
            break;

        case IN_CLOSE_NOWRITE: // File opened read-only was closed
            syslog(LOG_INFO, "IN_CLOSE_NOWRITE: %s '%s' opened read-only was closed (wd: %d)",
                                type, name, wd);
            break;

        case IN_CREATE: // File/directory created
            syslog(LOG_INFO, "IN_CREATE: %s '%s' created (wd: %d)",
                                type, name, wd);
            break;

        case IN_DELETE: // File/directory deleted
            syslog(LOG_INFO, "IN_DELETE: %s '%s' deleted (wd: %d)",
                                type, name, wd);
            break;

        case IN_DELETE_SELF: // File/directory was itself deleted
            syslog(LOG_INFO, "IN_DELETE_SELF: %s '%s' was itself deleted (wd: %d)",
                                type, name, wd);
            break;

        case IN_MODIFY: // File was modified
            syslog(LOG_INFO, "IN_MODIFY: %s '%s' was modified (wd: %d)",
                                type, name, wd);
            break;

        case IN_MOVE_SELF: // File/directory was itself moved
            syslog(LOG_INFO, "IN_MOVE_SELF: %s '%s' was itself moved (wd: %d)",
                                type, name, wd);
            break;

        case IN_MOVED_FROM: // File moved out of directory
            syslog(LOG_INFO, "IN_MOVED_FROM: %s '%s' moved out of directory (wd: %d)",
                                type, name, wd);
            break;

        case IN_MOVED_TO: // File moved into directory
            syslog(LOG_INFO, "IN_MOVED_INTO: %s '%s' moved into directory (wd: %d)",
                                type, name, wd);
            break;

        case IN_OPEN: // File was opened
            syslog(LOG_INFO, "IN_OPEN: %s '%s' was opened (wd: %d)",
                                type, name, wd);
            break;
    }
}

void readEvents(int fd) {

    struct inotify_event *event;
    char buffer[BUF_LEN];
    ssize_t count = read(fd, buffer, BUF_LEN);

    if (count < 0) {
        if (monitoring) {
            syslog(LOG_ERR, "Failed to read events");
            exit(EXIT_FAILURE);
        }
        return;
    }

    // Show every read event
    for (char *e = buffer; e < buffer+count; ) {
        event = (struct inotify_event *)e;
        showEvent(event);
        e += EVENT_SIZE + event->len;
    }
}


int monitorize(DirectoryList *d){

    // Register signal SIGTERM and signal handler
    signal(SIGTERM, signalHandler);

    // Initialize inotify
    int fd = initMonitor();

    // Add directories to watch
    int size = d->size;
    for (int i=0; i<size; i++) {
        addDirectory(fd, findByIndex(d, i));
    }

    // Read and show events
    while (monitoring) {
        readEvents(fd);
    }

    // Close and free
    closeMonitor(fd, d);

    return 0;
}
