#include "header.h"

int nbRand(int nbMin, int nbMax)
{
    if ((nbMax >= 0) && (nbMin<nbMax))
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
    appendFrameAtEnd(button->usages, door);
    return EXIT_SUCCESS;
}

void appendFrameAtEnd(Frame* tab[],Frame* add)
{
    int i = 0;
    while (tab[i] != NULL) {
        i++;
    }
    tab[i+1] = add;
}

void addFrameInMap(Map* map, Frame* frame)
{
    map->items[map->nb_items] = frame;
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
        if (verbose == true) puts("La frame n'a pas été retrouvé : Carte ou coordoonées incorrecte");    // TODO: Supprimer ce message
        return NULL;
    }
    for (int i = 0; i < map->nb_items; i++)
    {
        Frame* tmp = getFrameAtRank(map->items, map->nb_items, i);
        if (tmp == NULL) 
        {
            if (verbose == true) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
            return NULL;
        }
        if ((tmp->x == posx) && (tmp->y == posy)) return tmp;
    }
    if (verbose == true) puts("La frame n'a pas été retrouvé : Les coordoonées ne sont pas référencés"); // TODO: Supprimer ce message
    return NULL;
}

int deleteFrameInMap(Map* map, int posx, int posy, int id_object, bool verbose)
{
    if (map == NULL)
    {
        puts("La frame n'a pas pu être supprimé : Carte incorrecte");   // TODO: Supprimer ce message
        return EXIT_FAILURE;
    }

    Frame* tmp = locateFrame(map, posx, posy, true);
    if (tmp == NULL)
    {
        if (verbose == true) puts("La frame n'a pas pu être supprimé : Frame introuvable");  // TODO: Supprimer ce message
        return EXIT_FAILURE;
    } else 
    {
        if (id_object == 0 || id_object == tmp->id)
        {
            free(tmp);
            map->nb_items--;
            return EXIT_SUCCESS;
        } else {
            if (verbose == true) puts("La frame n'a pas pu être supprimé : Vérification de l'ID incorrect"); // TODO: Supprimer ce message
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

bool checkCoordinates (int posx, int posy) {
    if (posx < 0 || posx > SIZE_MAP-1 || posy < 0 || posy > SIZE_MAP-1)
    {
        puts("Les coordonnées ne sont pas dans la carte."); // TODO: Supprimer ce message
        return false;
    } else return true;
}


Frame* createFrameOnWall(Map* map, int posX, int posY, int id_object) 
{
    if (map == NULL || id_object == 3 || id_object ==4 )
    {
        puts("La frame n'a pas été créée : Carte ou identifiant incorrect"); // TODO: Supprimer ce message
        return NULL;
    }
    Frame* tmp = malloc(sizeof(Frame));
    if (tmp != NULL)
    {
        Frame* wall = locateFrame(map, posX, posY, true);

        /*if ((posX == posY) == 0) {    //FIXME: Erreur de segmentation
            do {
                posX = nbRand(0,SIZE_MAP);
                posY = nbRand(0,SIZE_MAP);
                wall = locateFrame(map, posX, posY, true);
            } while (wall != NULL && tmp->id == 3);
        }*/

        if (wall == NULL) {
            puts("La frame n'a pas été créée : Mur non existant"); // TODO: Supprimer ce message
            return NULL;
        } else 
        {
            deleteFrameInMap(map,posX,posY,3, true);
            tmp->x = posX;
            tmp->y = posY;
            tmp->id = id_object;
            tmp->usages[0] = NULL;
            addFrameInMap(map, tmp);
        }
        
    }
    else
    {
        puts("La frame n'a pas été crée : Erreur interne"); // TODO: Supprimer ce message
        free(tmp);
    }
    return tmp;
}

int addButtonInMap(Map* map, Frame* button, Frame* door) {
    if (map == NULL || button == NULL || door == NULL) return EXIT_FAILURE;
    addElementInButton(button, door);
    addFrameInMap(map, button);
    addFrameInMap(map, door);
    return EXIT_SUCCESS;
}

void display(Map* map)
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
                        printf("l ");
                        break;
                    case 3:     // wall
                        printf("1 ");
                        break;
                    case 4:     // hole
                        printf("o ");
                        break;
                    case 5:     // torch
                        printf("* ");
                        break;
                } 
            } else printf("  ");    // Do not display anything when there is no object
        }
        puts(" ");  // Back
    }
}

Map* generateRandomMap(int nb_item)
{
    if (nb_item > NB_WALL_MAX) return NULL;
    Map* map = createMap("Random", "Enguerrand");
    if (map == NULL) return NULL;

    trumpWall(map, 1);
    for (int i = 0; i < 3; i++)
    {    
        trumpWall(map, nbRand(0,2));
    }

    return map;
}

int trumpWall(Map* map, int dir) //FIXME: Corriger les problèmes de fonctions infinies.
{
    // dir is the direction choosed to place a Wall
    // 1 -> vertical
    // 2 -> horizontal
    printf("\n");
    if (dir == 1)
    {
        int x = nbRand(2,13);
        printf("x -> %d\n", x); // TODO: Retirer cette sécurité
        while ((locateFrame(map, x-1, 7, false) != NULL) || (locateFrame(map, x+1, 7, false) != NULL ) || (locateFrame(map, x, 7, false) != NULL))
        {
            x = nbRand(2,13);
            printf("new x -> %d\n", x); // TODO: Retirer cette sécurité

        }
        for (size_t y = 0; y < 15; y++)
        {
            
            //if (x == 7) return trumpWall(map,dir + 1);
            //if (locateFrame(map, x-1, 7, false) == NULL && locateFrame(map, x+1, 7, false) == NULL)
            //{ // Check if is there no wall in front and behind it
                Frame* tmp = createFrame(x,y,3);
                if (tmp != NULL) addFrameInMap(map, tmp);
           // }
            //else trumpWall(map, dir + 1);      
        }
    }
    else if (dir == 2)
    { 
        int y = nbRand(1,14);
        printf("y -> %d\n", y); // TODO: Retirer cette sécurité
        while (y == 7 || ((locateFrame(map, 1, y-1, false) != NULL) || (locateFrame(map, 1, y+1, false) != NULL ) || (locateFrame(map, 1, y, false) != NULL)))
        {
            y = nbRand(0,14);
            printf("new y -> %d\n", y); // TODO: Retirer cette sécurité

        }
        for (size_t x = 0; x < 15; x++)
        {

            
          //  if (locateFrame(map, 7, y-1, false) == NULL && locateFrame(map, 7, y+1, false) == NULL)
          //  {   // Check if is there no wall in front and behind it
            Frame* tmp = createFrame(x,y,3);
            if (tmp != NULL) addFrameInMap(map, tmp);
          //  }
           // else trumpWall(map, dir - 1);
        }
    }
    else return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int door(Map* map)
{
    int x = 0;
    int y = 0;
    
}