#include "header.h"

File initFile(){
    File new_file;
    new_file.first = NULL;
    return new_file;
}

void put(File *file, Frame* data) {
    Element *new_elem = malloc(sizeof(*new_elem));
    if (file == NULL || new_elem == NULL) {
        exit(EXIT_FAILURE);
    }
    new_elem->data = data;
    new_elem->next = NULL;
    if (file->first != NULL) {
        Element *current = file->first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_elem;
    } else {
        file->first = new_elem;
    }
}

Frame* pull(File *file) {
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }
    Frame* to_pull;
    if (file->first != NULL) {
        Element *el = file->first;
        to_pull = el->data;
        file->first = el->next;
        free(el);
    }
    return to_pull;
}