// Project includes

#include <stdlib.h>
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"



// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
void init_maze_map ();
int main ();
int go_up();
int go_down();
int go_left();
int go_right();
int mapNum = 0;
int atStartPage = 1;
int spokeWithNPC = 0;
int atMenuPage = 0;
int atMaze = 0;
int canSearchTreasure = 0;
int level = 0;
int wonGame = 0;
int died = 0;
void speak_with_NPC(int npc_ID);
void choose_level();
void wait_for_button2_pressed();
void display_menu();
void wait_for_button3_pressed();

int using_sonar = 0;
int foundTreasure = 0;
int sonarUses = 0;
int treasureX = 0;
int treasureY = 0;



/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    // You can add other properties for the player here
    int hasSonar;
    int hasTreasure;
    int canEnterMaze;
    int omnipotentMode;
    int gameFinished;
    int canContinue;
    int hasSword;
    int livesCount;
    int hasTreasureKey;
    

} Player;



/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define OMNIPOTENT_MODE 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define START_GAME 7
#define MENU_BUTTON 8

char* mushroomGuySpeak = "WELCOME TO GT LEGEND!To beat this game, you have to get to a maze, find the sonar, and then find the treasure. For more instructions, speak to Blocky at (11, 11). GOOD LUCK!\n\n\n\n\n\n";
char* oldHagSpeak = "CONGRATULATIONS! You can now go   find the ladder  at (48, 48) to get to the maze and find sonar. However, you have to talk to the Pacman first before entering the maze. GOOD LUCK!\n\n\n\n\n ";
char* sonarMessage = "CONGRATULATIONS! You now have the sonar. Now use the blue key at (1, 4) to get back to the main map and find the treaasure. GOOD LUCK!\n\n\n\n\n\n\n";
char* sonarHint = "";
char* firstTreasureMessage = "The treasure chest has escaped to a new location...\n\n\n\n\n";
char* secondTreasureMessage = " CONGRATULATIONS!    YOU WON!!\n\n\n\n\n\n\n\n\n\n";
char* fakeTreasureMessage = "Sorry. This is a fake treasure.\n\n\n\n\n\n\n\n\n";
char* pacmanSpeak = "Hello! Yeah...the alien guards are really overbearing! Here's the sword, go kill them first and find the sonar. GOOD LUCK!\n\n\n\n\n\n\n";
char* blockySpeak = "In order  to get to the maze, you need to talk to  the old hag first. She's at location(33, 33). GOOD LUCK!\n\n\n\n\n\n\n";
char* phantomSpeak = "Here's the key for the magic chest! Now, go find it! Your sonar will show its relative direction.\n\n\n\n\n\n\n\n\n\n\\n\n\n\n";
char* zerothTreasureMessage = "YOU DON'T HAVE THE KEY TO OPEN THIS TREASURE! Go to location (47, 47) and find the blue phantom. He will give you the key.\n\n\n\n\n\n\n";
char* sonarLimitReachedMessage = "Your have reached your sonar use limit! Go back to the maze and get another one..\n\n\n\n\n\n\n";

int get_action(GameInputs inputs) {

    if (inputs.ay > 0) {
        if (inputs.ay > 0.3) {
            
            return GO_UP;
        
        }
            
    } else if (inputs.ay < 0){
        if (inputs.ay < -0.3) {
            return GO_DOWN;
            
        }
    
    } 
    if (inputs.ax < 0) {
        if (inputs.ax < -0.3) {  
            
            return GO_LEFT;
        
        }
            
    } else if (inputs.ax > 0) {
        
        if (inputs.ax > 0.3) {
            
            return GO_RIGHT;
        
        }       
    }
   
   if (!inputs.b1){        
        
        return OMNIPOTENT_MODE;
        
    } else if (!inputs.b2) {
    
        return ACTION_BUTTON;
    
    } else if (!inputs.b3) {
        
        return MENU_BUTTON;   
        
    }
    return NO_ACTION;
   
}

void wait_for_button3_pressed() {
    
    GameInputs in = read_inputs();
                
    while (in.b3) {
                    
        in = read_inputs();              
                    
    }  
    
    
}
void wait_for_button2_pressed() {
    
    GameInputs in = read_inputs();
                
    while (in.b2) {
                    
        in = read_inputs();              
                    
    }  
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2

int update_game(int action)
{
    // Save player previous location before updating

    Player.px = Player.x;
    Player.py = Player.y;
    Map* current = get_active_map();
    MapItem* itemBelow = get_south(Player.x, Player.y);
    MapItem* itemAbove = get_north(Player.x, Player.y);
    MapItem* itemRight = get_east(Player.x, Player.y);
    MapItem* itemLeft =  get_west(Player.x, Player.y);
    MapItem* itemHere =  get_here(Player.x, Player.y);

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        
        case GO_UP:
            if (go_up())
                return FULL_DRAW;
            break;
            
        case GO_LEFT:
            if (go_left())
                return FULL_DRAW;
            break;
            
        case GO_DOWN:
             if (go_down())
                return FULL_DRAW;
             break;
             
        case GO_RIGHT:
            if (go_right())
                return FULL_DRAW;
            break;
             
        case OMNIPOTENT_MODE:
            
            if (Player.omnipotentMode = !Player.omnipotentMode) {
                
                return FULL_DRAW;
                
            }
            
            break;
            
       
            
        case ACTION_BUTTON:
        
             if (itemAbove->type == ALIEN && Player.hasSword) {
                
                   
                map_erase(Player.x, Player.y - 1);
                return FULL_DRAW;
            
            
            }
        
            if (Player.canEnterMaze && itemHere->type == LADDER) {
                 
                  destroyHashTable(current->items);
                  set_active_map(1);
                  maps_init();
                  init_maze_map();
                  if (sonarUses > 0)
                    add_key(1, 4);
                  Player.x = 0;
                  Player.y = 18;
                  return FULL_DRAW;
               
                
                
            } else if (itemHere->type == KEY) {
                
                  
                    destroyHashTable(current->items);
                    canSearchTreasure = 1;
                    set_active_map(0); 
                    Player.x = 49;
                    Player.y = 49;     
                    treasureX = rand() % 39;
                    treasureY = rand() % 39;
        
                       
                       
                    for(int i = 0; i < 35; i++){
        
                        add_plant(rand() % 48, rand() % 48);
       
                    }
                    
                    for(int i = 0; i < 30; i++) {
                
                        add_plant(rand() % 48, rand() % 48);
       
                    }
                    
                    add_treasure(treasureX, treasureY);  
                    add_mushroom_guy(1, 1);
                    add_blocky(11, 11);
                    add_phantom(47, 47);
                    add_fake_treasure(44, 44);        
                    add_old_hag(33, 33);
                    add_tree(43, 42);
                    add_star(24, 24);
                    add_star(29, 24);
                    add_star(treasureX - 1, treasureY);
                    add_star(treasureX + 1, treasureY);
                    add_star(treasureX, treasureY - 1);
                    add_ladder(49, 49);
                             
                    
                    return FULL_DRAW;
                
            
            } 
            
            if (itemHere->type == TREASURE) {      
              
                if (!Player.hasTreasureKey) {
                    
                    speech(zerothTreasureMessage);
                    wait_for_button2_pressed();
                    return FULL_DRAW;
                    
                    
                } else if (!foundTreasure) {
                    
                    speech(firstTreasureMessage);
                    wait_for_button2_pressed();
                    map_erase(Player.x, Player.y);
                    treasureX = rand() % 46;
                    treasureY = rand() % 46;       
                    add_treasure(treasureX, treasureY);
                    foundTreasure = 1;
                    return FULL_DRAW;
                
                } else {
                    
                    speech(secondTreasureMessage);
                    wait_for_button2_pressed();         
                    Player.hasTreasure = 1;     
                    wonGame = 1;    
                    return FULL_DRAW;
                    
                }
                
            }
            
           
            if (itemHere->type == FAKE_TREASURE) {
                
                speech(fakeTreasureMessage);
                wait_for_button2_pressed();
                int num = rand() % 5;
                
                if (num == 0) {
                    
                    if (level == 1) {
                         
                        Player.livesCount--;
                        
                    } else if (level == 2) {
                        
                        
                        Player.livesCount-=2;   
                        
                    } else {
                        
                        died = 1;   
                        
                    }
                    
                }
                return FULL_DRAW;    
            
            }
            
            
            if (itemHere->type == SONAR && sonarUses == 0) {
                 
                canSearchTreasure = 0; 
                speak_with_NPC(itemHere->ID);
                sonarUses = 10;
                canSearchTreasure = 1;
                add_key(1, 4);
                wait_for_button2_pressed();
                return FULL_DRAW;
                
            } else if (sonarUses > 1 && itemHere->type == SONAR) {
                
                speech("You already have the sonar... GO LOOK FOR THE TREASURE!");
                wait_for_button2_pressed();
                return FULL_DRAW;        
                
            }
        
            if (itemAbove->type == NPC) {
                
                speak_with_NPC(itemAbove->ID);
                wait_for_button2_pressed();
                return FULL_DRAW;
                
            
                
            } else if(itemBelow->type == NPC) {
                
             
                speak_with_NPC(itemBelow->ID);
                wait_for_button2_pressed();
                return FULL_DRAW;
                
            } else if(itemRight->type == NPC) {
                
              
                speak_with_NPC(itemRight->ID);  
                wait_for_button2_pressed();            
                return FULL_DRAW;
                    
                
            } else if (itemLeft->type == NPC) {
                
            
                speak_with_NPC(itemLeft->ID);
                wait_for_button2_pressed();
                return FULL_DRAW;
          
            } 
            
            if (Player.hasSonar && canSearchTreasure) {
                
             
                speak_with_NPC(3);             
                wait_for_button2_pressed();
                return FULL_DRAW;
                
                
            } 
            
            if (sonarUses == 0 && canSearchTreasure) {
        
                    Player.hasSonar = 0;
                    speech(sonarLimitReachedMessage);
                    wait_for_button2_pressed();
                    return FULL_DRAW;
            
        
             }       
                
            
         
            break;
            
        case MENU_BUTTON: 
            
            display_menu();
            wait_for_button3_pressed();
            return FULL_DRAW;
     
 
        default:break;
    }
    
    return NO_RESULT;
    
}

void display_menu() {
    
    uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
    uLCD.color(GREEN);
    uLCD.textbackground_color(BLACK);
    uLCD.locate(7, 1);
    uLCD.printf("MENU");
    uLCD.color(WHITE);
    uLCD.locate(1, 6);
    uLCD.printf("Press the third   button from the    top button to      continue and     navigate through     the menu");
    
    
    wait_for_button3_pressed();
    
    
    uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
    
    uLCD.color(GREEN);
    uLCD.textbackground_color(BLACK);
    uLCD.locate(0, 0);
    uLCD.printf("Go LEFT:\nTilt Left\n");
    uLCD.printf("Go RIGHT:\nTilt Right\n");
    uLCD.printf("Go UP:\nTilt Forward\n");
    uLCD.printf("Go DOWN:\nTilt Backward\n");
    
    uLCD.color(BLUE);
    uLCD.printf("\nButton 1:\nTop Button\n");
    uLCD.printf("Button 2:\nBelow Button 1\n");
    uLCD.printf("Button 3:\nBelow Button 2\n");
    
    wait_for_button3_pressed();
    
    uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
    uLCD.color(WHITE);
    uLCD.textbackground_color(BLACK);
    uLCD.locate(0, 0);
    uLCD.printf("Position: (%d, %d)\n", Player.px, Player.py);
    uLCD.printf("\nSword: %d\n ", Player.hasSword);
    uLCD.printf("\nSonar Uses:%d\n ", sonarUses);
    uLCD.printf("\nLives:%d\n ", Player.livesCount);
    
        
    
}

void speak_with_NPC(int npcID) {
    
    if (npcID == 1) {
        
        speech(mushroomGuySpeak);
        return;
    
    } else if (npcID == 2) {
        
        speech(oldHagSpeak);
        Player.canContinue = 1;
        add_ladder(49, 49);

    } else if (npcID == 4){
        
        speech(pacmanSpeak);
        Player.hasSword = 1;
    
    
    } else if (npcID == 5) {
    
        speech(blockySpeak);
    
    
    } else if (npcID == 6) {
        
        speech(phantomSpeak);
        Player.hasTreasureKey = 1;
        
    } else if (npcID == 3) {
        
        if (!canSearchTreasure) {
            
            speech(sonarMessage);
            Player.hasSonar = 1;
            add_key(1, 4);
               
        } else if (sonarUses > 0){
            
            if (treasureX - Player.x > 0 && treasureY - Player.y > 0) {
                
                sonarHint = "    Treasure at      SouthEast";   
                
            } else if (treasureX - Player.x > 0 && treasureY - Player.y < 0) {
                
                
                sonarHint = "    Treasure at      NorthEast";  
                
            
            } else if (treasureX - Player.x < 0 && treasureY - Player.y < 0) {
            
            
                sonarHint = "    Treasure at      NorthWest";
                
            
            } else if (treasureX - Player.x < 0 && treasureY - Player.y > 0){
                
                
                sonarHint = "    Treasure at      SouthWest";   
                
            }  else if (treasureX - Player.x < 0){
                
                
                sonarHint = "    Treasure at      WestWest";  
                
            } else if (treasureX - Player.x > 0) {
                
                 
                sonarHint = "    Treasure at      EastEast";  
                
            } else if (treasureY - Player.y > 0) {
                
                sonarHint = "    Treasure at      SouthSouth";  
                
            } else {
                        
                sonarHint = "    Treasure at     NorthNorth"; 
           
            }
            
            speech(sonarHint);   
            sonarUses--;
            return;
            
        } 
        
    }
}

int go_up() {

   MapItem* itemAbove = get_north(Player.x, Player.y);
    
   if ((itemAbove->walkable && Player.y != 0) || Player.omnipotentMode) {

        Player.y -= 1;
        
         if (itemAbove->type == ENEMY) {
            
            Player.livesCount--;   
            
        } else if (itemAbove->type == LADDER) {
            
            if (Player.canContinue) {
                
                Player.canEnterMaze = 1;
                
            } else if (Player.hasSonar) {
                
                canSearchTreasure = 1;   
                
            }
            
        }
            
            
        return 1;

    } 
    
    return 0;

}

int go_down() {

    MapItem* itemBelow = get_south(Player.x, Player.y);
    
    if ((itemBelow->walkable && Player.y !=  map_height() - 1) || Player.omnipotentMode) {

        Player.y += 1;
        
         if (itemBelow->type == ENEMY) {
            
             Player.livesCount--;   
            
        } else if (itemBelow->type == LADDER) {
               
            if (Player.canContinue) {
            
                Player.canEnterMaze = 1;
            
            } else if (Player.hasSonar) {
                
                canSearchTreasure = 1;   
                
            }
            
        }
        
        return 1;

    } 

    return 0;

}

int go_left() {

    MapItem* itemLeft = get_west(Player.x, Player.y);
    
    if ((itemLeft->walkable && Player.x != 0) || Player.omnipotentMode) {

        Player.x -= 1;
        
         if (itemLeft->type == ENEMY) {
            
             Player.livesCount--;   
            
        } else if (itemLeft->type == LADDER) {
               
            if (Player.canContinue) {
                Player.canEnterMaze = 1;
                
            } else if (Player.hasSonar) {
                
                canSearchTreasure = 1;   
                
            } else if (itemLeft -> type == TREE) {
                
                
        
            }
            
        }
        
        return 1;

    }

    return 0;

}


int go_right() {

    MapItem* itemRight = get_east(Player.x, Player.y);
    
    if ((itemRight->walkable && Player.x != map_width() - 1) || Player.omnipotentMode) {

        Player.x += 1;
        
        if (itemRight->type == ENEMY) {
            
             Player.livesCount--;   
            
        } else if (itemRight->type == LADDER) {
               
            if (Player.canContinue) {
                
                Player.canEnterMaze = 1;
            
            } else if (Player.hasSonar) {
                
                canSearchTreasure = 1;   
                
            }
        } 
        
        return 1;

    }

    return 0;

}
/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    MapItem* itemHere = get_here(Player.x, Player.y);
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                if (itemHere->type == TREE) {
                 
                    draw_tree(u, v);   
                    
                } else {
                    
                    draw_player(u, v, Player.has_key);
                    
                }
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status(Player.x, Player.y, Player.livesCount);
    draw_lower_status(Player.omnipotentMode);
}

void choose_level() {
    
    GameInputs in = read_inputs();
    uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
    
    while (!level) {
        
        uLCD.text_bold(TEXTBOLD);
        uLCD.textbackground_color(BLACK);
        uLCD.color(GREEN);
        uLCD.locate(3, 1);
        uLCD.printf("PLEASE CHOOSE       YOUR LEVEL!" );  
        uLCD.color(BLUE);
        uLCD.locate(1, 7);
        uLCD.printf("Button 1: Level 1 Button 2: Level 2 Button 3: Level 3");       
        in = read_inputs();
        
        if (!in.b1) {
            
            level = 1;  
            Player.livesCount = 8;
            
        } else if (!in.b2) {
            
            level = 2;       
            Player.livesCount = 4; 
            
        } else if(!in.b3) {
            
            level = 3;
            Player.livesCount = 2;
            
        }
    }
}

void draw_start_page() {
 
    GameInputs in = read_inputs();
    
    uLCD.filled_rectangle(0, 0, 130, 130, BLACK);

    while (in.b1 && in.b2 && in.b3) {
        
        uLCD.text_bold(TEXTBOLD);
        uLCD.textbackground_color(BLACK);
        uLCD.color(GREEN);
        uLCD.locate(4, 1);
        uLCD.printf("WELCOME TO \n    GT LEGEND!" );  
        uLCD.color(BLUE);
        uLCD.locate(3, 7);
        uLCD.printf("Please press\n    any button \n   to continue..");       
        in = read_inputs();
    
    }
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    // "Random" plants
    srand(time(NULL));
    
    if (!canSearchTreasure) {
        
        draw_start_page();
        choose_level();
        
    }

   
    for(int i = 0; i < 35; i++){
        
        add_plant(rand() % 48, rand() % 48);
       
    }
                    
    for(int i = 0; i < 30; i++) {
                
        add_plant(rand() % 48, rand() % 48);
       
    }
                    
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_tree(4, 4);
    add_mushroom_guy(2, 2);
    add_old_hag(33, 33);
    add_blocky(11, 11);
    
    
    pc.printf("Walls done!\r\n");


    print_map();
}
    

void init_maze_map() {
    
    
    pc.printf("Maze Bug");
    
     int mazeMap[10][10] = {
                            {1, 1, 1, 0, 0, 1, 1, 1, 1, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                            {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                            {1, 1, 0, 1, 1, 0, 1, 1, 1, 1},
                            {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
                            {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
                            {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                            {1, 1, 1, 1, 1, 1, 0, 0, 1, 1}
                                                            };
    
    if (level == 1) {
       
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++) {
                if(mazeMap[i][j] == 1) {
                    add_plant(i, j);       
            }
        }
    }                                 
                    
                                                            
                                                            
    } else if (level == 2) {
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++) {
                if(mazeMap[i][j] == 1) {
                    add_star(i, j);       
                }
            }
        }
          
    } else if (level == 3) {
        
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++) {
                if(mazeMap[i][j] == 1) {
                    add_star(i, j);       
                }
            }
        
        }   
    
    } 
    
    pc.printf("Maze Bug");
    


    
    
    add_sonar(8, 3);
    add_alien_guard(3, 10);
    add_alien_guard(2, 10);
    add_pacman(3, 11);
   
    
    pc.printf("Maze Bug");

    add_star(0, 13);
    print_map();

    
}



/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
 
 
int main()
{
    
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    

    Player.hasSonar = 0;
    Player.hasTreasure = 0;
    Player.canEnterMaze = 0;
    Player.omnipotentMode = 0;
    Player.gameFinished = 0;
    Player.canContinue = 0;

    
    // Main game loop
      
    pc.printf("Maze Bug!\r\n");
     maps_init();
    init_main_map();
    set_active_map(0);
    Player.x = Player.y = 1;
    draw_game(true);

    while(!wonGame && !died) {
            // Timer to measure game update speed
            
             Timer t; t.start();
            // Actuall do the game update:
            // 1. Read inputs
            GameInputs in = read_inputs();

            // 2. Determine action (get_action)
            int current_action = get_action(in);
            // 3. Update game (update_game)
            int current_update = update_game(current_action);
            // 3b. Check for game over

            // 4. Draw frame (draw_game)
            draw_game(current_update);
            t.stop();
            int dt = t.read_ms();
            if (dt < 100) wait_ms(100 - dt);
            if (Player.livesCount == 0) {
                
                died = 1;
                
            }
            
        
    }
    
     uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
     uLCD.text_bold(TEXTBOLD);
     uLCD.textbackground_color(BLACK);
     uLCD.color(WHITE);
     uLCD.locate(3, 6);
     uLCD.printf("GAME OVER!!!");
     
     
     if (wonGame) {
     
         uLCD.color(GREEN);
         uLCD.locate(3, 8);
         uLCD.printf("  YOU WON!!");
     
     } else {
         
         uLCD.color(RED);
         uLCD.locate(3, 8);
         uLCD.printf("  YOU LOST...");
         
         
    }
}
