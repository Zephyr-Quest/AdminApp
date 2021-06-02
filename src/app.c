#include "../headers/header.h"
#include <stdio.h>

// #define SOLVER
// #define GENERATOR

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
            printf("(%ld) %s by %s%s\n", i, maps[i]->name, maps[i]->author, maps[i]->solvable ? " (solvable)" : "");
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
            // Solve the map
            Stack actions = initStack();
            bool can_be_solved = solve(to_solve, &actions, false);
            puts(can_be_solved ? "It can be solved !" : "It can't be solved...");
            if(can_be_solved){
                puts("\nSolution :");
                Element* current = actions.first;
                if(current == NULL) puts("No actions is required to finish this level !");
                else {
                    size_t i = 0;
                    while(current != NULL){
                        Coord current_lever = current->data;
                        printf("Step %ld -> (%ld:%ld)\n", i, current_lever.x, current_lever.y);
                        current = current->next;
                        i++;
                    }
                }
                
                //puts("Trying to find a easier solution :");
                //Stack easy_actions = initStack();
                //searchEasySolution(to_solve, &easy_actions, countOfStack(&actions), true);

                // Update database
                // printf("Do you want to update the database ? (y/n) ");
                // scanf(" %c", &choice);
                // if(choice == 'y'){
                //     // Validate the map
                //     bool success = setCanBeDone(to_solve, &actions);
                //     if(success) puts("Everything is good !");
                //     else puts("Something went wrong...");
                // }
            }
        }

    #endif

    #ifdef GENERATOR
        Map* map2 = generateRandomMap();
        display(map2, false);
        
        Stack test = initStack();
        bool tmp = solve(map2, &test, false);
        Coord end_point, player;
        player.x = START_X; player.y = START_Y;
        end_point.x = END_X; end_point.y = END_Y;
        pathfinding(map2, player, end_point, true);
        if(tmp == true) puts("solvable");
        else puts("non solvable");
    #endif

    return 0;
}
