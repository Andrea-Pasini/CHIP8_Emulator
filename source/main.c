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
#include <locale.h> // remove when the GUI will be implemented

#include "cpu.h"

#define SLEEP_TIME 16667

int main( int argc , char** argv )
{
    // enables utf-8 characters
    setlocale(LC_CTYPE, "");  // attiva UTF-8
    
    // creates the random number generator seed
    srand( time( NULL ) ) ;
    
    // loads the font and the ROM in the RAM
    ram_init( )           ;
    load_rom( "/home/andrea/Projects/CHIP-8/roms/chip8-logo.ch8" )   ;

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