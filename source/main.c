/*

    Right now, the emulator automatically plays the chip8-logo.ch8 demo, 
    a small program written by Timendus that draws a logo on the screen. 
    This is due to the fact that the cpu module is not fully debugged 
    yet and other programs could crash/not work properly.

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

#define ERROR      2
#define TERMINATED 0
#define RUNNING    1

extern win_ren_t window ;

int main( int argc , char** argv )
{
    // creates the window and handles related errors
    if ( !create_window( 1000 , 1000 , 0 )  ) exit( ERROR ) ;

    // makes the window pitch black
    clear_window( )                                       ;

    // destroys the window when the program ends
    atexit( destroy_window )                              ;
    
    // creates the random number generator seed
    srand( time( NULL ) )                                 ;
    
    // initializes the emulator
    ram_init( )                                                    ;
    load_rom( "/home/andrea/Projects/CHIP-8/roms/chip8-logo.ch8" ) ;
    stack_init( )                                                  ;

    int       emulator_state = RUNNING ;
    SDL_Event event                    ;
    uint16_t  inst                     ;

    // main loop
    while( emulator_state == RUNNING )
    {
        // temporary event handling
        while( SDL_PollEvent( &event ) )
        {
            // handles the termination input
            if ( event.type == SDL_QUIT ) emulator_state = TERMINATED ;
        }

        // pipeline
        id_exe_st( if_st( ) ) ;

        // forces 60 Hz execution
        SDL_Delay( 16       ) ;
    }

    // termination 
    exit( TERMINATED ) ;
    
}

