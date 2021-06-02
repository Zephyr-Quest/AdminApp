#include "../headers/header.h"
#include <stddef.h>

bool moveTo(Map* base_map, Coord* player, Coord destination, Stack* keep_path, bool verbose){
    // Search and print distances
    char distance_map[SIZE_MAP][SIZE_MAP];
    generateMapArray(distance_map, base_map);
    int distance = 1;
    Stack near_points = initStack();
    put(&near_points, *player);
    bool end_reached = false;

    // Front propagation
    while(near_points.first != NULL){
        Stack new_near_points = initStack();

        // Loop through the last near points file
        while(near_points.first != NULL && !end_reached){
            Coord current_point = pull(&near_points);

            //Find near points of the current point and print distances on them
            Coord* current_nears = getNearPoints(current_point);
            int i = 0;
            while(current_nears[i].x != SIZE_MAP && current_nears[i].y != SIZE_MAP && i < 4){
                Coord current_near_point = current_nears[i];
                char current_near_value = distance_map[current_near_point.y][current_near_point.x];
                if(isCoordsEquals(current_near_point, destination)) end_reached = true;
                else if(canBeHover(current_near_value) && !isCoordsEquals(current_near_point, *player)){
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

    // Back propagation
    if(keep_path != NULL && end_reached){
        Coord current_point = destination;
        char d = 0;
        bool start_reached = false;
        const size_t path_size = countOfStack(keep_path);
        while(!start_reached){
            Coord* near_points = getNearPoints(current_point);
            int i = 0;
            bool path_chose = false;
            while(isInMap(near_points[i]) && i < 4 && !path_chose){
                Coord current_near = near_points[i];
                char current_near_value = distance_map[current_near.y][current_near.x];
                if(isCoordsEquals(current_near, *player)) start_reached = true;
                else if(!canBeHover(current_near_value) && !isObstacle(current_near_value)){
                    if(current_near_value <= d || d == 0){
                        current_point = current_near;
                        d = current_near_value;
                        path_chose = true;
                        putAtRank(keep_path, current_point, path_size);
                    }
                }
                i++;
            }
        }
    }

    if(end_reached) *player = destination;

    return end_reached;
}

bool pathfinding(Map* base_map, Coord start, Coord end, bool verbose){
    return moveTo(base_map, &start, end, NULL, verbose);
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

List* searchExits(Map* map, Coord player){
    // Get all of closed doors
    List* blocking_doors = initList();
    List* closed_doors = initList();
    ListElement* current = map->items->first;
    while (current != NULL) {
        if (current->data->id == ID_DOOR && !current->data->state)
            appendAtList(closed_doors, current->data);
        current = current->next;
    }

    // Choose which doors are blocking doors
    current = closed_doors->first;
    while (current != NULL) {
        Coord door_coord; door_coord.x = current->data->x; door_coord.y = current->data->y;
        Coord* nears_doorpoints = getNearPoints(door_coord);

        size_t j = 0;
        bool is_blocking = false;
        while(isInMap(nears_doorpoints[j]) && j < 4){
            Coord current_near = nears_doorpoints[j];
            Frame* current_near_frame = locateFrameByCoord(map, current_near, false);
            bool can_be_reached = pathfinding(map, player, current_near, false);
            if(current_near_frame == NULL) {
                if(can_be_reached) is_blocking = !is_blocking;
            } else if(current_near_frame->id == ID_DOOR && current_near_frame->state) {
                if(can_be_reached) is_blocking = !is_blocking;
            }
            j++;
        }

        if(is_blocking){
            appendAtList(blocking_doors, current->data);
        }
        current = current->next;
    }

    return blocking_doors;
}

Frame* getDoorLever(Map* map, Frame* door, Coord player, Stack* actions){
    Frame* lever = NULL;
    Coord lever_coord;
    ListElement* current = door->usages->first;
    while (current != NULL && lever == NULL) {
        lever = current->data;
        lever_coord = lever->pos;
        if(!pathfinding(map, player, lever_coord, false) || stackContainsFrame(actions, lever)) lever = NULL;
        current = current->next;
    }
    if(lever == NULL) return NULL;
    else return locateFrameByCoord(map, lever->pos, false);
}

bool useLever(Map* map, Frame* lever, Coord* player, bool verbose){
    if(verbose) {
        puts("Open the lever :");
        printFrame(lever);
    }
    Coord lever_coord;
    lever_coord.x = lever->x;
    lever_coord.y = lever->y;

    // Move the player to the lever
    if(!moveTo(map, player, lever_coord, NULL, false)) return false;

    // Open or close doors
    ListElement* current = lever->usages->first;
    while (current != NULL) {
        Frame* current_door = current->data;
        current_door = locateFrameByCoord(map, current_door->pos, true);
        if(current_door != NULL) {
            current_door->state = !current_door->state;
        }
        current = current->next;
    }

    // Remove the lever
    removeFromList(map->items, lever, false);

    if(verbose) display(map, false);
    return true;
}

bool solve(Map* base_map, Stack* interactions, bool verbose){
    // Setup the map
    Map* map = copyMap(base_map);
    Coord end_point, player;
    player.x = START_X; player.y = START_Y;
    end_point.x = END_X; end_point.y = END_Y;

    bool res = true;    // true means resolvable, false means unresolvable
    size_t nb_actions = 0;
    if(verbose) printf("\n");

    while(!pathfinding(map, player, end_point, false) && res){
        List* blocking_doors = searchExits(map, player);
        bool can_exit = false;
        ListElement* current = blocking_doors->first;
        while (current != NULL && !can_exit) {
            Frame* lever = getDoorLever(map, current->data, player, interactions);
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
            current = current->next;
        }
        if(!can_exit) res = false;
    }

    return res;
}

List* pathThroughDoors(Map* base_map, Coord start, bool verbose){
    // Set up the map
    Map* map = copyMap(base_map);
    Coord end_point; end_point.x = END_X; end_point.y = END_Y;
    if(pathfinding(map, start, end_point, false)) return NULL;

    // Front propagation
    char d = 1;
    List* blocking_doors = searchExits(map, start);
    List* super_doors = initList();
    while (blocking_doors->first != NULL) {
        // Loop through blocking_doors
        ListElement* current = blocking_doors->first;
        while (current != NULL) {
            Frame* blocking_door = current->data;
            Frame* to_store = createFrameByCoord(blocking_door->pos, 5 + d);
            appendAtList(super_doors, to_store);
            blocking_door->state = true;
            d++;
            current = current->next;
        }
        blocking_doors = searchExits(map, start);
    }
    if(verbose) {
        puts("Front propagation :");
        display(map, false);
        printFrameList(super_doors);
    }

    // Back propagation
    map = copyMap(base_map);
    List* best_path = initList();
    blocking_doors = searchExits(map, end_point);
    while (!pathfinding(map, end_point, start, false) && blocking_doors->first != NULL) {
        ListElement* current = blocking_doors->first;
        Frame* chosen_door = NULL;
        // Loop through blocking_doors
        while (current != NULL) {
            Frame* blocking_door = current->data;
            blocking_door = getDoorByCoord(super_doors, blocking_door->pos);
            if(blocking_door != NULL){
                if(chosen_door == NULL || blocking_door->id < chosen_door->id)
                    chosen_door = locateFrameByCoord(map, blocking_door->pos, false);
            }
            current = current->next;
        }
        if(chosen_door != NULL) {
            chosen_door->state = true;
            appendAtList(best_path, chosen_door);
        }
        blocking_doors = searchExits(map, end_point);
    }
    if(verbose) {
        puts("Back propagation :");
        display(map, false);
        printFrameList(best_path);
    }

    return best_path;
}

Frame* getDoorByCoord(List* doors, Coord pos){
    if (doors == NULL) return NULL;
    ListElement* current_door = doors->first;
    while (current_door != NULL && !isCoordsEquals(current_door->data->pos, pos))
        current_door = current_door->next;
    if(current_door == NULL) return NULL;
    return current_door->data;
}

bool searchEasySolution(Map* base_map, Stack* actions, size_t max_actions, bool verbose){
    // Setup the map
    Map* map = copyMap(base_map);
    closeAllDoors(map);
    Coord end_point, player;
    player.x = START_X; player.y = START_Y;
    end_point.x = END_X; end_point.y = END_Y;

    size_t nb_actions = 0;
    if(verbose) {
        printf("\n");
        display(map, false);
    }

    List* blocking_doors = pathThroughDoors(map, player, false);
    Frame* blocking_door = getFirstClosedDoor(map, blocking_doors);
    while(blocking_door != NULL) {
        blocking_door = locateFrameByCoord(map, blocking_door->pos, false);
        Frame* lever = getDoorLever(map, blocking_door, player, actions);
        if(lever != NULL) locateFrameByCoord(map, lever->pos, false);
        bool can_exit = false;
        if(lever != NULL){
            // Try to open the door
            can_exit = useLever(map, lever, &player, verbose);
            if(can_exit && nb_actions < max_actions) {
                // Save the current lever
                putFrame(actions, lever);
                nb_actions++;
            }
        }
        if(lever == NULL || !can_exit){
            // Replace the door by a wall
            if(verbose){
                puts("Block this door :");
                printFrame(blocking_door);
                printf("\n");
            }
            blocking_door->id = 3;
        }
        blocking_doors = pathThroughDoors(map, player, false);
        blocking_door = getFirstClosedDoor(map, blocking_doors);
    }

    return pathfinding(map, player, end_point, false);
}

Frame* getFirstClosedDoor(Map* map, List* path){
    if(path == NULL) return NULL;
    ListElement* current = path->first;
    Frame* res = NULL;
    while (current != NULL) {
        res = current->data;
        res = locateFrameByCoord(map, res->pos, false);
        if(!res->state) break;
        current = current->next;
    }
    return res;
}
