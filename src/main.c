// NAME          : main.c
// DESCRIPTION   : Parse input argumens
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"

int main(int argc, char *argv[]) {

    // Parse arguments
    if (argc > 1) {
        if ((strcmp(argv[1], "-d") == 0) || (strcmp(argv[1], "--directories") == 0)) {
            Directories *d = parseDirectories(argc, argv);
            printf("%d\n", demonize(d));
        } else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            showHelp();
        } else if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
            showVersion();
        } else {
            printf("Unknown option '%s'\nTry: %s --help\n", argv[1], APP);
        }
    } else {
        printf("No arguments\n");
    }

    return 0;
}
