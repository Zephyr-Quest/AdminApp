#pragma once

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define _countofFrames(array) (sizeof(array) / sizeof(struct Frame*))

// HTTP libs
#include <curl/curl.h>
#include <json-c/json.h>

#define NB_HOLE_MAX 5   // Maximum number of holes
#define NB_BUTTON_MAX 5 // Maximum number of button 
#define NB_LEVER_MAX 5  // Maximum number of lever
#define NB_WALL_MAX 20  // Maximum number of wall
#define NB_DOOR_MAX 15  // Maximum number of wall
#define MAX_INTERACTION 3   // Maximum interaction for items
#define MAX_ACTIONS 100 // Maximum of player actions
#define MAX_NAME_SIZE 255   // Maximum caracters for naming map
#define SIZE_MAP 15 // Size of map
#define MAX_OBJECT ((SIZE_MAP * SIZE_MAP) - 1)

// Caracters
#define BUTTON 'u'
#define DOOR 'l'
#define WALL '&'
#define HOLE '@'
#define TORCH '*'

// Start point
#define START_X 0
#define START_Y 7

// End point
#define END_X 14
#define END_Y 7

// Coordinates Structure

typedef struct Coord Coord;
struct Coord {
    size_t x;
    size_t y;
};

// Frame Strucure
typedef struct Frame
{
    int id; // Object identifier : Recognises the type of the object
    int x;  // x position  
    int y;  // y position
    struct Frame* usages[MAX_INTERACTION]; // If it is a sensor, list the linked actuators
    bool state; // If the door is open (false if it's not a door)
} Frame;

// Map structure
typedef struct Map
{
    char name[MAX_NAME_SIZE];       // Name of the map
    char author[MAX_NAME_SIZE];     // Author of the map
    struct Frame* items[MAX_OBJECT]; // Object in the map
    int nb_items;                   // Number of objects in the map
} Map;

Frame** getAllItemInMap(Map* map, int id_object);


/*
 * GENERATOR
 */

// Return random number, between ]min ; max] 
int nbRand(int nbMin, int nbMax);

// Create a simple frame
Frame * createFrame(int posX, int posY, int id_object);

// Create an object on a wall. Set posX and posY to 0 to randomize position
Frame* createFrameOnWall(Map* map, int posX, int posY, int id_object, bool verbose);

// D4rk V4d0r is creating a map : xX_De4th-5t4r5_Xx
Map* createMap(char name[], char author[]);

// Add button's girlfriend (door)
int addElementInButton(Frame* button, Frame* door);

// Add a frame at the end of an array
void appendFrameAtEnd(Frame* tab[],Frame* add, int max);

// Add a frame in the map
void addFrameInMap(Map* map, Frame* frame);

// return the frame at the "item" rank
Frame* getFrameAtRank(Frame* tab[], int nb_item, int item);

// return the frame at position x, y
Frame* locateFrame(Map* map, int posx, int posy, bool verbose);

// frere il se comprend celui ci non ?
void display(Map* map, bool show_zeros);

// Generate a random map
Map* generateRandomMap(int nb_item);

// Add a door and a button in a map
int addButtonInMap(Map* map, Frame* door, Frame* button);

// Create a straight wall 
int trumpWall(Map* map, int dir);

// Remove an item in the map using its coordinates. Using ID for checking object, let id = 0 for no checking
int deleteFrameInMap(Map* map, Frame* frame, bool verbose);

// Check if the coordinates [x][y] are in the map
bool checkCoordinates (int posx, int posy); 

// Place door(s) on the map
int placeDoor(Map* map);

// Check if there is a door on the wall
bool passePartout(Map* map, int wallPos, int start, int end, int dir);

// Compare two frames
bool compareFrame(Frame* frame1, Frame* frame2);

// Print the given frame
void printFrame(Frame*);

/*
 * HTTP
 */

struct MemoryStruct {
  char *memory;
  size_t size;
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
 * STACKS
 */

typedef struct Element Element;
struct Element {
    Coord data;
    Element *next;
};
typedef struct Stack Stack;
struct Stack {
    Element *first;
};

// Init a stack
Stack initStack();

// Put a new value to the stack
void put(Stack *stack, Coord data);

// Pull the first value of the stack (remove it)
Coord pull(Stack *stack);

// Put a new value to the stack
void putFrame(Stack *stack, Frame* data);

// Pull the first value of the stack (remove it)
Frame* pullFrame(Stack *stack, Map* map);

// Check if a stack contains a coord
bool stackContains(Stack *stack, Coord data);

// Check if a stack contains a frame
bool stackContainsFrame(Stack* stack, Frame* frame);

/*
 * LIST
 */

typedef struct ListElement ListElement;
struct ListElement
{
    Frame* data;
    ListElement *next;
};
typedef struct List List;
struct List
{
    ListElement *first;
};

// Init a new list
List* initList();

// Add a new frame in the list
void appendAtList(List *list, Frame* data);

// Get a 'ListElement' at a specific rank
ListElement* getElementAtRank(List* list, size_t rank);

// Get a 'Frame' at a specific rank
Frame* getFrameInList(List* list, size_t rank);

// Try to remove a 'Frame' from a list
void removeFromList(List *list, Frame* frame, bool verbose);

/*
 * SOLVER
 */

// Search the four near points around a specific point
Coord* getNearPoints(Coord center);

// Try to find a way from 'start' point to 'end' point
bool pathfinding(Map* map, Coord start, Coord end, bool verbose);

// Check if a point is in the map
bool isInMap(Coord point);

// return the frame at position x, y
Frame* locateFrameByCoord(Map* map, Coord coord, bool verbose);

// Generate a 2D array which store a Map
void generateMapArray(char destination[SIZE_MAP][SIZE_MAP], Map* source);

// Generate a 2D array which store a Map
void printMapArray(char map[SIZE_MAP][SIZE_MAP], bool show_zeros);

// Print given coordinates
void printCoord(Coord);

// Check if a char is an obactle
bool isObstacle(char);

// Check if a char can be hover by the player
bool canBeHover(char);

// Check if two points are equals or not
bool isCoordsEquals(Coord, Coord);

// Copy the map content into an other map
void mapCopy(char destination[SIZE_MAP][SIZE_MAP], char source[SIZE_MAP][SIZE_MAP]);

// Search all of blocking door
Frame** searchExits(Map* map, Coord player);

// Search lever which open the door
Frame* getDoorLever(Map* map, Frame* door, Coord player, Stack* actions);

// Try to open a door, return false if the lever can't be reached
bool useLever(Map* map, Frame* lever, Coord* player, bool verbose);

// Move the player to a specific location
bool moveTo(Map* map, Coord* player, Coord destination, bool verbose);

// Try to solve 
bool solve(Map* map, Stack* interactions, bool verbose);


// CONIO
// 
int _getch(void);
//
int _getche(void);
