// NAME          : directory.c
// DESCRIPTION   : Singly linked list to manage directories
// AUTHOR        : Madh93 (Miguel Hernandez)
// VERSION       : 0.1.0
// LICENSE       : GNU General Public License v3

#include "directory.h"


// Directory operations

Directory* newDirectory(char *pathname) {

    Directory *dir = (Directory*)malloc(sizeof(Directory));

    dir->wd = -1;
    dir->pathname = pathname;
    dir->next = NULL;

    return dir;
}

void deleteDirectory(Directory *dir) {
    free(dir);
}


// DirectoryList operations

DirectoryList* newDirectoryList() {
    DirectoryList *list = (DirectoryList*)malloc(sizeof(DirectoryList));
    list->size = 0;
}

void deleteDirectoryList(DirectoryList *list) {

    while (!isEmpty(list)) {
        deleteAtFront(list);
    }

    free(list);
}


int isEmpty(DirectoryList* list) {
    return list->head ? 0 : 1;
}


// Insertions & Deletions

void insertAtFront(DirectoryList *list, Directory *dir) {

    dir->next = list->head;
    list->head = dir;
    list->size++;
}

void deleteAtFront(DirectoryList *list) {

    if (isEmpty(list)) {
        return;
    }

    Directory *aux = list->head;
    list->head = list->head->next;
    deleteDirectory(aux);
    list->size--;
}


void updateWD(DirectoryList *list, int index, int wd) {

    Directory *dir = findByIndex(list, index);
    dir->wd = wd;
}

void deleteByWD(DirectoryList *list, int wd) {

    if (isEmpty(list)) {
        return;
    }

    Directory *aux = list->head;
    Directory *prev = NULL;

    while (aux->wd != wd) {
        prev = aux;
        aux = aux->next;
    }

    // At the front
    if (aux == list->head && aux->wd == wd) {
        list->head = aux->next;
        deleteDirectory(aux);
    } // At the end
    else if (!aux->next && aux->wd == wd) {
        prev->next = NULL;
        deleteDirectory(aux);
    } else {
        prev->next = aux->next;
        deleteDirectory(aux);
    }

    list->size--;
}


// Find

Directory* findByIndex(DirectoryList *list, int index) {

    if (isEmpty(list)) {
        return NULL;
    }

    Directory *aux = list->head;
    int n = 0;

    while (n < index && aux->next) {
        aux = aux->next;
        n++;
    }

    return n == index ? aux : NULL;
}

Directory* findByWD(DirectoryList *list, int wd) {

    if (isEmpty(list)) {
        return NULL;
    }

    Directory *aux = list->head;

    while (aux) {
        if (aux->wd == wd) {
            return aux;
        }
        aux = aux->next;
    }

    return NULL;
}

Directory* findByPathname(DirectoryList *list, char *pathname) {

    if (isEmpty(list)) {
        return NULL;
    }

    Directory *aux = list->head;

    while (aux) {
        if (strcmp(aux->pathname, pathname) == 0) {
            return aux;
        }
        aux = aux->next;
    }

    return NULL;
}


void showDirectories(DirectoryList *list) {

    Directory *aux = list->head;
    int i=0;
    while (aux) {
        printf("%d: %s\n", i, aux->pathname);
        i++;
        aux = aux->next;
    }
}
