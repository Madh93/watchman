// NAME          : watchman.c
// DESCRIPTION   : Demonize process and start all jobs
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"
#include "monitor.h"


int demonize(Directories *d) {

    pid_t pid;

    // Fork of the parent process
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask for permissions
    umask(0);

    // Open logs
    openlog(APP, LOG_NOWAIT | LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Starting...");

    // Create a new SID for the child process
    if (setsid() < 0) {
        syslog(LOG_ERR, "Failed to start a new sesion");
        exit(EXIT_FAILURE);
    }

    // Change current directory
    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "Failed to change of directory");
        exit(EXIT_FAILURE);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    syslog(LOG_NOTICE, "Started");

    // Do something
    // sleep(10);
    monitorize(d);

    // Close logs
    syslog(LOG_NOTICE, "Finished");
    closelog();

    return 0;
}

Directories* parseDirectories(int size, char *args[]) {

    if (size < 3) {
        printf("No directories\n");
        exit(EXIT_FAILURE);
    }

    // Get directories data
    Directories *d = malloc(sizeof(Directories));

    d->size = size - 2;
    d->dirs = malloc(d->size * sizeof(Directory));

    for (int i=0; i<d->size; i++) {
        d->dirs[i].name = args[i+2];
    }

    return d;
}


void showHelp() {
    printf("%s: Simple implementation of inotify\n", APP);
    printf("\nUsage: %s [options]\n", APP);
    printf("\nOptions:\n");
    printf("  -d, --directories   Directories to monitorize\n");
    printf("  -h, --help          Show this help\n");
    printf("  -v, --version       Show version\n");
}

void showVersion() {
    printf("%s %s\n", APP, VERSION);
}
