
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

extern win_ren_t window       ;
extern uint16_t  SND_TIMER    ;
extern uint16_t  DEL_TIMER    ;
extern uint8_t   keys[ 16 ]   ;

int main( int argc , char** argv )
{
    // creates the window and handles related errors
    if ( !create_window( 640 , 320 , 0 )  ) exit( ERROR ) ;

    // makes the window monochrome
    clear_window( )                                       ;

    // destroys the window when the program ends
    atexit( destroy_window )                              ;
    
    // creates the random number generator seed
    srand( time( NULL ) )                                 ;
    
    // prepares the emulator to run a game
    init_chip8( "/home/andrea/Projects/CHIP-8/roms/Soccer.ch8" ) ;
    
    int       past_timestamp , curr_timestamp ;
    int       emulator_state = RUNNING        ;
    SDL_Event event                           ;

    past_timestamp = SDL_GetTicks( )          ; 

    // main loop
    while( emulator_state == RUNNING )
    {

        // temporary event handling
        while( SDL_PollEvent( &event ) )
        {
            // handles the termination input
            if (   event.type == SDL_QUIT ) emulator_state = TERMINATED ;

            // resizes the window
            else if ( ( event.type         == SDL_WINDOWEVENT         ) && 
                        event.window.event == SDL_WINDOWEVENT_RESIZED )
            {
                draw_texture( ) ;
            }
            
            // increases a counter each time a keypress is detected
            else if ( event.type == SDL_KEYDOWN ) 
            {
                int key = map_key( event.key.keysym.sym ) ;
                //printf( "%d\n", key ) ;
                if( key != NOT_IN_KEYPAD ) keys[ key ]++  ;
            }

            else if (event.type == SDL_KEYUP) 
            {  
                int key = map_key(event.key.keysym.sym);
                
                if (key != NOT_IN_KEYPAD) 
                {
                    keys[key] = 0;  // Set to 0 when released
                }
            }

        }

        // executes a batch of instructions
        for ( int i = 0 ; i < 10 ; i++ ) 
        {
            id_exe_st( if_st( ) )             ;
            if ( DEL_TIMER > 0 ) DEL_TIMER -- ; 
            if ( SND_TIMER > 0 ) SND_TIMER -- ;
        }

        
        // forces 60 Hz drawing
        curr_timestamp = SDL_GetTicks( )    ; 

        if ( curr_timestamp - past_timestamp >= 16 )
        {
            draw_texture( )                 ;
            past_timestamp = curr_timestamp ; 
        }

    }

    // termination 
    exit( TERMINATED ) ;
    
}

