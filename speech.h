#ifndef SPEECH_H
#define SPEECH_H

/**
 * Display a speech bubble.
 */
void speech(const char* line);

/**
 * Display a long speech bubble (more than 2 lines).
 *
 * @param lines The actual lines of text to display
 * @param n The number of lines to display.
 */
void long_speech(const char* lines[], int n);

/**
 * @Return the remaining length of a sentence.
 * The purpose of this function is to make sure that the rest of a sentence
   starts at a new line if it gets too long
 */
int find_remaining_length(const char* words);

#endif // SPEECH_H
