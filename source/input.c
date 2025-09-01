
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "input.h"

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