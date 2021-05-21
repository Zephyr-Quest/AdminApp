#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define _countof(array) (sizeof(&array) / sizeof(array[0]))

// HTTP libs
#include <curl/curl.h>
#include<json-c/json.h>

#define NB_HOLE_MAX 5   // Maximum number of holes
#define NB_BUTTON_MAX 5 // Maximum number of button 
#define NB_LEVER_MAX 5  // Maximum number of lever
#define NB_WALL_MAX 20  // Maximum number of wall
#define MAX_INTERACTION 3   // Maximum interaction for items
#define MAX_NAME_SIZE 255   // Maximum caracters for naming map
#define SIZE_MAP 15 // Size of map
#define MAX_OBJECT ((SIZE_MAP * SIZE_MAP) - 1)

// Start point
#define START_X 0
#define START_Y 7

// End point
#define END_X 14
#define END_Y 7

// Frame Strucure
typedef struct Frame
{
    int id; // Object identifier : Recognises the type of the object
    int x;  // x position  
    int y;  // y position
    struct Frame* usages[MAX_INTERACTION]; // If it is a sensor, list the linked actuators
} Frame;

// Map structure
typedef struct Map
{
    char name[MAX_NAME_SIZE];       // Name of the map
    char author[MAX_NAME_SIZE];     // Author of the map
    struct Frame* items[MAX_OBJECT]; // Object in the map
    int nb_items;                   // Number of objects in the map
} Map;

/*
 * GENERATOR
 */

// Return random number, between ]min ; max] 
int nbRand(int nbMin, int nbMax);

// Create a simple frame
Frame * createFrame(int posX, int posY, int id_object);

// Create an object on a wall. Set posX and posY to 0 to randomize position
Frame* createFrameOnWall(Map* map, int posX, int posY, int id_object);

// D4rk V4d0r is creating a map : xX_De4th-5t4r5_Xx
Map* createMap(char name[], char author[]);

// Add button's girlfriend (door)
int addElementInButton(Frame* button, Frame* door);

// Add a frame at the end of an array
void appendFrameAtEnd(Frame* tab[],Frame* add);

// Add a frame in the map
void addFrameInMap(Map* map, Frame* frame);

// return the frame at the "item" rank
Frame* getFrameAtRank(Frame* tab[], int nb_item, int item);

// return the frame at position x, y
Frame* locateFrame(Map* map, int posx, int posy, bool verbose);

// frere il se comprend celui ci non ?
void display(Map* map);

// Generate a random map
Map* generateRandomMap(int nb_item);

// Add a door and a button in a map
int addButtonInMap(Map* map, Frame* door, Frame* button);

// Create a straight wall 
int trumpWall(Map* map, int dir);

// Remove an item in the map using its coordinates. Using ID for checking object, let id = 0 for no checking
int deleteFrameInMap(Map* map, int posX, int posY, int id_object, bool verbose);

// Check if the coordinates [x][y] are in the map
bool checkCoordinates (int posx, int posy); 

// Place door(s) on the map
int placeDoor(Map* map);

/*
 * HTTP
 */

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct Coord Coord;
struct Coord {
    size_t x;
    size_t y;
};

// Check if a HTTP request was succeeded by its response code
bool isHttpError(long http_code);

// Send a HTTP GET request
char* getRequest(const char* path, long* response_code);

// Send a HTTP POST request
bool postRequest(const char* path, const char* body);

// Get all of saved maps, return the length of the array
Map* * getAllMaps(size_t* nb_maps);

// Get a map by its name
Map* getMapByName(const char* name);

// Upload a new generated map to the server
bool uploadNewMap(Map* map);

/*
 * FILES
 */

typedef struct Element Element;
struct Element {
    Coord data;
    Element *next;
};
typedef struct File File;
struct File {
    Element *first;
};

// Init a file
File initFile();

// Put a new value to the file
void put(File *file, Coord data);

// Pull the first value of the file (remove it)
Coord pull(File *file);

/*
 * SOLVER
 */

// Search the four near points around a specific point
Coord* getNearPoints(Coord center);

// Try to find a way from 'start' point to 'end' point
bool pathfinding(char map[SIZE_MAP][SIZE_MAP], Coord start, Coord end, bool verbose);

// Check if a point is in the map
bool isInMap(Coord point);

// return the frame at position x, y
Frame* locateFrameByCoord(Map* map, Coord coord, bool verbose);

// Generate a 2D array which store a Map
void generateMapArray(char destination[SIZE_MAP][SIZE_MAP], Map* source);

// Generate a 2D array which store a Map
void printMapArray(char map[SIZE_MAP][SIZE_MAP], bool show_zeros);

// Check if a char is an obactle
bool isObstacle(char);

// Check if a char can be hover by the player
bool canBeHover(char);

// Check if two points are equals or not
bool isCoordsEquals(Coord, Coord);

// Copy the map content into an other map
void mapCopy(char destination[SIZE_MAP][SIZE_MAP], char source[SIZE_MAP][SIZE_MAP]);