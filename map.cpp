#include "map.h"

#include "globals.h"
#include "graphics.h"
#define NUM_BUCKETS 50
/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */


/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map[2];
static int active_map = 0;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    return Y * (map[active_map].w) + X;

}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
    return key % NUM_BUCKETS;
}

void maps_init()
{
    // TODO: Implement!
    // Initialize hash table
    // Set width & height
    map[0].items = createHashTable(&map_hash, NUM_BUCKETS);
    map[1].items = createHashTable(&map_hash, NUM_BUCKETS);
    
    if (active_map == 0) {
    
         map[active_map].w = 50;
         map[active_map].h = 50;
        
    } else {
        
        map[active_map].w = 20;
        map[active_map].h = 20;
    
    }
}

Map* get_active_map()
{
    // There's only one map
    return &(map[active_map]);
}

Map* set_active_map(int m)
{
    active_map = m;
    return &(map[active_map]);
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return map[active_map].w;
}

int map_height()
{
    return map[active_map].h;
}

int map_area()
{
    return (map[active_map].w * map[active_map].h);
}

MapItem* get_north(int x, int y)
{
    MapItem* itemNorth = get_here(x, y - 1);

    return itemNorth;
}

MapItem* get_south(int x, int y)
{
    MapItem* itemSouth = get_here(x, y + 1);

    return itemSouth;
}

MapItem* get_east(int x, int y)
{
    MapItem* itemEast = get_here(x + 1, y);

    return itemEast;
}

MapItem* get_west(int x, int y)
{
    MapItem* itemWest = get_here(x - 1, y);

    return itemWest;
}

MapItem* get_here(int x, int y)
{
    MapItem* itemHere = (MapItem*) getItem(map[active_map].items, XY_KEY(x, y));

    return itemHere;
}


void map_erase(int x, int y)
{
    deleteItem(map[active_map].items, XY_KEY(x, y));
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        w1->can_get = false;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_pacman(int x, int y) {
     
    MapItem* pacman = (MapItem*) malloc(sizeof(MapItem));
    pacman->type = NPC;
    pacman->ID = 4;
    pacman->draw = draw_pacman;
    pacman->walkable = false;
    pacman->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), pacman);
    if (val) free(val); // If something is already there, free it
    

}

void add_blocky(int x, int y) {
    
    MapItem* blocky = (MapItem*) malloc(sizeof(MapItem));
    blocky->type = NPC;
    blocky->ID = 5;
    blocky->draw = draw_blocky;
    blocky->walkable = false;
    blocky->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), blocky);
    if (val) free(val); // If something is already there, free it
    
     
}

void add_alien_guard(int x, int y) {
    
    MapItem* alien_guard = (MapItem*) malloc(sizeof(MapItem));
    alien_guard->type = ALIEN;
    alien_guard->draw = draw_alien_guard;
    alien_guard->walkable = false;
    alien_guard->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), alien_guard);
    if (val) free(val); // If something is already there, free it   
    
}

void add_phantom(int x, int y) {
    
    MapItem* phantom = (MapItem*) malloc(sizeof(MapItem));
    phantom->type = NPC;
    phantom->ID = 6;
    phantom->draw = draw_phantom;
    phantom->walkable = true;
    phantom->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), phantom);
    if (val) free(val); // If something is already there, free it
    
}

void add_treasure(int x, int y) {
    
    MapItem* treasure = (MapItem*) malloc(sizeof(MapItem));
    treasure->type = TREASURE;
    treasure->draw = draw_treasure_chest;
    treasure->walkable = true;
    treasure->data = (void*) "TREASURE";
    treasure->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), treasure);
    if (val) free(val); // If something is already there, free it
    
}


void add_fake_treasure(int x, int y) {
    
    MapItem* treasure = (MapItem*) malloc(sizeof(MapItem));
    treasure->type = FAKE_TREASURE;
    treasure->draw = draw_fake_treasure;
    treasure->walkable = true;
    treasure->data = (void*) "FAKE_TREASURE";
    treasure->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), treasure);
    if (val) free(val); // If something is already there, free it
    
}


void add_key(int x, int y) {
    
    MapItem* key = (MapItem*) malloc(sizeof(MapItem));
    key->type = KEY;
    key->draw = draw_key;
    key->walkable = true;
    key->data = (void*) "KEY";
    key->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), key);
    if (val) free(val); // If something is already there, free it
    
}

void add_tree(int x, int y) {
    
    MapItem* tree = (MapItem*) malloc(sizeof(MapItem));
    tree->type = TREE;
    tree->draw = draw_tree;
    tree->walkable = true;
    tree->data = (void*) "TREE";
    tree->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), tree);
    if (val) free(val); // If something is already there, free it
     
}

void erase(int x, int y) {
    
    MapItem* ERASER = (MapItem*) malloc(sizeof(MapItem));
    ERASER->draw = draw_eraser;
    ERASER->walkable = true;
    ERASER->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), ERASER);
    if (val) free(val); // If something is already there, free it
 
}
void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    w1->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_mushroom_guy(int x, int y) {

    MapItem* theMushroomGuy = (MapItem*) malloc(sizeof(MapItem));
    theMushroomGuy->type = NPC;
    theMushroomGuy->draw = draw_mushroom_guy;
    theMushroomGuy->walkable = false;
    theMushroomGuy->ID = 1;
    theMushroomGuy->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), theMushroomGuy);
    if (val) free(val);

}

void add_star(int x, int y) {

    MapItem* theStar = (MapItem*) malloc(sizeof(MapItem));
    theStar->type = ENEMY;
    theStar->draw = draw_yellow_star;
    theStar->walkable = true;
    theStar->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), theStar);
    if (val) free(val);

}

void add_sonar(int x, int y) {
    
    MapItem* theSonar = (MapItem*) malloc(sizeof(MapItem));
    theSonar->type = SONAR;
    theSonar->draw = draw_sonar;
    theSonar->ID = 3;
    theSonar->walkable = true;
    theSonar->can_get = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), theSonar);
    if (val) free(val);  
    
    
}

void add_old_hag(int x, int y) {
    
    MapItem* theOldHag = (MapItem*) malloc(sizeof(MapItem));
    theOldHag->type = NPC;
    theOldHag->draw = draw_old_hag;
    theOldHag->walkable = false;
    theOldHag->can_get = false;
    theOldHag->ID = 2;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), theOldHag);
    if (val) free(val);
    
}

void add_ladder(int x, int y) {
    
    MapItem* ladder = (MapItem*) malloc(sizeof(MapItem));
    ladder->type = LADDER;
    ladder->draw = draw_ladder;
    ladder->walkable = true;
    ladder->can_get = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), ladder);
    if (val) free(val);
    
}