#include "../headers/header.h"

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

int addElementInButton(Frame* button, Frame* door) {
    if(button == NULL || door == NULL) return EXIT_FAILURE;
    if (button->id != ID_BUTTON) return EXIT_FAILURE;
    appendAtList(button->usages, door);
    appendAtList(door->usages, button);
    return EXIT_SUCCESS;
}

Frame* createFrameOnWall(Map* map, int posX, int posY, int id_object, bool verbose) {
    if (map == NULL || id_object == ID_WALL || id_object == ID_HOLE ) // Watch Dog
    {
        if (verbose) puts("La frame n'a pas été créée : ID ou map incorrect");
        return NULL;
    } else {
        Frame* wall = locateFrame(map, posX, posY, verbose);
        if (wall == NULL) // WatchDog
        {
            if (verbose) puts("La frame n'a pas été créée : coordonnées incorrecte");
            return wall;
        } else {
            wall->id = id_object;
            wall->usages = initList();
            return wall;
        }
    }
}

Frame* createFrameOnWallWithCoord(Map* map, Coord coord, int id_object, bool verbose) {
    if (map == NULL || id_object == ID_WALL || id_object == ID_HOLE ) // Watch Dog
    {
        if (verbose) puts("La frame n'a pas été créée : ID ou map incorrect");
        return NULL;
    } else {
        Frame* wall = locateFrameByCoord(map, coord, verbose);
        if (wall == NULL) // WatchDog
        {
            if (verbose) puts("La frame n'a pas été créée : coordonnées incorrecte");
            return wall;
        } else {
            wall->id = id_object;
            wall->usages = initList();
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

Map* generateRandomMap(int nb_item) {
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
        while ((locateFrame(map, x-2 , 7, false) != NULL) || (locateFrame(map, x+2, 7, false) != NULL) || (locateFrame(map, x-1, 7, false) != NULL) || (locateFrame(map, x+1, 7, false) != NULL ) || (locateFrame(map, x, 7, false) != NULL))
        {
            x = nbRand(2,13);
            printf("new x -> %d\n", x); // TODO: Retirer cette sécurité

        }
        for (size_t y = 0; y < SIZE_MAP; y++)
        {
                Frame* tmp = createFrame(x, y, ID_WALL);
                if (tmp != NULL) addFrameInMap(map, tmp);
        }
    }
    else if (dir == 2)  // horizontal
    { 
        int y = nbRand(1,13);
        printf("y -> %d\n", y); // TODO: Retirer cette sécurité
        while (y == 7 || ((locateFrame(map, 1, y-1, false) != NULL) || (locateFrame(map, 1, y+1, false) != NULL ) || (locateFrame(map, 1, y, false) != NULL)))
        {
            y = nbRand(0,13);
            printf("new y -> %d\n", y); // TODO: Retirer cette sécurité

        }
        for (size_t x = 0; x < SIZE_MAP; x++)
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
    int y = 7;
    int i = 0;

    int coord[4];
    bool creation = true;
    bool posed = false;
    bool reachEnd = false;
    
    Frame* item;
    Frame* door;

    Coord start;
    Coord end;

    start.x = START_X;
    start.y = START_Y;
    
    while(creation == true && i < 100)
    {
        i++;
        do
        {
            item = locateFrame(map, x, y, false);           
            x--;
        } while (item == NULL && x >= 0);
        x++;
        coord[0] = x;
        x++;

        do
        {
            item = locateFrame(map, x, y, false);
            y--;
            if(item != NULL)
            {
                if(item->id == 1) item = NULL;
            }
        }while (item == NULL && y >= 0);
        y++;
        coord[1] = y;
        y++;

        do
        {
            item = locateFrame(map, x, y, false);           
            x++;
            if(item != NULL)
            {
                if(item->id == 1) item = NULL;
            }
        } while (item == NULL && x < 15);
        x--;
        coord[2] = x;
        if(x != 14) x--;
        
        do 
        {
            item = locateFrame(map, x, y, false);
            y++;
                        
            if(x == 14 && y == 7) 
            {
                puts("FIN ATTEINTE");
                creation = false;
                reachEnd = true;
            }
            if(item != NULL)
            {
                if(item->id == 1) item = NULL;
            }
        } while(item == NULL && y < 15);
        y--;
        coord[3] = y;

        printf("\n%d; %d; %d; %d\n", coord[0], coord[1], coord[2], coord[3]);
        if (creation == true)
        {
            posed = false;
            int pos = 0;

            while(posed == false && i<100)
            {  
                i++;
                int nb = nbRand(1,4);
                printf("random mur = %d\n", nb);
                switch (nb)
                {                
                case 2: // top wall
                    if(coord[1] != 0 && passePartout(map, coord[1], coord[0], coord[2], 2) == true)
                    {
                        pos = nbRand(coord[0], coord[2] - 1);
                        if(coord[2]==14) pos = nbRand(coord[0], coord[2]);

                        printf("%d / %d : %d\n", coord[0], coord[2], pos);

                        door = createFrameOnWall(map, pos, coord[1], ID_DOOR, true);
                        posed = true;
                        end.x = pos;
                        end.y = coord[1];

                        x = pos;
                        y = coord[1];
                        y--;
                    }
                    break;

                case 3: // right wall
                    if(coord[2] != 14 && passePartout(map, coord[2], coord[1], coord[3], 1) == true)
                    {
                        pos = nbRand(coord[1], coord[3] - 1);
                        if(coord[3]==14) pos = nbRand(coord[1], coord[3]);
                        printf("%d / %d : %d\n", coord[1], coord[3], pos);
                        
                        door = createFrameOnWall(map, coord[2], pos, ID_DOOR, true);
                        posed = true;
                        end.x = coord[2];
                        end.y = pos;

                        x = coord[2];
                        y = pos;
                        x++;                        
                    }
                    break;

                case 4: // bottom wall
                    if(coord[3] != 14 && passePartout(map, coord[3], coord[0], coord[2], 2) == true)
                    {
                        pos = nbRand(coord[0], coord[2] - 1);
                        if(coord[2]==14)pos = nbRand(coord[0], coord[2]);
                        printf("%d / %d : %d\n", coord[0], coord[2], pos);

                        door = createFrameOnWall(map, pos, coord[3], ID_DOOR, true);
                        posed = true;
                        end.x = pos;
                        end.y = coord[3];

                        x = pos;
                        y = coord[3];
                        y++;
                    }
                    break;
                
                default:
                    break;
                } 
                int j = 0;
                if(reachEnd != true)
                {
                    do
                    {
                        posed = false;
                        //Coord tmp = createCoord(nbRand(coord[0],coord[2]-1), nbRand(coord[1], coord[3]-1));
                        Frame* lever = createFrame(nbRand(coord[0],coord[2]-1), nbRand(coord[1], coord[3]-1),ID_BUTTON);  
                        addFrameInMap(map, lever);
                        if(pathfinding(map, start, end, false) == false)
                        {
                            removeFromList(map->items, lever, true);
                            puts("levier retiré");
                        }
                        else
                        {
                            posed = true;
                            addElementInButton(lever, door);
                            addElementInButton(door, lever);
                            start = end;
                            puts("levier posé");
                        }
                        j++;
                    } while(posed == false && j < 50);
                }
                
                 
                     
            }    
        }
    }    
    return EXIT_SUCCESS;
}

bool passePartout(Map* map, int wallPos, int start, int end, int dir)
{    
    Frame* tmp;
    if (dir == 1) // 1 for a vertical wall
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrame(map, wallPos, i, false);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    else if (dir == 2)
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrame(map, i, wallPos, false);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    else return EXIT_FAILURE;
    return true;  
}
