// NAME          : main.c
// DESCRIPTION   : Parse input arguments
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"


int findOption(int argc, char *argv[], char *option) {
    int i = 1;
    while (i < argc) {
        if ((strcmp(argv[i], option) == 0) || argv[i][1] == option[2]) {
            i++;
            break;
        }
        i++;
    }
    printf("%d\n",i);
    return i < argc ? i : -1;
}


int findOptionDir(int argc, char *argv[]) {
    int pos = 1;
    while (pos < argc) {
        if (argv[pos][0] == '/') {
            break;
        }
        pos++;
    }

    if (pos >= argc) {
        printf("No directories\n");
        exit(EXIT_FAILURE);
    }

    return pos;
}


int main(int argc, char *argv[]) {

    // Defaults
    char *host;
    int port;
    int pos = -1;

    if (argc <= 1) {
        printf("No directories\n");
        exit(EXIT_FAILURE);
    }

    // Show built-in commands
    if ((strcmp(argv[1], "-?") == 0) || (strcmp(argv[1], "--help") == 0)) {
        showHelp();
    } else if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
        showVersion();
    }

    // Get data

    // Host
    pos = findOption(argc, argv, (char*)"--host");
    if (pos > 0) {
        host = argv[pos];
    } else {
        host = (char*)"localhost";
    }

    // Port
    pos = findOption(argc, argv, (char*)"--port");
    if (pos > 0) {
        port = atoi(argv[pos]);
    } else {
        port = 12345;
    }

    // Directories
    pos = findOptionDir(argc, argv);
    DirectoryList *d = parseDirectories(pos, argc, argv);

    // Start daemon
    demonize(d, host, port);

    return 0;
}
