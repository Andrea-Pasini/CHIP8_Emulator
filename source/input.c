
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "cpu.h"

#define RUNNING         1
#define NOT_IN_KEYPAD (-1)
#define TERMINATED      0

extern uint8_t keys[ 16 ]        ;
SDL_Event event                  ;
uint8_t emulator_state = RUNNING ;

// maps every key to a chip-8 button
uint8_t map_key( uint32_t sdl_key )
{
    switch( sdl_key )
    {
        // first row of the keypad
        case SDLK_1: return 1   ;

        case SDLK_2: return 2   ;

        case SDLK_3: return 3   ;

        case SDLK_4: return 12  ;

        // second row of the keypad
        case SDLK_q: return 4   ;

        case SDLK_w: return 5   ;

        case SDLK_e: return 6   ;

        case SDLK_r: return 13  ;

        // third row of the keypad
        case SDLK_a: return 7  ;

        case SDLK_s: return 8  ;

        case SDLK_d: return 9  ;

        case SDLK_f: return 14 ;

        // fourth row of the keypad
        case SDLK_z: return 10 ;

        case SDLK_x: return 0  ;

        case SDLK_c: return 11 ;

        case SDLK_v: return 15 ;

        // out of the keypad
        default    : return NOT_IN_KEYPAD ; 


    }
}


// handles game-inputs and window related events
void input_handling( void )
{

    // event handling
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
}

char* rom_selection( char* program_name )
{
    // creates the path
    static char path[ 10 ] = "./roms/"  ;
    strcat( path , program_name )       ; 
    strcat( path , ".ch8"       )       ;

    return  path                        ;
}  