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
        tmp->opened_doors[0] = NULL;
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
    appendFrameAtEnd(door->usages, button);
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
    if(compareFrame(frame, locateFrame(map, frame->x, frame->y, false))) return;
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
        int frame_idx = map->nb_items;
        for (int i = 0; i < map->nb_items; i++) {
            if (compareFrame(frame, map->items[i])) {
                free(frame);
                frame_idx = i;
            }
            if(i >= frame_idx){
                if(i == map->nb_items - 1) {
                    map->items[i] = NULL;
                    free(map->items[i]);
                } else map->items[i] = map->items[i+1];
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
            wall->id = id_object;
            wall->usages[0] = NULL;
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
                        printf("l ");
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
    if (dir == 1) // vertical
    {
        int x = nbRand(2,13);
        printf("x -> %d\n", x); // TODO: Retirer cette sécurité
        while ((locateFrame(map, x-1, 6, false) != NULL) || (locateFrame(map, x+1, 6, false) != NULL ) || (locateFrame(map, x, 6, false) != NULL))
        {
            x = nbRand(2,13);
            printf("new x -> %d\n", x); // TODO: Retirer cette sécurité

        }
        for (size_t y = 0; y < 15; y++)
        {
                Frame* tmp = createFrame(x,y,3);
                if (tmp != NULL) addFrameInMap(map, tmp);
        }
    }
    else if (dir == 2)  // horizontal
    { 
        int y = nbRand(1,14);
        printf("y -> %d\n", y); // TODO: Retirer cette sécurité
        while (y == 6 || ((locateFrame(map, 1, y-1, false) != NULL) || (locateFrame(map, 1, y+1, false) != NULL ) || (locateFrame(map, 1, y, false) != NULL)))
        {
            y = nbRand(0,14);
            printf("new y -> %d\n", y); // TODO: Retirer cette sécurité

        }
        for (size_t x = 0; x < 15; x++)
        {
            Frame* tmp = createFrame(x,y,3);
            if (tmp != NULL) addFrameInMap(map, tmp);
        }
    }
    else return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int placeDoor(Map* map)
{
    int x = 0;
    int y = 6;
   // int countWall = 0;

    int coord[4];
    bool creation = true;
    bool posed = false;
    
    Frame* item;
    //Walls wall;

    /*for (size_t x = 0; x < SIZE_MAP; x++)
    {
        if (locateFrame(map, x, y , false) != NULL)
        {
            wall.count[countWall].x = x;
            countWall++;
            wall.nbVert++;
        }
    }
    printf("pos x du 1er mur : %d\n", wall.count[0].x);
    printf("pos x du 2e mur : %d\n", wall.count[1].x);

    printf("nb mur verticaux : %d\n", wall.nbVert);
    */
    
    while(creation == true)
    {
        do  // find x position
        {
            item = locateFrame(map, x, y + 1, false);           
            x--;
                            /*if (item->id == 2)
                            {
                                item = locateFrame(map, x + 1, y + 1, true);
                            }*/
        } while (item == NULL && x >= 0);
        x++;
        coord[0] = x;

        do
        {
            item = locateFrame(map, x + 1, y, false);
            y--;
        }while (item == NULL && y >= 0);
        y++;
        coord[1] = y;

        do
        {
            item = locateFrame(map, x, y + 1, false);           
            x++;
        } while (item == NULL && x < 15);
        x--;
        coord[2] = x;
        
        do 
        {
            y++;
            item = locateFrame(map, x + 1, y, false);
            if(x == 14 && y == 6) printf("C'EST LA FIN");
        } while(item == NULL && y < 15);
        y--;
        coord[3] = y;

        printf("\n%d; %d; %d; %d\n", coord[0], coord[1], coord[2], coord[3]);
        if (creation == true)
        {
            posed = false;
            int pos = 0;
            while(posed == false)
            {  
                int nb = nbRand(0,4);
                //int nb = 2;
                printf("nb rand = %d\n", nb);
                switch (nb)
                {
                case 1: // left wall
                    if(coord[0] != 0)
                    {
                        pos = nbRand(coord[1], coord[3]);

                        createFrameOnWall(map, coord[0], pos, 2);
                        posed = true;
                        x = coord[0];
                        y = coord[3];
                        x--;
                        y--;
                        do
                        {
                            item = locateFrame(map, x, y, false);
                            x--;
                        }while(item == NULL);
                        coord[0] = x;
                    }
                    break;
                
                case 2: // top wall
                    if(coord[1] != 0)
                    {
                        pos = nbRand(coord[0], coord[2] - 1);
                        printf("%d / %d : %d\n", coord[0], coord[2], pos);

                        createFrameOnWall(map, pos, coord[1], 2);
                        posed = true;
                    }
                    break;

                case 3: // right wall
                    if(coord[2] != 14)
                    {
                        pos = nbRand(coord[1], coord[3] - 1);
                        printf("%d / %d : %d\n", coord[1], coord[3], pos);
                        
                        createFrameOnWall(map, coord[2], pos, 2);
                        posed = true;
                    }
                    break;

                case 4: // bottom wall
                    if(coord[3] != 14)
                    {
                        pos = nbRand(coord[0], coord[2]);
                        printf("%d / %d : %d\n", coord[0], coord[2], pos);

                        createFrameOnWall(map, pos, coord[3], 2);
                        posed = true;
                    }
                    break;
                
                default:
                    break;
                }
                creation = false;
            }    
        }
    }    
    return EXIT_SUCCESS;
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
    return !!!(frame1 == NULL || frame2==NULL || frame1->id != frame2->id ||
    frame1->x != frame2->x ||frame1->y != frame2->y);
}

void printFrame(Frame* frame)
{
    printf("id: %d, x: %d, y: %d\n", frame->id, frame->x, frame->y);
}