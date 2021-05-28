#include "../headers/header.h"

//#define SOLVER
#define GENERATOR

int main() {
    srand(time(NULL));

    #ifdef SOLVER
        // Get all saved maps
        printf("All maps :\n");
        size_t nb_maps;
        Map** maps = getAllMaps(&nb_maps);
        if(maps == NULL || nb_maps == 0){
            puts("The database is empty...");
            return 0;
        }
        for (size_t i = 0; i < nb_maps; i++) {
            printf("(%ld) %s by %s\n", i, maps[i]->name, maps[i]->author);
        }
        printf("\n");

        // Get the wanted map
        printf("Which map do you want to get ? (0-%ld) ", nb_maps-1);
        size_t map_id;
        scanf(" %ld", &map_id);
        if(map_id >= nb_maps){
            puts("The given id is not correct...");
            return 0;
        }

        // Print the chosen map
        Map* to_solve = maps[map_id];
        printf("The map is named '%s' and made by %s :\n", to_solve->name, to_solve->author);
        printf("\n");
        display(to_solve, false);
        
        // Solve it or not
        char choice;
        printf("Do you want to solve it ? (y/n) ");
        scanf(" %c", &choice);
        if(choice == 'y'){
            Stack actions = initStack();
            bool can_be_solved = solve(to_solve, &actions, false);
            puts(can_be_solved ? "It can be solved !" : "It can't be solved...");
            if(can_be_solved){
                puts("\nSolution :");
                size_t i = 0;
                while(actions.first != NULL && can_be_solved){
                    Frame* current_lever = pullFrame(&actions, to_solve);
                    printf("Step %ld -> (%d:%d)\n", i, current_lever->x, current_lever->y);
                    i++;
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
