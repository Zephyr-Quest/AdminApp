#include "../headers/header.h"

bool isInMap(Coord point){
    return point.x < SIZE_MAP && point.y < SIZE_MAP;
}

Frame* locateFrameByCoord(Map* map, Coord coord, bool verbose){
    return locateFrame(map, coord.x, coord.y, verbose);
}

void generateMapArray(char destination[SIZE_MAP][SIZE_MAP], Map* source){
    for(size_t y = 0; y < SIZE_MAP; y++){
        for(size_t x = 0; x < SIZE_MAP; x++){
            Frame* current_frame = locateFrame(source, x, y, false);
            if(current_frame != NULL){
                switch (current_frame->id) {
                    // button
                    case 1 : destination[y][x] = BUTTON; break;
                    // door
                    case 2:
                        if (current_frame->state) destination[y][x] = 0;
                        else destination[y][x] = DOOR;
                        break;
                    // wall
                    case 3: destination[y][x] = WALL; break;
                    // hole
                    case 4: destination[y][x] = HOLE; break;
                    // torch
                    case 5: destination[y][x] = TORCH; break;
                }
            } else destination[y][x] = 0;
        }
    }
}

void printMapArray(char map[SIZE_MAP][SIZE_MAP], bool show_zeros){
    for(size_t y = 0; y < SIZE_MAP; y++){
        for(size_t x = 0; x < SIZE_MAP; x++){
            char current_item = map[y][x];
            if(current_item == BUTTON || current_item == TORCH || isObstacle(current_item))
                printf("%2c", current_item);
            else {
                if(current_item == 0 && !show_zeros) printf("  ");
                else printf("%2d", current_item);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printCoord(Coord to_print){
    printf("(%ld;%ld)\n", to_print.x, to_print.y);
}

bool isObstacle(char to_check){
    return to_check == DOOR || to_check == WALL || to_check == HOLE || to_check == BUTTON;
}

bool canBeHover(char to_check){
    return to_check == TORCH || to_check == 0;
}

bool isCoordsEquals(Coord c1, Coord c2){
    return c1.x == c2.x && c1.y == c2.y;
}

void mapCopy(char destination[SIZE_MAP][SIZE_MAP], char source[SIZE_MAP][SIZE_MAP]){
    for(size_t y = 0; y < SIZE_MAP; y++){
        for(size_t x = 0; x < SIZE_MAP; x++){
            destination[y][x] = source[y][x];
        }
    }
}

Map* createMap(char name[], char author[]) {
    Map* tmp = malloc(sizeof(Map));
    if (tmp != NULL) {
        strcpy(tmp->name, name);
        strcpy(tmp->author, author);
        tmp->items = initList();
    } else free(tmp);
    return tmp;
}

Frame* createFrame(int posX, int posY, int id_object) {
    Frame* tmp = malloc(sizeof(Frame));
    if (tmp != NULL) {
        tmp->x = posX;
        tmp->y = posY;
        tmp->id = id_object;
        tmp->usages = initList();
        tmp->state = false;
    } else free(tmp);
    return tmp;
}

List* getAllItemInMap(Map* map, int id_object) {
    List* items = initList();
    ListElement* current = map->items->first;
    while (current != NULL) {
        if(current->data->id == id_object){
            appendAtList(items, current->data);
        }
        current = current->next;
    }
    return items;
}

void addFrameInMap(Map* map, Frame* frame) {
    if(compareFrame(frame, locateFrame(map, frame->x, frame->y, false))) return;
    appendAtList(map->items, frame);
}

Frame* locateFrame(Map* map, int posx, int posy, bool verbose) {
    if (map == NULL || posx < 0 || posx > SIZE_MAP-1 || posy < 0 || posy > SIZE_MAP-1 ) {
        if (verbose) puts("La frame n'a pas été retrouvé : Carte ou coordoonées incorrecte"); // TODO: Supprimer ce message
        return NULL;
    }
    Coord to_find; to_find.x = posx; to_find.y = posy;
    ListElement* current = map->items->first;
    Frame* found = NULL;
    while (current != NULL && found == NULL) {
        Coord current_coord;
        current_coord.x = current->data->x;
        current_coord.y = current->data->y;
        if(isCoordsEquals(to_find, current_coord)){
            found = current->data;
        }
        current = current->next;
    }
    if (found == NULL && verbose) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
    return found;
}

void display(Map* map, bool show_zeros) {
    char map_array[SIZE_MAP][SIZE_MAP];
    generateMapArray(map_array, map);
    printMapArray(map_array, show_zeros);
}

bool compareFrame(Frame* frame1, Frame* frame2) {
    return !(frame1 == NULL || frame2==NULL || frame1->id != frame2->id ||
    frame1->x != frame2->x ||frame1->y != frame2->y);
}

void printFrame(Frame* frame) {
    if(frame == NULL) puts("The frame is NULL");
    else printf("id: %d, x: %d, y: %d\n", frame->id, frame->x, frame->y);
}