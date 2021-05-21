#include "header.h"

int main() {
    Coord start_point, end_point;
    start_point.x = START_X; start_point.y = START_Y;
    end_point.x = END_X; end_point.y = END_Y;

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
        char map1_test[SIZE_MAP][SIZE_MAP];
        generateMapArray(map1_test, map1);
        printMapArray(map1_test, false);
        // Coord test_point;
        // test_point.x = 2; test_point.y = 14;
        printf("%d", pathfinding(map1_test, start_point, end_point, true));
    }
    
    //Map* map2 = createMap("Eng", "moi" );
    // trumpWall(map2, nbRand(0,2));
    // placeDoor(map2);
    // display(map2);

    printf("\n");
    return 0;
}
