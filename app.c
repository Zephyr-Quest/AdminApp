#include "header.h"

// #define SOLVER
// #define GENERATOR

int main() {
    srand(time(NULL));    

    // Get all saved maps
    // printf("All maps :\n");
    // size_t nb_maps;
    // Map** maps = getAllMaps(&nb_maps);
    // if(maps == NULL || nb_maps == 0){
    //     puts("The database is empty...");
    //     return 0;
    // }
    // for (size_t i = 0; i < nb_maps; i++) {
    //     printf(" - %s by %s\n", maps[i]->name, maps[i]->author);
    // }
    // printf("\n");

    // Get the map named 'test'
    #ifdef SOLVER
        const char* map_name = "map2";
        printf("The map named '%s' :\n", map_name);
        Map* to_solve = getMapByName(map_name);
        if (to_solve == NULL) printf("The map named '%s' doesn't exist...\n", map_name);
        else {
            printf("%s by %s is saved in the memory\n", to_solve->name, to_solve->author);
            printf("\n");
            display(to_solve, false);
            Frame* lever = locateFrame(to_solve, 1, 0, true);
            printFrame(lever->usages[1]);
            // char choice = 'y';
            char choice;
            printf("Do you want to solve it ? (y/n) ");
            scanf("%c", &choice);
            if(choice == 'y'){
                Stack actions = initStack();
                bool can_be_solved = solve(to_solve, &actions, true);
                puts(can_be_solved ? "It can be solved !" : "It can't be solved...");
                while(actions.first != NULL && can_be_solved){
                    printFrame(pullFrame(&actions, to_solve));
                }
            }
        }
    #endif

    #ifdef GENERATOR
        Map* map2 = generateRandomMap(2);
    
        //addFrameInMap(map2, createFrame(0, 6, 2));
        display(map2, false);
        placeDoor(map2);
        display(map2, false);
    #endif

    
    return 0;
}
