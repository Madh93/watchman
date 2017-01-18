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

    while (d->size > 0) {
        removeWatch(fd, d, findByIndex(d,0));
    }

    if (close(fd) < 0) {
        syslog(LOG_ERR, "Failed to close inotify");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Monitor closed");
}

void addWatch(int fd, Directory *dir) {

    int wd = inotify_add_watch(fd, dir->pathname, IN_ALL_EVENTS);

    if (wd < 0) {
        syslog(LOG_ERR, "Failed to add watch '%s' (wd: %d)", dir->pathname, dir->wd);
        exit(EXIT_FAILURE);
    }

    dir->wd = wd;
    syslog(LOG_INFO, "Added watch '%s' (wd: %d)", dir->pathname, dir->wd);
}

void removeWatch(int fd, DirectoryList *d, Directory *dir) {

    if (inotify_rm_watch(fd, dir->wd) < 0) {
        syslog(LOG_ERR, "Failed to remove watch '%s' (wd: %d)", dir->pathname, dir->wd);
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Removed watch '%s' (wd: %d)", dir->pathname, dir->wd);
    // dir->wd = -1;
    deleteByWD(d, dir->wd);
}

void showEvent(int fd, DirectoryList* d, struct inotify_event *event) {

    int wd = event->wd;
    char *path = NULL;
    char *name = NULL;
    char *type = NULL;

    // Get filename
    if (event->len > 0) {
        name = event->name;
    }

    // Get pathname
    Directory* dir = findByWD(d, wd);
    if (dir) {
        path = (char*)dir->pathname;
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
            syslog(LOG_INFO, "IN_ACCESS: %s '%s' was accessed in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_ATTRIB: // File metadata changed
            syslog(LOG_INFO, "IN_ATTRIB: %s '%s' metadata changed in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_CLOSE_WRITE: // File opened for writing was closed
            syslog(LOG_INFO, "IN_CLOSE_WRITE: %s '%s' opened for writing was closed in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_CLOSE_NOWRITE: // File opened read-only was closed
            syslog(LOG_INFO, "IN_CLOSE_NOWRITE: %s '%s' opened read-only was closed in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_CREATE: // File/directory created
            syslog(LOG_INFO, "IN_CREATE: %s '%s' created in '%s' (wd: %d)",
                                type, name, path, wd);

            // Add to watch new directory
            if (type == "directory") {
                Directory* newdir = newDirectory(appendPath(path, name));
                insertAtFront(d, newdir);
                addWatch(fd, newdir);
            }
            break;

        case IN_DELETE: // File/directory deleted
            syslog(LOG_INFO, "IN_DELETE: %s '%s' deleted in '%s' (wd: %d)",
                                type, name, path, wd);

            // Remove watch directory
            if (type == "directory") {
                Directory* deldir = findByPathname(d, appendPath(path, name));
                if (deldir) {
                    removeWatch(fd, d, deldir);
                }
            }
            break;

        case IN_DELETE_SELF: // File/directory was itself deleted
            syslog(LOG_INFO, "IN_DELETE_SELF: %s '%s' was itself deleted in '%s' (wd: %d)",
                                type, name, path, wd);

            // Remove watch directory
            if (type == "directory") {
                Directory* deldir = findByPathname(d, appendPath(path, name));
                if (deldir) {
                    removeWatch(fd, d, deldir);
                }
            }
            break;

        case IN_MODIFY: // File was modified
            syslog(LOG_INFO, "IN_MODIFY: %s '%s' was modified in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_MOVE_SELF: // File/directory was itself moved
            syslog(LOG_INFO, "IN_MOVE_SELF: %s '%s' was itself moved in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_MOVED_FROM: // File moved out of directory
            syslog(LOG_INFO, "IN_MOVED_FROM: %s '%s' moved out of directory in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_MOVED_TO: // File moved into directory
            syslog(LOG_INFO, "IN_MOVED_INTO: %s '%s' moved into directory in '%s' (wd: %d)",
                                type, name, path, wd);
            break;

        case IN_OPEN: // File was opened
            syslog(LOG_INFO, "IN_OPEN: %s '%s' was opened in '%s' (wd: %d)",
                                type, name, path, wd);
            break;
    }
}

void readEvents(int fd, DirectoryList *d) {

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
        showEvent(fd, d, event);
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
        addWatch(fd, findByIndex(d, i));
    }

    // Read and show events
    while (monitoring) {
        readEvents(fd, d);
    }

    // Close and free
    closeMonitor(fd, d);

    return 0;
}
