// NAME          : main.c
// DESCRIPTION   : Parse input arguments
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"


int findOption(int argc, char *argv[], char *option) {
    int i = 0;
    while (++i < argc)
        if (!strcmp(argv[i], option) || argv[i][1] == option[2])
            return ++i;
    return 0;
}


int findArgumentDir(int argc, char *argv[]) {
    int i = 0;
    while (++i < argc)
        if (argv[i][0] == '/')
            return i;
    printf("No directories\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {

    // Show built-in commands
    if (argc < 2) {
        printf("No directories\n");
        exit(EXIT_FAILURE);
    } else if ((!strcmp(argv[1], "-?")) || (!strcmp(argv[1], "--help"))) {
        showHelp();
    } else if ((!strcmp(argv[1], "-v")) || (!strcmp(argv[1], "--version"))) {
        showVersion();
    }

    // Get and parse input data
    int pos;

    // Host
    pos = findOption(argc, argv, (char*)"--host");
    char *host = pos ? argv[pos] : (char*)"localhost";

    // Port
    pos = findOption(argc, argv, (char*)"--port");
    int port = pos ? atoi(argv[pos]) : 12345;

    // Directories
    pos = findArgumentDir(argc, argv);
    DirectoryList *d = parseDirectories(pos, argc, argv);

    // Start daemon
    demonize(d, host, port);

    return 0;
}
