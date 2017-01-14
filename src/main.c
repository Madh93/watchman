// NAME          : main.c
// DESCRIPTION   : Parse input argumens
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "watchman.h"

int main(int argc, char *argv[]) {

    // DEBUG
    printf("Debug:\n");
    printf("%d\n", argc);

    for (int i=0; i<argc; i++)
        printf("%s | ", argv[i]);
    printf("\n");

    // Parse arguments
    if (argc > 1) {
        if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            showHelp();
        } else if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
            showVersion();
        } else {
            printf("%d\n",demonize());
        }
    } else {
        printf("No arguments\n");
    }

    return 0;
}
