// NAME          : directory.h
// DESCRIPTION   : Singly linked list to manage directories
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Directory: Node struct with directory info*/
typedef struct Directory {
    int wd;             // Inotify ID
    char *pathname;     // Directory name
    struct Directory *next;
} Directory;

/* Directories: List with all nodes*/
typedef struct DirectoryList {
    Directory *head;
    int size;
} DirectoryList;


// Directory operations
Directory* newDirectory(char *pathname);
void deleteDirectory(Directory *dir);

// DirectoryList operations
DirectoryList* newDirectoryList();
void deleteDirectoryList(DirectoryList *list);

int isEmpty(DirectoryList* list);
void insertAtFront(DirectoryList *list, Directory *dir);
void deleteAtFront(DirectoryList *list);

void updateWD(DirectoryList *list, int index, int wd);
void deleteByWD(DirectoryList *list, int wd);
Directory* findByIndex(DirectoryList *list, int index);


#endif /* DIRECTORY_H */
