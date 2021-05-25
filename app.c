#include "header.h"

int main() {
    srand(time(NULL));
    Coord start_point;
    start_point.x = START_X; start_point.y = START_Y;
    // end_point.x = END_X; end_point.y = END_Y;

    // Get all saved maps
    printf("All maps :\n");
    size_t nb_maps;
    Map** maps = getAllMaps(&nb_maps);
    if(maps == NULL || nb_maps == 0){
        printf("The database is empty...");
        return 0;
    }
    for (size_t i = 0; i < nb_maps; i++) {
        printf(" - %s by %s\n", maps[i]->name, maps[i]->author);
    }
    printf("\n");

    // Get the map named 'test'
    printf("The map named 'map1' :\n");
    Map* map1 = getMapByName("map1");
    if (map1 == NULL) printf("The map named 'map1' doesn't exist...");
    else {
        printf("%s by %s is saved in the memory\n", map1->name, map1->author);
        printf("\n");
        display(map1, false);
        Frame** blocking_doors = searchExits(map1, start_point);
        Frame* blocking_door = blocking_doors[0];
        if(blocking_door != NULL){
            if(blocking_door->usages[0] == NULL) printf("test");
            printf("%d\n", openDoor(map1, blocking_door, &start_point));
        }
    }
    
    // Map* map2 = createMap("Eng", "moi" );
    Map* map2 = generateRandomMap(2);
    
    addFrameInMap(map2, createFrame(0, 6, 2));
    display(map2, true);
    placeDoor(map2);
    display(map2, false);

    return 0;
}
