#include "header.h"

int nbRand(int nbMin, int nbMax)
{
    if ((nbMax >= 0) && (nbMin < nbMax))
    {   
        int x = 0;
        do
        {
            x = 1 + rand()/((RAND_MAX + 1u)/nbMax);
        } while (x <= nbMin);      
        return x;
    }
    return EXIT_FAILURE;
}

Map* createMap(char name[], char author[])
{
    Map* tmp = malloc(sizeof(Frame)*2400);
    if (tmp != NULL) 
    {
        strcpy(tmp->name, name);
        strcpy(tmp->author, author);
        tmp->nb_items = 0;
        tmp->items[0] = NULL;
    } else free(tmp);
    return tmp;
}

Frame* createFrame(int posX, int posY, int id_object) 
{
    Frame* tmp = malloc(sizeof(Frame));
    if (tmp != NULL)
    {
        tmp->x = posX;
        tmp->y = posY;
        tmp->id = id_object;
        tmp->usages[0] = NULL;
        tmp->state = false;
    }
    else
    {
        free(tmp);
    }
    return tmp;
}

int addElementInButton(Frame* button, Frame* door)
{
    if(button == NULL || door == NULL) return EXIT_FAILURE;
    if (button->id != 1) return EXIT_FAILURE;
    appendFrameAtEnd(button->usages, door, MAX_INTERACTION);
    appendFrameAtEnd(door->usages, button, MAX_INTERACTION);
    return EXIT_SUCCESS;
}

void appendFrameAtEnd(Frame* tab[],Frame* add, int max) {
    int i = 0;
    while (tab[i] != NULL && i < max) i++;
    tab[i] = add;
}

void addFrameInMap(Map* map, Frame* frame)
{
    if(compareFrame(frame, locateFrame(map, frame->x, frame->y, false))) return;
    appendFrameAtEnd(map->items, frame, MAX_OBJECT);
    map->nb_items++;
}

Frame* getFrameAtRank(Frame* tab[], int nb_item, int item)
{
    if (item > nb_item) return NULL;
    else return tab[item];
}

Frame* locateFrame(Map* map, int posx, int posy, bool verbose)
{
    if (map == NULL || posx < 0 || posx > SIZE_MAP-1 || posy < 0 || posy > SIZE_MAP-1 )
    {
        if (verbose) puts("La frame n'a pas été retrouvé : Carte ou coordoonées incorrecte");    // TODO: Supprimer ce message
        return NULL;
    }
    for (int i = 0; i < map->nb_items; i++)
    {
        Frame* tmp = getFrameAtRank(map->items, map->nb_items, i);
        if (tmp == NULL) 
        {
            if (verbose) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
            return NULL;
        }
        if ((tmp->x == posx) && (tmp->y == posy)) return tmp;
    }
    if (verbose) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
    return NULL;
}

int deleteFrameInMap(Map* map, Frame* frame, bool verbose) {
    if (map == NULL) {
        if(verbose) puts("La frame n'a pas pu être supprimé : Carte incorrecte");   // TODO: Supprimer ce message
        return EXIT_FAILURE;
    }

    if (frame == NULL) {
        if (verbose) puts("La frame n'a pas pu être supprimé : Frame introuvable");  // TODO: Supprimer ce message
        return EXIT_FAILURE;
    } else {
        const int max_el = map->nb_items;
        int frame_idx = max_el;
        for (int i = 0; i < max_el; i++) {
            if (compareFrame(frame, map->items[i])) {
                free(frame);
                frame_idx = i;
            }
            if(i >= frame_idx){
                if(i == max_el - 1) {
                    map->items[i] = NULL;
                    free(map->items[i]);
                } else {
                    map->items[i] = map->items[i+1];
                }
            }
        }
        map->nb_items--;
    }
    return EXIT_SUCCESS;
}
bool checkCoordinates (int posx, int posy) {
    if (posx < 0 || posx > SIZE_MAP-1 || posy < 0 || posy > SIZE_MAP-1)
    {
        puts("Les coordonnées ne sont pas dans la carte."); // TODO: Supprimer ce message
        return false;
    } else return true;
}


Frame* createFrameOnWall(Map* map, int posX, int posY, int id_object, bool verbose)
{
    if (map == NULL || id_object == 3 || id_object ==4 ) // Watch Dog
    {
        if (verbose) puts("La frame n'a pas été créée : ID ou map incorrect");
        return NULL;
    }
    else
    {
        Frame* wall = locateFrame(map, posX, posY, verbose);
        if (wall == NULL) // WatchDog
        {
            if (verbose) puts("La frame n'a pas été créée : coordonnées incorrecte");
            return wall;
        } else
        {
            wall->id = id_object;
            wall->usages[0] = NULL;
            return wall;
        }
    }
}

int addButtonInMap(Map* map, Frame* button, Frame* door) {
    if (map == NULL || button == NULL || door == NULL) return EXIT_FAILURE;
    addElementInButton(button, door);
    addFrameInMap(map, button);
    addFrameInMap(map, door);
    return EXIT_SUCCESS;
}

void display(Map* map, bool show_zeros)
{   // Create a double loop and go through them, checking for frames in map at the [x][y] coordinates
    for (int x = 0; x < SIZE_MAP; x++)
    { // For the x-axis
        for (int y = 0; y < SIZE_MAP; y++)
        { // For the y-axis
            Frame* tmp = locateFrame(map, y, x, false); // Temporary found variable at [x][y] coordinates
            if (tmp != NULL)    // if there is a Frame found at [x][y] coordinates
            {
                switch (tmp->id)
                { // Select the right caracters to display according to the type of object
                    case 1 :    // button
                        printf("u ");
                        break;
                    case 2:     // door
                        if(tmp->state) {
                            if(show_zeros){
                                printf("0 "); // Do not display anything when there is no object
                            } else printf("  ");
                        } else printf("l ");
                        break;
                    case 3:     // wall
                        printf("& ");
                        break;
                    case 4:     // hole
                        printf("@ ");
                        break;
                    case 5:     // torch
                        printf("* ");
                        break;
                } 
            } else {
                if(show_zeros){
                    printf("0 "); // Do not display anything when there is no object
                } else printf("  ");
            }
        }
        puts(" ");  // Back
    }
    printf("\n");
}

Frame** getAllItemInMap(Map* map, int id_object) {
    Frame** items = (Frame**) malloc(NB_DOOR_MAX * sizeof(Frame));
    size_t j = 0;
    for (int i = 0; i < map->nb_items; i++){
        if (map->items[i]->id == id_object) {
            items[j] = map->items[i];
            j++;
        }
    }
    return items;
}

bool compareFrame(Frame* frame1, Frame* frame2) 
{
    return !(frame1 == NULL || frame2==NULL || frame1->id != frame2->id ||
    frame1->x != frame2->x ||frame1->y != frame2->y);
}

void printFrame(Frame* frame)
{
    if(frame == NULL) puts("The frame is NULL");
    else printf("id: %d, x: %d, y: %d\n", frame->id, frame->x, frame->y);
}