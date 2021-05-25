#include "header.h"

bool pathfinding(Map* base_map, Coord start, Coord end, bool verbose){
    // Search and print distances
    char distance_map[SIZE_MAP][SIZE_MAP];
    generateMapArray(distance_map, base_map);
    int distance = 1;
    Stack near_points = initStack();
    put(&near_points, start);

    while(near_points.first != NULL){
        Stack new_near_points = initStack();

        // Loop through the last near points file
        while(near_points.first != NULL){
            Coord current_point = pull(&near_points);

            //Find near points of the current point and print distances on them
            Coord* current_nears = getNearPoints(current_point);
            int i = 0;
            while(current_nears[i].x != SIZE_MAP && current_nears[i].y != SIZE_MAP && i < 4){
                Coord current_near_point = current_nears[i];
                char current_near_value = distance_map[current_near_point.y][current_near_point.x];
                if(canBeHover(current_near_value) && !(isCoordsEquals(current_near_point, start) || isCoordsEquals(current_near_point, end))){
                    distance_map[current_near_point.y][current_near_point.x] = distance;
                    put(&new_near_points, current_near_point);
                }
                i++;
            }
        }
        near_points.first = new_near_points.first;
        distance++;
    }
    if(verbose) printMapArray(distance_map, true);

    // Check if a path exists
    bool res = false;
    Coord* nears_endpoint = getNearPoints(end);
    int i = 0;
    while(isInMap(nears_endpoint[i]) && i < 4){
        Coord current_near = nears_endpoint[i];
        char current_near_value = distance_map[current_near.y][current_near.x];
        if(current_near_value != 0 && !isObstacle(current_near_value)){
            // A path exists
            res = true;
        }
        i++;
    }

    return res;
}

Coord* getNearPoints(Coord center){
    Coord* res = (Coord *) malloc(4 * sizeof(Coord));
    int i = 0;
    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            if(abs(x) != abs(y)){
                Coord corner;
                corner.x = center.x + x;
                corner.y = center.y + y;
                if(isInMap(corner)){
                    res[i] = corner;
                    i++;
                }
            }
        }
    }
    if(i < 4){
        Coord end; end.x = SIZE_MAP; end.y = SIZE_MAP;
        res[i] = end;
    }
    return res;
}

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
                    case 1 : destination[y][x] = 'u'; break;
                    // door
                    case 2: destination[y][x] = 'l'; break;
                    // wall
                    case 3: destination[y][x] = '&'; break;
                    // hole
                    case 4: destination[y][x] = '@'; break;
                    // torch
                    case 5: destination[y][x] = '*'; break;
                }
            } else destination[y][x] = 0;
        }
    }
}

void printMapArray(char map[SIZE_MAP][SIZE_MAP], bool show_zeros){
    for(size_t y = 0; y < SIZE_MAP; y++){
        for(size_t x = 0; x < SIZE_MAP; x++){
            char current_item = map[y][x];
            if(current_item == 'u' || current_item == '*' || isObstacle(current_item))
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

bool isObstacle(char to_check){
    return to_check == 'l' || to_check == '&' || to_check == '@';
}

bool canBeHover(char to_check){
    return to_check == 'u' || to_check == '*' || to_check == 0;
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

Frame** searchExits(Map* map, Coord player){
    Frame** blocking_doors = (Frame**) malloc(NB_DOOR_MAX * sizeof(Frame*));
    Frame** doors = getAllItemInMap(map, 2);
    size_t i = 0, k = 0;
    while(doors[i] != NULL){
        Coord door_coord; door_coord.x = doors[i]->x; door_coord.y = doors[i]->y;
        Coord* nears_doorpoints = getNearPoints(door_coord);

        size_t j = 0;
        bool is_blocking = false;
        while(isInMap(nears_doorpoints[j]) && j < 4){
            Coord current_near = nears_doorpoints[j];
            if(locateFrameByCoord(map, current_near, false) == NULL && pathfinding(map, player, current_near, false)) {
                is_blocking = !is_blocking;
            }
            j++;
        }

        if(is_blocking) {
            blocking_doors[k] = doors[i];
            k++;
        }
        i++;
    }

    return blocking_doors;
}

bool openDoor(Map* map, Frame* door, Coord* player){
    size_t i = 0;
    Frame* lever = NULL;
    while (door->usages[i] != NULL && lever == NULL) {
        lever = door->usages[i];
        Coord lever_coord; lever_coord.x = lever->x; lever_coord.y = lever->y;
        if(!pathfinding(map, *player, lever_coord, false)) lever = NULL;
    }
    if(lever == NULL) return false;
    printf("%d;%d\n", lever->x, lever->y);
    return true;
}