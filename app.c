#include "header.h"

int main() {
    srand(time(NULL));

    // Get all saved maps
    printf("All maps :\n");
    size_t nb_maps;
    Map** maps = getAllMaps(&nb_maps);
    if(maps == NULL || nb_maps == 0){
        puts("The database is empty...");
        return 0;
    }
    for (size_t i = 0; i < nb_maps; i++) {
        printf(" - %s by %s\n", maps[i]->name, maps[i]->author);
    }
    printf("\n");

    // Get the map named 'test'
    puts("The map named 'map1' :");
    Map* map1 = getMapByName("map2");
    if (map1 == NULL) puts("The map named 'map1' doesn't exist...");
    else {
        printf("%s by %s is saved in the memory\n", map1->name, map1->author);
        printf("\n");
        display(map1, false);
        Stack actions = initStack();
        printf("%d\n", solve(map1, &actions));
        while(actions.first != NULL){
            printFrame(pullFrame(&actions, map1));
        }
    }

    Map* map2 = generateRandomMap(2);
    
    //addFrameInMap(map2, createFrame(0, 6, 2));
    display(map2, false);
    placeDoor(map2);
    display(map2, false);

    return 0;
}
