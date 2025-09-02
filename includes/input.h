#ifndef INPUT_H
#define INPUT_H


#define NOT_IN_KEYPAD (-1)

uint8_t map_key         ( uint32_t sdl_key ) ;
void    input_handling  ( void             ) ;
char* rom_selection( char* program_name ) ;



#endif
