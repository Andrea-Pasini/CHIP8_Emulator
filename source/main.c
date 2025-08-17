#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"

#define SLEEP_TIME 16666

int main( int argc , char** argv )
{
    // loads the font and the ROM in the RAM
    ram_init( )           ;
    load_rom( argv[1] )   ;

    // initializes the stack
    stack_init( )         ;

    uint16_t inst ;

    // while the machine is on
    while( 1 )
    {
        // fetches the instruction from the ROM
        inst = if_st(  )     ;

        // decodes and executes the instruction  
        id_exe_st( inst )    ;

        // delay introduced to make the emulator run at 60 fps
        usleep( SLEEP_TIME ) ;
    }
}