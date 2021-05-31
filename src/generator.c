#include "../headers/header.h"
#include <stdlib.h>

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

    trumpWall(map, 1, false);
    for (int i = 0; i < 3; i++)
    {    
        trumpWall(map, nbRand(0,2), false);
    }

    return map;
}

int trumpWall(Map* map, int dir, bool verbose) //FIXME: Corriger les problèmes de fonctions infinies.
{
    // dir is the direction choosed to place a Wall
    // 1 -> vertical
    // 2 -> horizontal
    Coord pos;
    //Coord temp;
    int i = 0;
    if (dir == 1) // vertical
    {
        //int x = nbRand(2,12);
        pos = createCoord(nbRand(2,12), 7);
        while (!trumpTower(map, pos, dir, verbose) && i<20)
        {
            i++;
            pos.x = nbRand(2,12);
        }
        printf("%d  ", i);
        if(i >= 20) return EXIT_FAILURE;;

        for (size_t y = 0; y < SIZE_MAP; y++)
        {
            pos.y = y;
            Frame* tmp = createFrameByCoord(pos, ID_WALL);
            if (tmp != NULL) addFrameInMap(map, tmp);
        }
    }
    else if (dir == 2)  // horizontal
    { 
        //int y = nbRand(1,12);
        pos = createCoord(1, nbRand(1,12));
        while (pos.y == 7 && !trumpTower(map, pos, dir, verbose) && i<20)
        {
            i++;
            pos.y = nbRand(1,12);
        }
        printf("%d  ", i);
        if(i >= 20) return EXIT_FAILURE;
        for (size_t x = 0; x < SIZE_MAP; x++)
        {
            pos.x = x;
            Frame* tmp = createFrameByCoord(pos, ID_WALL);
            if (tmp != NULL) addFrameInMap(map, tmp);
        }
    }
    else return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int placeDoor(Map* map, bool verbose)
{
    int i = 0;

    bool creation = true;
    bool posed = false;
    bool reachEnd = false;
    
    Frame* item;
    Frame* door;

    Coord start;    // first door location
    Coord end;      // second door location
    Coord top;      // top left hand corner coord
    Coord bottom;   // bottom right hand corner coord

    int x = 0;  // niquez vous j'en ai besoin
    int y = 7;  // niquez vous j'en ai besoin

    start.x = START_X;
    start.y = START_Y;

    while(creation == true && i < 100) //pose des portes
    {
        i++;
        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);           
            x--;
        } while (item == NULL && x >= 0);
        x++;
        top.x = x;
        x++;

        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);
            y--;

        }while (item == NULL);
        y++;
        top.y = y;
        y++;

        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);           
            x++;

        } while (item == NULL && x < 15);
        x--;
        bottom.x = x;
        if(x != 14) x--;
        
        do 
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);
            y++;

            if(x == 14 && y == 7) 
            {
                if (verbose) puts("FIN ATTEINTE");
                creation = false;
                reachEnd = true;
            }
        } while(item == NULL && y < 15);
        y--;
        bottom.y = y;
        puts("ici");


        if (verbose)  printf("\n%ld; %ld; %ld; %ld\n", top.x, top.y, bottom.x, bottom.y);
        if (creation == true)
        {
            posed = false;
            int pos = 0;

            while(posed == false && i<100) // pose de la porte de la chambre
            {  
                i++;
                int nb = nbRand(1,4);
                if (verbose) printf("random mur = %d\n", nb);
                switch (nb)
                {                
                case 2: // top wall
                    if(top.y != 0 && passePartout(map, top.y, top.x, bottom.x, 2, verbose) == true)
                    {
                        pos = nbRand(top.x, bottom.x - 1);
                        if(bottom.x==14) pos = nbRand(top.x, bottom.x);

                        if (verbose) printf("%ld / %ld : %d\n", top.x, bottom.x, pos);
                        door = createFrameOnWallWithCoord(map, createCoord(pos, top.y), ID_DOOR, verbose);
                        posed = true;
                        end.x = pos;
                        end.y = top.y;

                        x = pos;
                        y = top.y;
                        y--;
                    }
                    break;

                case 3: // right wall
                    if(bottom.x != 14 && passePartout(map, bottom.x, top.y, bottom.y, 1, verbose) == true)
                    {
                        pos = nbRand(top.y, bottom.y - 1);
                        if(bottom.y==14) pos = nbRand(top.y, bottom.y);
                        if (verbose) printf("%ld / %ld : %d\n", top.y, bottom.y, pos);
                        
                        door = createFrameOnWallWithCoord(map, createCoord(bottom.x, pos), ID_DOOR, verbose);
                        posed = true;
                        end.x = bottom.x;
                        end.y = pos;

                        x = bottom.x;
                        y = pos;
                        x++;                        
                    }
                    break;

                case 4: // bottom wall
                    if(bottom.y != 14 && passePartout(map, bottom.y, top.x, bottom.x, 2, verbose) == true)
                    {
                        pos = nbRand(top.x, bottom.x - 1);
                        if(bottom.x == 14) pos = nbRand(top.x, bottom.x);
                        if (verbose) printf("%ld / %ld : %d\n", top.x, bottom.x, pos);

                        door = createFrameOnWallWithCoord(map, createCoord(pos, bottom.y), ID_DOOR, verbose);
                        posed = true;
                        end.x = pos;
                        end.y = bottom.y;

                        x = pos;
                        y = bottom.y;
                        y++;
                    }
                    break;               
                default:
                    break;
                } 
            }
            int j = 0;
            posed = false;
            if(reachEnd != true)
            {
                Frame* lever;
                while(posed == false && j < 50) //pose des leviers
                {
                    posed = false;
                    Coord tmp = createCoord(nbRand(top.x,bottom.x-1), nbRand(top.y, bottom.y-1));
                    lever = createFrameByCoord(tmp ,ID_BUTTON);  
                    addFrameInMap(map, lever);
                    if(pathfinding(map, start, end, verbose) == false)
                    {
                        removeFromList(map->items, lever, true);
                        if (verbose) puts("levier retiré");  //TODO : Enlever cette merde
                    }
                    else
                    {
                        posed = true;
                        addElementInButton(lever, door);
                        addElementInButton(door, lever);
                        if (verbose) puts("levier posé");    //TODO : Enlever cette merde
                    }
                    j++;
                }
                roccoSiffredi(map, start, end, top, bottom, lever->pos);
                start = end;
            }
        }
        // tmp = createCoord(7,14);
        // roccoSiffredi(map, start, tmp, top, bottom, tmp);
    }  
    lanceFlamme(map);  
    return EXIT_SUCCESS;
}

bool passePartout(Map* map, int wallPos, int start, int end, int dir, bool verbose)
{    
    Frame* tmp;
    if (dir == 1) // 1 for a vertical wall
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrameByCoord(map, createCoord(wallPos, i), verbose);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    else if (dir == 2)
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrameByCoord(map, createCoord(i, wallPos), verbose);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    else return EXIT_FAILURE;
    return true;  
}

int roccoSiffredi(Map* map, Coord start, Coord end, Coord top, Coord bottom, Coord lever)
{
    int nbHole = nbRand(0, ((bottom.x - top.x) * (bottom.y - top.y))/4);
    printf("nb trou %d\n", nbHole);


    bool posed = false;
    int j = 0;
    for (int i = 0; i < nbHole; i++)
    {
        posed = false;
        j=0;
        while (posed == false && j < 10)
        {
            j++;
            Coord pos = createCoord(nbRand(top.x, bottom.x-1), nbRand(top.y, bottom.y-1));
            if (locateFrameByCoord(map, pos, false) == NULL)
            {
                Frame* hole = createFrameByCoord(pos, ID_HOLE);  
                addFrameInMap(map, hole);
                if(!pathfinding(map, start, lever, false) || !pathfinding(map, lever, end, false) || !pathfinding(map,start,end,false))
                {
                    removeFromList(map->items, hole, true);
                }
                else posed = true;
            }
        }
    }
    return 1;
}

void lanceFlamme(Map* map)
{
    Coord pos;
    Frame* tmp;
    bool posed = false;
    for (int i = 0; i < nbRand(0,3); i++)
    {
        posed = false;
        while(posed == false)
        {
            pos = createCoord(nbRand(0, 14), nbRand(0, 14));
            if (locateFrameByCoord(map, pos, false) == NULL)
            {
                tmp = createFrameByCoord(pos, ID_TORCH);
                addFrameInMap(map, tmp);
                posed = true;
            }
        }
    }
}

bool trumpTower(Map* map, Coord pos, int dir, bool verbose)
{
    if(dir == 1)
    {        
        if ((locateFrameByCoord(map, createCoord(pos.x-2, 7), verbose) == NULL) && (locateFrameByCoord(map, createCoord(pos.x+2, 7), verbose) == NULL) && (locateFrameByCoord(map,createCoord(pos.x-1, 7), verbose) == NULL) && (locateFrameByCoord(map,createCoord(pos.x+1, 7), verbose) == NULL ) && (locateFrameByCoord(map,createCoord(pos.x, 7), verbose) == NULL))  return true;
        else return false;
    }
    if (dir == 2)
    {
        if((locateFrameByCoord(map, createCoord(0, pos.y-2), verbose) == NULL) && (locateFrameByCoord(map, createCoord(0, pos.y+2), verbose) == NULL ) && (locateFrameByCoord(map, createCoord(0, pos.y-1), verbose) == NULL) && (locateFrameByCoord(map, createCoord(0, pos.y+1), verbose) == NULL ) && (locateFrameByCoord(map, createCoord(0, pos.y), verbose) == NULL)) return true;
        else return false;
    }
    return false;
}
