
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "display.h"

#define DFT_GPU (-1)

win_ren_t window = { NULL , NULL , NULL }    ;

// returns the pointer to an SDL window and renderer
int create_window( int width , int height , uint32_t flags )
{
    
    uint32_t init_flags = SDL_INIT_VIDEO                                       ;
    uint32_t wind_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE              ;
    uint32_t rend_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ;

    // declares the retval and initializes SDL2
    if ( SDL_Init( init_flags ) != 0 ) return 0 ;

    // creates a window
    window.win = SDL_CreateWindow(   
                                    "CHIP-8 EMULATOR"       , 
                                    SDL_WINDOWPOS_UNDEFINED , 
                                    SDL_WINDOWPOS_UNDEFINED , 
                                    width                   , 
                                    height                  , 
                                    wind_flags 
    )                                                       ;
    
    // window creation error handling
    if ( window.win == NULL ) 
    {
        fprintf( stderr, "SDL failed: %s\n", SDL_GetError() ) ;
        return 0                                              ;
    }

    // creates a renderer for the window
    window.ren = SDL_CreateRenderer(
                                        window.win ,
                                        DFT_GPU    ,
                                        rend_flags      
    )                                              ;
    
    // renderer creation error handling
    if ( window.ren == NULL ) 
    {
        fprintf( stderr, "SDL failed: %s\n", SDL_GetError() ) ;
        return 0                                              ;
    }

    // sets the scale
    SDL_RenderSetLogicalSize( window.ren , 64 , 32 ) ;
    
    // creates a texture
    window.tex = SDL_CreateTexture(
                                        window.ren                  ,
                                        SDL_PIXELFORMAT_RGBA8888    ,
                                        SDL_TEXTUREACCESS_STREAMING ,
                                        64                          ,
                                        32
    )                                                               ;

    if ( window.tex == NULL )
    {
        fprintf( stderr, "SDL failed: %s\n", SDL_GetError() ) ;
        return 0                                              ; 
    }

    return 1 ;
}


// destroys every subsystem created
void destroy_window( )
{
    SDL_DestroyRenderer( window.ren ) ;
    SDL_DestroyWindow  ( window.win ) ;
    SDL_Quit           (            ) ;

}


// fills the window with a black rectangle
void clear_window( void )
{
    // sets the drawing color to #000000
    SDL_SetRenderDrawColor( window.ren , 0 , 0 , 0xFF , 1 ) ;

    // fills the buffer with #000000
    SDL_RenderClear       ( window.ren                    ) ; 

    // fills the render with a rectangle
    SDL_RenderFillRect    ( window.ren , NULL             ) ;

    // draws the buffer into the window
    SDL_RenderPresent     ( window.ren                    ) ;

    return                                                  ;
}

