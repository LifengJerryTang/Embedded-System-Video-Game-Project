#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.filled_rectangle(0, 5 , 11, 11, BLACK); //Draw a white rectangular speech box

}

void erase_speech_bubble()
{

    uLCD.filled_rectangle(0, 5 , 14, 14, BLACK);


}

void draw_speech_line(const char* line) {

    uLCD.text_bold(TEXTBOLD);
    uLCD.color(BLUE);
    int x = 0;
    int y = 3;
    int length = strlen(line);
    int j = 0;

    while(j < length && *line) {
    
        uLCD.locate(x, y);
       
          
        for (int i = 0; i < 17; i++) {
          
             uLCD.printf("%c", *line);
             line++;
             j++;
             
             if (*line == ' ' && i == 14) {
              
                break;
              
              }
              
            if (*line == ' ') {
                
                if (find_remaining_length(line) > (17 - i)) {
                    
                    break;   
                    
                }
                
            }
            
            

        }
        
        y++;
        
    }
}


int find_remaining_length(const char* words) {
    
    int remaining_length = 0;
    if (*words == ' ')
        words++;
    
    while(*words != ' ') {
        
        remaining_length++;
        words++;
        
    }
    
    return remaining_length;
    
} 

void speech_bubble_wait()
{
 
    wait_ms(200);   
}

void speech(const char* line)
{
    draw_speech_bubble();
    draw_speech_line(line);
    speech_bubble_wait();
    erase_speech_bubble();
}
