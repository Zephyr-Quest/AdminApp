#include "../headers/header.h"
#include <curl/curl.h>

bool isInMap(Coord point){
    return point.x < SIZE_MAP && point.y < SIZE_MAP;
}

Frame* locateFrameByCoord(Map* map, Coord coord, bool verbose){
     if (map == NULL || coord.x > SIZE_MAP-1 || coord.y > SIZE_MAP-1 ) {
        if (verbose) puts("La frame n'a pas été retrouvé : Carte ou coordoonées incorrecte"); // TODO: Supprimer ce message
        return NULL;
    }
    ListElement* current = map->items->first;
    Frame* found = NULL;
    while (current != NULL && found == NULL) {
        Coord current_coord;
        current_coord.x = current->data->x;
        current_coord.y = current->data->y;
        if(isCoordsEquals(coord, current_coord)){
            found = current->data;
        }
        current = current->next;
    }
    if (found == NULL && verbose) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
    return found;
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
                    default:
                        if(current_frame->id > 5)
                            destination[y][x] = -(current_frame->id - 5);
                        break;
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
                printf("%3c", current_item);
            else {
                if(current_item == 0 && !show_zeros) printf("   ");
                else if (current_item < 0) printf("%3d", abs(current_item));
                else printf("%3d", current_item);
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
    return to_check == TORCH || to_check <= 0;
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
        tmp->solvable = false;
    } else free(tmp);
    return tmp;
}

Coord createCoord(int pos_x, int pos_y) {
    Coord tmp;
    tmp.x = pos_x;
    tmp.y = pos_y;
    return tmp;
}

Frame* createFrame(int posX, int posY, int id_object) {
    Frame* tmp = malloc(sizeof(Frame));
    if (tmp != NULL) {
        tmp->pos.x = posX;
        tmp->pos.y = posY;
        tmp->x = posX;  // depreciated
        tmp->y = posY;  // depreciated
        tmp->id = id_object;
        tmp->usages = initList();
        tmp->state = false;
    } else free(tmp);
    return tmp;
}

Frame * createFrameByCoord(Coord pos, int id_object) {
    Frame* tmp = malloc(sizeof(Frame));
    if (tmp != NULL) {
        tmp->x = pos.x;  // depreciated
        tmp->y = pos.y;  // depreciated
        tmp->pos = pos;
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
    (frame1->x != frame2->x ||frame1->y != frame2->y) || (frame1->pos.x != frame2->pos.x || frame1->pos.y != frame2->pos.y));
}

void printFrame(Frame* frame) {
    if(frame == NULL) puts("The frame is NULL");
    else printf("id: %d, x: %ld, y: %ld\n", frame->id, frame->pos.x, frame->pos.y);
}

Map* copyMap(Map* map) {
    Map* tmp_map = createMap(map->name, map->author);
    ListElement* current = map->items->first;
    while (current != NULL) {
        Frame* current_frame = current->data;
        Frame* tmp_frame = createFrameByCoord(current_frame->pos, current_frame->id);
        ListElement* current_usage = current_frame->usages->first;
        while(current_usage != NULL){
            Frame* tmp_usage = createFrameByCoord(current_usage->data->pos, current_usage->data->id);
            appendAtList(tmp_frame->usages, tmp_usage);
            current_usage = current_usage->next;
        }
        tmp_frame->state = current_frame->state;
        appendAtList(tmp_map->items, tmp_frame);
        current = current->next;
    }
    return tmp_map;
}

void closeAllDoors(Map* map){
    List* doors = getAllItemInMap(map, 2);
    ListElement* current = doors->first;
    while (current != NULL) {
        current->data->state = false;
        current = current->next;
    }
}