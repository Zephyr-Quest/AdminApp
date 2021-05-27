#include "../headers/header.h"

Stack initStack(){
    Stack new_stack;
    new_stack.first = NULL;
    return new_stack;
}

void put(Stack *stack, Coord data) {
    Element *new_elem = malloc(sizeof(*new_elem));
    if (stack == NULL || new_elem == NULL) {
        exit(EXIT_FAILURE);
    }
    new_elem->data = data;
    new_elem->next = NULL;
    if (stack->first != NULL) {
        Element *current = stack->first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_elem;
    } else {
        stack->first = new_elem;
    }
}

Coord pull(Stack *stack) {
    if (stack == NULL) {
        exit(EXIT_FAILURE);
    }
    Coord to_pull;
    if (stack->first != NULL) {
        Element *el = stack->first;
        to_pull = el->data;
        stack->first = el->next;
        free(el);
    }
    return to_pull;
}

void putFrame(Stack *stack, Frame* data) {
    Coord tmp;
    tmp.x = data->x;
    tmp.y = data->y;
    put(stack, tmp);
}

Frame* pullFrame(Stack *stack, Map* map){
    return locateFrameByCoord(map,pull(stack), false);
}

bool stackContains(Stack* stack, Coord data){
    if(stack == NULL || !isInMap(data)) return false;
    bool res = false;
    Element* el = stack->first;
    while(el != NULL && !res){
        if(isCoordsEquals(el->data, data)) res = true;
        el = el->next;
    }
    return res;
}

bool stackContainsFrame(Stack* stack, Frame* frame){
    Coord data; data.x = frame->x; data.y = frame->y;
    return stackContains(stack, data);
}