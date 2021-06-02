#include "../headers/header.h"
#include <json-c/json_object.h>
#include <stdbool.h>
#include <stdlib.h>

int nbRand(int nb_min, int nb_max)
{
    if ((nb_max >= 0) && (nb_min<nb_max))
    {   
        int x = 0;
        do
        {
            x = (int)((double)rand() / ((double)RAND_MAX + 1) * ((double)nb_max - (double)nb_min)) + nb_min;
        } while (x < nb_min);      
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

Map* generateRandomMap() {
    Map* map = createMap("Random", "Generatoooooooor");
    if (map == NULL) return NULL;

    trumpWall(map, 1, false);
    for (int i = 0; i < 3; i++)
    {    
        trumpWall(map, nbRand(1,3), false);
    }

    placeDoor(map, false);

    flameThrower(map);

    return map;
}

int trumpWall(Map* map, int dir, bool verbose) 
{
    // dir is the direction choosed to place a Wall
    // 1 -> vertical
    // 2 -> horizontal
    Coord pos;
    int i = 0;
    if (dir == 1) // vertical
    {
        do
        {
            pos = createCoord(nbRand(2,13), 7);
            i++;
        }while (trumpTower(map, pos, dir, verbose) == false && i<50);
        if(i < 40)
        {
            for (size_t y = 0; y < SIZE_MAP; y++)
            {
                pos.y = y;
                Frame* tmp = createFrameByCoord(pos, ID_WALL);
                if (tmp != NULL) addFrameInMap(map, tmp);
            }
        }
        else trumpWall(map, 2, verbose);

    }
    else if (dir == 2)  // horizontal
    { 
        do
        {
            pos = createCoord(1, nbRand(2,13));
            i++;
        }while ((pos.y == 7 || trumpTower(map, pos, dir, verbose) == false) && i<50);
        if(i < 40)
        {
            for (size_t x = 0; x < SIZE_MAP; x++)
            {
                pos.x = x;
                Frame* tmp = createFrameByCoord(pos, ID_WALL);
                if (tmp != NULL) addFrameInMap(map, tmp);
            }
        }   
        else trumpWall(map, 1, verbose);
    }
    else return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int placeDoor(Map* map, bool verbose)
{
    bool creation = true;
    bool posed = false;
    bool reachEnd = false;
    
    Frame* item;
    Frame* door;

    Coord start;    // first door location
    Coord end;      // second door location
    Coord top;      // top left hand corner coord
    Coord bottom;   // bottom right hand corner coord

    int x = 0;  // DON'T TOUCH IT
    int y = 7;  // DON'T TOUCH IT

    start.x = START_X;
    start.y = START_Y;

    int watchDogs = 0;
    int watchDogs2 = 0;
    while(creation == true && watchDogs < 100) //pose des portes
    {
        watchDogs2 = 0;
        watchDogs++;
        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);           
            x--;
            watchDogs2++;
        } while (item == NULL && x >= 0 && watchDogs2 < 20);
        x++;
        top.x = x;
        x++;
        watchDogs2 = 0;
        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);
            y--;
            watchDogs2++;
        }while (item == NULL && y >= 0 && watchDogs2 < 20);
        y++;
        top.y = y;
        y++;
        watchDogs2 = 0;
        do
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);           
            x++;
            watchDogs2++;
        } while (item == NULL && x < 15 && watchDogs2 < 20);
        x--;
        bottom.x = x;
        if(x != 14) x--;
        watchDogs2 = 0;
        do 
        {
            item = locateFrameByCoord(map, createCoord(x, y), verbose);
            watchDogs2++;
            if(x == END_X && y == END_Y && item == NULL) 
            {
                if (verbose) puts("FIN ATTEINTE");
                creation = false;
                reachEnd = true;
            }
            y++;
        } while(item == NULL && y < 15 && watchDogs2 < 20);
        y--;
        bottom.y = y;

        if (verbose) printf("\n%ld; %ld; %ld; %ld\n", top.x, top.y, bottom.x, bottom.y);
        if (creation == true)
        {
            posed = false;
            int pos = 0;

            while(posed == false && watchDogs < 100) // pose de la porte de la chambre
            {  
                watchDogs++;
                int nb = nbRand(2,5);
                if (verbose) printf("random mur = %d\n", nb);
                switch (nb)
                {                
                case 2: // top wall
                    if(top.y != 0 && passePartout(map, top.y, top.x, bottom.x, 2, verbose) == true)
                    {
                        pos = nbRand(top.x + 1, bottom.x);
                        if(bottom.x==14) pos = nbRand(top.x + 1, bottom.x+1); // TODO vérifier

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
                        pos = nbRand(top.y + 1, bottom.y);
                        if(bottom.y==14) pos = nbRand(top.y + 1, bottom.y + 1); //TODO vérifier
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
                        pos = nbRand(top.x + 1, bottom.x);
                        if(bottom.x == 14) pos = nbRand(top.x + 1, bottom.x + 1);   // TODO vérifier
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
            watchDogs2 = 0;
            posed = false;
            if(reachEnd != true)
            {
                Frame* lever;
                while(posed == false && watchDogs2 < 50) //pose des leviers
                {
                    posed = false;
                    Coord tmp;
                    
                    if(top.x == 0 && top.y == 0) tmp = createCoord(nbRand(top.x,bottom.x), nbRand(top.y, bottom.y));
                    else if(top.y == 0 && bottom.x == 14) tmp = createCoord(nbRand(top.x+1,bottom.x+1), nbRand(top.y, bottom.y));
                    else if(bottom.x == 14 && bottom.y == 14) tmp = createCoord(nbRand(top.x+1,bottom.x+1), nbRand(top.y+1, bottom.y+1));
                    else if(top.x == 0 && bottom.y == 14) tmp = createCoord(nbRand(top.x,bottom.x), nbRand(top.y+1, bottom.y+1));
                    else  tmp = createCoord(nbRand(top.x+1,bottom.x), nbRand(top.y+1, bottom.y));
                    
                    lever = createFrameByCoord(tmp ,ID_BUTTON);  
                    addFrameInMap(map, lever);
                    if(pathfinding(map, start, end, verbose) == true && (tmp.x != 0 && tmp.y != 7))
                    {
                        posed = true;
                        addElementInButton(lever, door);
                        addElementInButton(door, lever);
                        if (verbose) puts("levier posé");
                    }
                    else
                    {
                        removeFromList(map->items, lever, true);
                        if (verbose) puts("levier retiré");

                    }
                    watchDogs2++;
                }
                holeInOne(map, start, end, top, bottom, lever->pos);
                start = end;
            }
        }
    }  
    holeInOne(map, start, createCoord(14,7), top, bottom, createCoord(13, 7));
    return EXIT_SUCCESS;
}

bool passePartout(Map* map, int wall_pos, int start, int end, int dir, bool verbose)
{    
    Frame* tmp;
    if (dir == 1) // 1 for a vertical wall
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrameByCoord(map, createCoord(wall_pos, i), verbose);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    else if (dir == 2)
    {
        for (int i = start; i <= end; i++)
        {
            tmp = locateFrameByCoord(map, createCoord(i, wall_pos), verbose);
            if(tmp->id == ID_DOOR) return false;
        }
    }
    return true;  
}

int holeInOne(Map* map, Coord start, Coord end, Coord top, Coord bottom, Coord lever)
{
    int nbHole = nbRand(1, ((bottom.x - top.x) * (bottom.y - top.y))/4+1);

    bool posed = false;
    int j = 0;
    for (int i = 0; i < nbHole; i++)
    {
        posed = false;
        j=0;
        while (posed == false && j < 50)
        {
            j++;
            Coord pos = createCoord(nbRand(top.x+1, bottom.x), nbRand(top.y+1, bottom.y));
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

void flameThrower(Map* map)
{
    Coord pos;
    Frame* tmp;
    bool posed = false;
    for (int i = 0; i < nbRand(1,4); i++)
    {
        posed = false;
        while(posed == false)
        {
            pos = createCoord(nbRand(1, 15), nbRand(1, 15));
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
        if ((locateFrameByCoord(map, createCoord(pos.x-2, 7), verbose) != NULL) || (locateFrameByCoord(map, createCoord(pos.x+2, 7), verbose) != NULL) || (locateFrameByCoord(map,createCoord(pos.x-1, 7), verbose) != NULL) || (locateFrameByCoord(map,createCoord(pos.x+1, 7), verbose) != NULL) || (locateFrameByCoord(map,createCoord(pos.x, 7), verbose) != NULL)) return false;
        else return true;
    }
    if (dir == 2)
    {
        if((locateFrameByCoord(map, createCoord(1, pos.y-2), verbose) != NULL) || (locateFrameByCoord(map, createCoord(1, pos.y+2), verbose) != NULL) || (locateFrameByCoord(map, createCoord(1, pos.y-1), verbose) != NULL) || (locateFrameByCoord(map, createCoord(1, pos.y+1), verbose) != NULL) || (locateFrameByCoord(map, createCoord(1, pos.y), verbose) != NULL)) return false;
        else return true;
    }
    return false;
}