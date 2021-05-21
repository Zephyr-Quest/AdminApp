#include "header.h"

bool pathfinding(char map[SIZE_MAP][SIZE_MAP], Coord start, Coord end, bool verbose){
    // Search and print distances
    char distance_map[SIZE_MAP][SIZE_MAP];
    mapCopy(distance_map, map);
    int distance = 1;
    File near_points = initFile();
    put(&near_points, start);

    while(near_points.first != NULL){
        File new_near_points = initFile();

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

Frame* searchAnExit(char map[SIZE_MAP][SIZE_MAP], Coord entry){
    Coord* current_nears = getNearPoints(entry);
    int i = 0;
    while(isInMap(current_nears[i]) && i < 4){
        Coord current_near = current_nears[i];
        char current_near_value = map[current_near.y][current_near.x];
        // if(current_near_value.)
        i++;
    }
    
    return NULL;
}

Coord followWallWithDirection(Coord start, int direction_x, int direction_y){
    Coord destination;
    return destination;
}