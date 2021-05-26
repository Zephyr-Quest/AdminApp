#include "header.h"

bool moveTo(Map* base_map, Coord* player, Coord destination, bool verbose){
    // Search and print distances
    char distance_map[SIZE_MAP][SIZE_MAP];
    generateMapArray(distance_map, base_map);
    int distance = 1;
    Stack near_points = initStack();
    put(&near_points, *player);

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
                if(canBeHover(current_near_value) && !(isCoordsEquals(current_near_point, *player) || isCoordsEquals(current_near_point, destination))){
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
    Coord* nears_endpoint = getNearPoints(destination);
    int i = 0;
    while(isInMap(nears_endpoint[i]) && i < 4){
        Coord current_near = nears_endpoint[i];
        char current_near_value = distance_map[current_near.y][current_near.x];
        if(!canBeHover(current_near_value) && !isObstacle(current_near_value)){
            // A path exists
            res = true;
            player->x = current_near.x;
            player->y = current_near.y;
        }
        i++;
    }

    return res;
}

bool pathfinding(Map* base_map, Coord start, Coord end, bool verbose){
    return moveTo(base_map, &start, end, verbose);
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

Frame** searchExits(Map* map, Coord player){
    // Get all of closed doors
    Frame** blocking_doors = (Frame**) malloc(NB_DOOR_MAX * sizeof(Frame*));
    Frame** closed_doors = (Frame**) malloc(NB_DOOR_MAX * sizeof(Frame*));
    size_t j = 0;
    for (int i = 0; i < map->nb_items; i++){
        if (map->items[i]->id == 2 && !map->items[i]->state) {
            closed_doors[j] = map->items[i];
            j++;
        }
    }

    // Choose which doors are blocking doors
    size_t i = 0, k = 0;
    while(closed_doors[i] != NULL){
        Coord door_coord; door_coord.x = closed_doors[i]->x; door_coord.y = closed_doors[i]->y;
        Coord* nears_doorpoints = getNearPoints(door_coord);

        j = 0;
        bool is_blocking = false;
        while(isInMap(nears_doorpoints[j]) && j < 4){
            Coord current_near = nears_doorpoints[j];
            if(locateFrameByCoord(map, current_near, false) == NULL && pathfinding(map, player, current_near, false)) {
                is_blocking = !is_blocking;
            }
            j++;
        }

        if(is_blocking) {
            blocking_doors[k] = closed_doors[i];
            k++;
        }
        i++;
    }

    return blocking_doors;
}

Frame* getDoorLever(Map* map, Frame* door, Coord player, Stack* actions){
    size_t i = 0;
    Frame* lever = NULL;
    Coord lever_coord;
    while (door->usages[i] != NULL && lever == NULL) {
        lever = door->usages[i];
        lever_coord.x = lever->x; lever_coord.y = lever->y;
        if(!pathfinding(map, player, lever_coord, false) || stackContainsFrame(actions, lever)) lever = NULL;
        i++;
    }
    return lever;
}

bool useLever(Map* map, Frame* lever, Coord* player, bool verbose){
    Coord lever_coord;
    lever_coord.x = lever->x;
    lever_coord.y = lever->y;

    // Move the player to the lever
    if(!moveTo(map, player, lever_coord, false)) return false;

    // Open or close doors
    printFrame(lever);
    for(size_t i = 0; i < _countofFrames(lever->usages); i++){
        Frame* current_door = lever->usages[i];
        current_door = locateFrame(map, current_door->x, current_door->y, false);
        printf("%ld\n", i);
        printFrame(current_door);
        if(current_door != NULL)
            current_door->state = !current_door->state;
    }
    if(verbose) display(map, false);

    return true;
}

bool solve(Map* map, Stack* interactions, bool verbose){
    // Start and end point
    Coord end_point, player;
    player.x = START_X; player.y = START_Y;
    end_point.x = END_X; end_point.y = END_Y;

    bool res = true;    // true means resolvable, false means unresolvable
    size_t nb_actions = 0;
    
    while(!pathfinding(map, player, end_point, false) && res){
        Frame** blocking_doors = searchExits(map, player);
        size_t i = 0;
        bool can_exit = false;
        while(blocking_doors[i] != NULL && !can_exit){
            Frame* lever = getDoorLever(map, blocking_doors[i], player, interactions);
            if(lever != NULL) lever = locateFrame(map, lever->x, lever->y, false);
            if(lever != NULL){
                can_exit = useLever(map, lever, &player, verbose);
                if(can_exit) {
                    if(nb_actions >= MAX_ACTIONS) res = false;
                    else {
                        putFrame(interactions, lever);
                        nb_actions++;
                    }
                }
            }
            i++;
        }
        if(!can_exit) res = false;
    }

    return res;
}