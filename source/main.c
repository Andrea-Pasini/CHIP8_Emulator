/*
        CHIP-8 EMULATOR
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "cpu.h"
#include "display.h"
#include "input.h"

#define ERROR      2
#define TERMINATED 0
#define RUNNING    1

/*********************************************************/
/*********************************************************/

extern win_ren_t window         ;
extern uint16_t  SND_TIMER      ;
extern uint16_t  DEL_TIMER      ;
extern uint8_t   keys[ 16 ]     ;
extern uint8_t   emulator_state ;

/*********************************************************/
/*********************************************************/


int main( int argc , char** argv )
{
    /*
        The first part of the program creates
        the window, then initializes the emulator
        and loads the rom which name is found in 
        argv[ 1 ].
    */
    
    // creates the window and handles related errors
    if ( !create_window( 640 , 320 , 0 )  ) 
        exit( ERROR )                             ;

    // destroys the window when the program ends
    atexit     ( destroy_window )                 ;
    
    // prepares the emulator to run a game
    init_chip8 ( rom_selection( argv[1] ) )       ;


    // initializes the timestamps used to 
    // allow 60 Hz drawing 
    int past_timestamp = SDL_GetTicks( )          ; 
    int curr_timestamp                            ;


    /*
        The main cycle starts executing batches
        of 10 instructions, then checks if the 
        display needs to be redrawn. This until 
        the window gets closed. 
    */

    while( emulator_state == RUNNING )
    {
        // handles game-inputs and window related events
        input_handling( ) ;

        // executes a batch of instructions
        for ( int i = 0 ; i < 10 ; i++ ) 
        {
            id_exe_st( if_st( ) )             ;
            if ( DEL_TIMER > 0 ) DEL_TIMER -- ; 
            if ( SND_TIMER > 0 ) SND_TIMER -- ;
        }
        
        // forces 60 Hz drawing
        curr_timestamp = SDL_GetTicks( )      ; 

        if ( curr_timestamp - past_timestamp >= 16 )
        {
            draw_texture( )                   ;
            past_timestamp += 16              ; 
        }

    }

    // termination 
    exit( TERMINATED ) ;
    
}

