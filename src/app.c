#include "../headers/header.h"
#include <stdio.h>

// #define SOLVER
// #define GENERATOR

int main() {
    srand(time(NULL));

    char choice_app;
    puts("\nWelcome to Zephyr Quest Admin App.");
    puts("\nWe use ASCII to print map in the terminal :");
    puts("A lever -> u");
    puts("A door -> l");
    puts("A wall -> &");
    puts("A hole -> @");
    puts("A torch -> *");
    puts("\nWhat do you want to do ?");
    puts("(1) - Generator");
    puts("(2) - Solver");
    puts("Choose 1 - 2 to open app. Press CTRL + C (^C) for exit the app.");
    scanf(" %c", &choice_app);
    while(choice_app != '1' && choice_app != '2') {
        puts("I'm sorry, I do not understood what do you want to open.");
        puts("(1) - Generator");
        puts("(2) - Solver");
        puts("Choose 1 - 2 to open app. Press CTRL + C (^C) for exit the app.");
        scanf(" %c", &choice_app);
    }
    if(choice_app == '2'){
        puts("Loading Solver");
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
        display(to_solve, false);

        // Solve it or not
        char choice;
        printf("Do you want to solve it ? (y/n) ");
        scanf(" %c", &choice);
        if(choice == 'y'){
            // Solve the map
            Stack final_sol, actions = initStack();
            bool can_be_solved = checkMap(to_solve, true) && solve(to_solve, &actions, false);
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

                // Update database
                printf("\nDo you want to update the database ? (y/n) ");
                scanf(" %c", &choice);
                if(choice == 'y'){
                    // Get the full path
                    puts("\nThe following path has been calculated from the previous solution :\n");
                    Stack path = getBestPath(to_solve, &final_sol, true);

                    // Validate the map
                    bool success = setCanBeDone(to_solve, &path);
                    if(success) puts("The solution was sent with no error");
                    else puts("Something went wrong...");
                }
            }
        }
    } 

    else if (choice_app == '1') {
        char choice;
        Map* map2 = NULL;
        bool tmp;
        bool exit;
        char map_name[255];


        puts("\nHi ! Would you like to create a map ? (y/n)");
        scanf(" %c", &choice);
        if(choice == 'y')
        {
            Stack test = initStack();
            int watch_dogs = 0;

            do
            {
                watch_dogs++;
                map2 = generateRandomMap();
                tmp = solve(map2, &test, false);
                if(tmp) {
                    puts("A solvable map as been generated");   
                    display(map2, false);
                    puts("Do you want to generate an other map (this one will be deleted) ? (y/n)");
                    scanf(" %c", &choice);
                    if(choice != 'y') exit = true;
                }
            } while(exit == false && watch_dogs < 50);
            puts("Do you want to upload it ? (y/n)");
            scanf(" %c", &choice);
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
            printf("\n");
        }
        puts("Bye");
    }

    return 0;
}
