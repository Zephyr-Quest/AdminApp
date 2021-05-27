#include "header.h"

List* initList(){
    List *list = malloc(sizeof(*list));
    if (list == NULL) exit(EXIT_FAILURE);
    list->first = NULL;
    return list;
}

void appendAtList(List *list, Frame* data) {
    ListElement *new_el = malloc(sizeof(*new_el));
    if (list == NULL || new_el == NULL) exit(EXIT_FAILURE);
    new_el->data = data;
    new_el->next = list->first;
    list->first = new_el;
}

ListElement* getElementAtRank(List* list, size_t rank){
    if (list == NULL) exit(EXIT_FAILURE);
    ListElement* current = list->first;
    size_t i = 0;
    while (current != NULL && i < rank) {
        current = current->next;
        i++;
    }
    return current;
}

Frame* getFrameInList(List* list, size_t rank){
    ListElement* res = getElementAtRank(list, rank);
    if(res == NULL) return NULL;
    else return res->data;
}

void removeFromList(List *list, Frame* frame, bool verbose) {
    if (list == NULL) exit(EXIT_FAILURE);
    ListElement *to_remove = list->first;
    bool match = false;
    size_t i = 0;
    // Search the frame
    while (to_remove != NULL && !match) {
        if(compareFrame(to_remove->data, frame)) {
            match = true;
            if(i == 0){
                // Remove first element
                list->first = to_remove->next;
            } else {
                // Remove an other element
                ListElement* previous = getElementAtRank(list, i - 1);
                if(previous != NULL) {
                    previous->next = to_remove->next;
                } else {
                    if(verbose) puts("[removeFromList] The previous element can't be found...");
                }
            }
        } else {
            to_remove = to_remove->next;
            i++;
        }
    }
    if(!match && verbose) puts("[removeFromList] Nothing was removed...");
}