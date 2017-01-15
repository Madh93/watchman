// NAME          : watchman.h
// DESCRIPTION   : Demonize process and start all jobs
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#ifndef WATCHMAN_H
#define WATCHMAN_H

/**
*** INCLUDES
**/

// To demonize
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>


/**
*** CONSTANTS
**/

// General
#define APP "watchman"
#define VERSION "0.1.0"
#define PATH "/usr/local/bin"


/**
*** DEFINITION FUNCTIONS
**/

/* Directory: Struct with directory info */
typedef struct Directory {
    char *path;     // Location path
} Directory;

/* Directories: Struct with directories info */
typedef struct Directories {
    int size;           // Nº directories
    Directory *dirs;    // Directories
} Directories;


/**
*** DEFINITION FUNCTIONS
**/

// General
int demonize(Directories *d);
Directories* parseDirectories(int size, char *args[]);

// Other
void showHelp();
void showVersion();

#endif /* WATCHMAN_H */
