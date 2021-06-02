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
        printf("The map is named '%s' and was made by %s :\n", to_solve->name, to_solve->author);
        printf("\n");
        display(to_solve, true);

        // Solve it or not
        char choice;
        printf("Do you want to solve it ? (y/n) ");
        scanf(" %c", &choice);
        if(choice == 'y'){
            // Solve the map
            Stack final_sol, actions = initStack();
            bool can_be_solved = solve(to_solve, &actions, false);
            puts(can_be_solved ? "It can be solved !" : "It can't be solved...");
            if(can_be_solved){
                puts("\nSolution :");
                Element* current = actions.first;
                if(current == NULL) puts("No actions is required to succeed this level !");
                else {
                    size_t i = 1;
                    while(current != NULL){
                        Coord current_lever = current->data;
                        printf("Step %ld -> (%ld:%ld)\n", i, current_lever.x, current_lever.y);
                        current = current->next;
                        i++;
                    }
                }

                // Try to optimise the solution
                if(countOfStack(&actions) > 1){
                    puts("\nTrying to find an easier solution :");
                    Stack easy_actions = initStack();
                    bool is_easier_sol = searchEasySolution(to_solve, &easy_actions, countOfStack(&actions), false);
                    if(is_easier_sol && countOfStack(&easy_actions) < countOfStack(&actions)){
                        puts("\nEasier solution :");
                        Element* current = easy_actions.first;
                        size_t i = 1;
                        while(current != NULL){
                            Coord current_lever = current->data;
                            printf("Step %ld -> (%ld:%ld)\n", i, current_lever.x, current_lever.y);
                            current = current->next;
                            i++;
                        }
                        final_sol = easy_actions;
                    } else {
                        puts("There isn't an easier solution... (sorry)");
                        final_sol = actions;
                    }
                } else final_sol = actions;

                // Get the full path
                printf("\n");
                Coord player; player.x = START_X; player.y = START_Y;
                Stack path = initStack();
                moveTo(to_solve, &player, final_sol.first->data, &path, false);
                moveTo(to_solve, &player, final_sol.first->next->data, &path, false);
                current = path.first;
                size_t i = 1;
                char path_map[SIZE_MAP][SIZE_MAP];
                generateMapArray(path_map, to_solve);
                while(current != NULL){
                    Coord current_point = current->data;
                    path_map[current_point.y][current_point.x] = -i;
                    current = current->next;
                    i++;
                }
                printMapArray(path_map, false);

                // Update database
                printf("\nDo you want to update the database ? (y/n) ");
                scanf(" %c", &choice);
                if(choice == 'y'){
                    // Validate the map
                    bool success = setCanBeDone(to_solve, &final_sol);
                    if(success) puts("Everything is good !");
                    else puts("Something went wrong...");
                }
            }
        }

    #endif

    #ifdef GENERATOR

        char choice;
        Map* map2;
        bool tmp;
        Map* unsolvable_map;


        puts("\nHi ! Would you like to create a map ? (y/n)");
        scanf(" %c", &choice);
        if(choice == 'y')
        {
            Stack test = initStack();
            int watch_dogs = 0;

            do
            {
                map2 = generateRandomMap();
                tmp = solve(map2, &test, false);
                if(tmp == false) unsolvable_map = map2;
                watch_dogs++;
            } while(tmp == false && watch_dogs < 10);
            printf("A solvable map as been generated in %d try, do you want to show it ? (y/n)\n", watch_dogs);
            scanf(" %c", &choice);
            if(choice == 'y')
            {
                display(map2, false);
            }
            puts("Do you want to upload it ? (y/n)");
            scanf(" %c", &choice);
            char* map_name;
            if(choice == 'y')
            {
                puts("How do you want to name it ?");
                puts("The name of the card must not be longer than 250 characters and should not contain any non-alphanumeric characters.");
                scanf("%s", map_name);
                strcpy(map2->name, map_name);
                tmp = uploadNewMap(map2);
                if(tmp == true) puts("The map has been upload ! :happy_face:");
                else puts("The map hasn't been upload :sad_face:");
            }

            if(watch_dogs > 1)
            {
                puts("Map non faisable :");
                display(unsolvable_map, false);
                puts("Upload ? (y/n)");
                scanf(" %c", &choice);
                if(choice == 'y')
                {
                    puts("name : ");
                    scanf("%s", map_name);
                    strcpy(map2->name, map_name);
                    map_name = "Unsolvable map";
                    strcpy(map2->author, map_name);
                    tmp = uploadNewMap(unsolvable_map);
                    if(tmp == true) puts("The map has been upload ! :happy_face:");
                    else puts("The map hasn't been upload :sad_face:");
                }
            }
        }
        puts("Bye");
    #endif
    return 0;
}
