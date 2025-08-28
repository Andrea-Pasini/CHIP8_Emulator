
#ifndef DISPLAY_H
#define DISPLAY_H

// custom type that combines a window and its renderer
typedef struct wr
{
    SDL_Window*   win ;
    SDL_Renderer* ren ;
    SDL_Texture*  tex ;
}   win_ren_t         ;


int  create_window     ( int width , int height , uint32_t flags ) ;
void destroy_window    (                                         ) ; 
void clear_window      (                                         ) ;

#endif
