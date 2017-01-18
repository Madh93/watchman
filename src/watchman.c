// NAME          : watchman.c
// DESCRIPTION   : Demonize process and start all jobs
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"
#include "monitor.h"


int demonize(DirectoryList *d) {

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

    // Do something interesting
    monitorize(d);

    // Free memory
    deleteDirectoryList(d);

    // Close logs
    syslog(LOG_NOTICE, "Finished");
    closelog();

    return 0;
}

DirectoryList* parseDirectories(int size, char *args[]) {

    if (size < 3) {
        printf("No directories\n");
        exit(EXIT_FAILURE);
    }

    // Get directories data
    DirectoryList *d = newDirectoryList();

    for (int i=2; i<size; i++) {
        Directory *dir = newDirectory(args[i]);
        insertAtFront(d, dir);
        findSubdirectories(d, dir);
    }

    return d;
}

void findSubdirectories(DirectoryList *d, Directory *dir) {

    // About: http://stackoverflow.com/a/30877241/2453259

    struct dirent *entry;
    DIR *dp = opendir(dir->pathname);

    if (!dp) {
        printf("Error opening directory '%s'\n",dir->pathname);
        exit(EXIT_FAILURE);
    }

    // Read entries in directory
    while ((entry = readdir(dp))) {

        struct stat buf;
        char *subdir = appendPath(dir->pathname, entry->d_name);

        // Check if is a directory
        if (stat(subdir, &buf) >= 0) {
            if (S_ISDIR(buf.st_mode) && (entry->d_name[0] != '.')) {
                // Insertion and recursive call
                if (!findByPathname(d, subdir)) {
                    Directory *newdir = newDirectory(subdir);
                    insertAtFront(d, newdir);
                    findSubdirectories(d, newdir);
                }
            }
        }
    }

    closedir(dp);
}


char* appendPath(char *path, char *name) {

    // Prepare subdirectory path
    int len = strlen(path);
    char* dir = (char*)malloc(len + NAME_MAX + 2);
    strcpy(dir, path);

    // Append separator
    if(dir[len-1] != '/') {
      dir[len] = '/';
      len++;
    }

    // Append name
    strcpy(&dir[len], name);

    return dir;
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
