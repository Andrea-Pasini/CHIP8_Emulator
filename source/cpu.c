/*
*
*   HARDWARE SIMULATION MODULE
*
*/

/* TO-DO NEXT: fetch, decode, and execute and other initialization functions ( i.e. registers ) */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cpu.h"

/**********************************************************/
/**********************| MACROS |**************************/


// macros: hardware
#define RAM_SIZE 4096
#define REG_NUM  16
#define STK_SIZE 256

// macros: execution
#define ENTRY_PT 0x200

// macros: booleans
#define TRUE     1
#define FALSE    0

// macros: display
#define SCR_W    64
#define SCR_H    32
#define CLR_DSP  "\033c"//"\033[2J\033[H" 
#define ON       1
#define OFF      0

/********************************************************************/
/**********************| GLOBAL_VARIABLES |**************************/


// hardware components ( as global variables )
stack_t   STK             ;
uint16_t  PC              ;
uint8_t   SND_TIMER       ;
uint8_t   DEL_TIMER       ;
uint8_t   RAM[ RAM_SIZE ] ;
uint8_t   REG[ REG_NUM  ] ;
uint16_t  I_REG           ;
display_t DSP             ;

// opcode tables ( needed to discriminate between different instructions )
opcode_t OPCXX[  16 ]   ;
opcode_t OPC0X[ 0xE ]   ;
opcode_t OPC8X[ 0xE ]   ;
opcode_t OPCEX[ 0xE ]   ;
opcode_t OPCFX[ 0xE ]   ;
opcode_t OPCF5[ 0x3 ]   ;


/*********************************************************/
/**********************| STACK |**************************/


// checks whether the stack is empty
int8_t stack_empty( void )
{
    int8_t ret = ( STK.sp == 0 ) ? TRUE : FALSE ;
    return ret                                  ;
}


// initializes the stack as empty
void stack_init( void ) 
{
    STK.sp = 0                            ;
    memset( STK.cells , 0x00 , STK_SIZE * sizeof( uint16_t ) ) ;
    return                                ;
}


// pushes a value in the stack
void stack_push( uint16_t val ) 
{
    // in case of overflow it doesn't signal it
    if ( STK.sp < STK_SIZE ) STK.cells[ STK.sp++ ] = val ;
    return                                               ; 
} 


// returns the value on top of the stack
uint16_t stack_pop ( void ) 
{
    return STK.cells[ STK.sp-- ] ; 
}


/*********************************************************/
/**********************|  RAM  |**************************/


// initializes the RAM
void ram_init( void )
{
    load_font( )    ;
    load_opc( )     ;
    PC = ENTRY_PT   ; 
}


// loads the chip-8 standard font in the RAM
void load_font( void )
{
    RAM[ 0x050 ] = 0xF0 ; RAM[ 0x051 ] = 0x90 ; RAM[ 0x052 ] = 0x90 ; RAM[ 0x053 ] = 0xF0 ; RAM[ 0x054 ] = 0xF0 ; // 0
    RAM[ 0x055 ] = 0x20 ; RAM[ 0x056 ] = 0x60 ; RAM[ 0x057 ] = 0x20 ; RAM[ 0x058 ] = 0x20 ; RAM[ 0x059 ] = 0x70 ; // 1
    RAM[ 0x05A ] = 0xF0 ; RAM[ 0x05B ] = 0x10 ; RAM[ 0x05C ] = 0xF0 ; RAM[ 0x05D ] = 0x80 ; RAM[ 0x05E ] = 0xF0 ; // 2
    RAM[ 0x05F ] = 0xF0 ; RAM[ 0x060 ] = 0x10 ; RAM[ 0x061 ] = 0xF0 ; RAM[ 0x062 ] = 0x10 ; RAM[ 0X063 ] = 0xF0 ; // 3

    RAM[ 0x064 ] = 0x90 ; RAM[ 0x065 ] = 0x90 ; RAM[ 0x066 ] = 0xF0 ; RAM[ 0x067 ] = 0x10 ; RAM[ 0x068 ] = 0x10 ; // 4
    RAM[ 0x069 ] = 0xF0 ; RAM[ 0x06A ] = 0x90 ; RAM[ 0x06B ] = 0xF0 ; RAM[ 0x06C ] = 0x10 ; RAM[ 0x06D ] = 0xF0 ; // 5
    RAM[ 0x06E ] = 0xF0 ; RAM[ 0x06F ] = 0x80 ; RAM[ 0x070 ] = 0xF0 ; RAM[ 0x071 ] = 0x90 ; RAM[ 0x072 ] = 0xF0 ; // 6
    RAM[ 0x073 ] = 0xF0 ; RAM[ 0x074 ] = 0x10 ; RAM[ 0x075 ] = 0x20 ; RAM[ 0x076 ] = 0x40 ; RAM[ 0x077 ] = 0x40 ; // 7

    RAM[ 0x078 ] = 0xF0 ; RAM[ 0x079 ] = 0x90 ; RAM[ 0x07A ] = 0xF0 ; RAM[ 0x07B ] = 0x90 ; RAM[ 0x07C ] = 0xF0 ; // 8
    RAM[ 0x07D ] = 0xF0 ; RAM[ 0x07E ] = 0x90 ; RAM[ 0x07F ] = 0xF0 ; RAM[ 0x080 ] = 0x10 ; RAM[ 0x081 ] = 0xF0 ; // 9
    RAM[ 0x082 ] = 0xF0 ; RAM[ 0x083 ] = 0x90 ; RAM[ 0x084 ] = 0xF0 ; RAM[ 0x085 ] = 0x90 ; RAM[ 0x086 ] = 0x90 ; // A
    RAM[ 0x087 ] = 0xE0 ; RAM[ 0x088 ] = 0x90 ; RAM[ 0x089 ] = 0xE0 ; RAM[ 0x08A ] = 0x90 ; RAM[ 0x08B ] = 0xE0 ; // B

    RAM[ 0x08C ] = 0xF0 ; RAM[ 0x08D ] = 0x80 ; RAM[ 0x08E ] = 0x80 ; RAM[ 0x08F ] = 0x80 ; RAM[ 0x090 ] = 0xF0 ; // C
    RAM[ 0x091 ] = 0xE0 ; RAM[ 0x092 ] = 0x90 ; RAM[ 0x093 ] = 0x90 ; RAM[ 0x094 ] = 0x90 ; RAM[ 0x095 ] = 0xE0 ; // D
    RAM[ 0x096 ] = 0xF0 ; RAM[ 0x097 ] = 0x80 ; RAM[ 0x098 ] = 0xF0 ; RAM[ 0x099 ] = 0x80 ; RAM[ 0x09A ] = 0xF0 ; // E
    RAM[ 0x09B ] = 0xF0 ; RAM[ 0x09C ] = 0x80 ; RAM[ 0x09D ] = 0xF0 ; RAM[ 0x09E ] = 0x80 ; RAM[ 0x09F ] = 0xF0 ; // F

    return                                                                                                      ;
}


// loads the game's code in the RAM starting from address 0x200
void load_rom( char* path )
{
    // opens the file and declares variables
    uint16_t buffer                        ; 
    uint16_t RAM_ptr = 0x200               ;
    FILE*    ROM     = fopen( path , "rb" )  ;

    if ( ROM == NULL ) 
    {
        fprintf( stderr , "ROM not found\n" ) ;
        exit( 5 )                               ;
    }

    // for every instruction 
    while( ( fread( &buffer , sizeof(uint16_t) , 1 , ROM ) != 0 ) )
    {
        // copies the last 8-bits  of the instruction in the RAM
        RAM[ RAM_ptr++ ] = (uint8_t) ( (buffer & 0x00FF)      ) ;
        // copies the first 8-bits of the instruction in the RAM
        RAM[ RAM_ptr++ ] = (uint8_t) ( (buffer & 0xFF00) >> 8 ) ;
        

    }

    return ;
}

/************************************************************************/
/**********************|  PIPELINE FUNCTIONS  |**************************/

// reads 2 bytes from memory and returns them as a 16-BIT number
uint16_t if_st( void ) 
{
    uint16_t instruction = ( RAM[ PC++ ] << 8 ) + RAM[ PC++ ] ; 
    return   instruction                                      ; 
}


// extracts a specified part of the instruction
uint16_t from_inst( uint16_t inst , inst_part ind )
{
    uint16_t value                 ; 
    
    // eliminates unrequested BITs
    switch( ind ) 
    {
        case F   : 
            value = inst & 0xF000 ;
            value = value >> 12   ;
            break                 ;

        case X   : 
            value = inst & 0x0F00 ;
            value = value >> 8    ;
            break                 ;

        case Y   : 
            value = inst & 0x00F0 ;
            value = value >> 4    ;
            break                 ;

        case N   : 
            value = inst & 0x000F ;
            break                 ;

        case NN  : 
            value = inst & 0x00FF ;
            break                 ;
        
        case NNN : 
            value = inst & 0x0FFF ;
            break                 ;


        default:
            return -1             ;
    }
    return value                  ;
}


/*

    In order to decode instructions the program will read the first half-byte
    of the instruction, triggering the execution of a function which will then
    do one of two things:

        - If the first half-byte isn't shared by more than one opcode
          it will execute the requested instruction
        
        - If the half-byte is shared by different opcodes it will 
          discriminate between those using other function tables
    
    This approach has been chosen in order not to write excessively long switch
    statements and thus keep the code clean and readable.

*/


// loads the opcodes into their destined array (done in function to keep the code tidy)
void load_opc( void )
{
    // separates the opcodes by starting half-byte
    OPCXX[0x0] = OPC_0 ; OPCXX[0x1] = OPC_1 ; OPCXX[0x2] = OPC_2 ; OPCXX[0x3] = OPC_3 ;
    OPCXX[0x4] = OPC_4 ; OPCXX[0x5] = OPC_5 ; OPCXX[0x6] = OPC_6 ; OPCXX[0x7] = OPC_7 ;
    OPCXX[0x8] = OPC_8 ; OPCXX[0x9] = OPC_9 ; OPCXX[0xA] = OPC_A ; OPCXX[0xB] = OPC_B ;
    OPCXX[0xC] = OPC_C ; OPCXX[0xD] = OPC_D ; OPCXX[0xE] = OPC_E ; OPCXX[0xF] = OPC_F ; 
    
    // loads the opcodes starting with the half-byte "0"
    OPC0X[0x0] = OPC0_0 ; OPC0X[0xE] = OPC0_E ;

    // loads the opcodes starting with the half-byte "8"
    OPC8X[0x0] = OPC8_0 ; OPC8X[0x1] = OPC8_1 ; OPC8X[0x2] = OPC8_2 ; 
    OPC8X[0x3] = OPC8_3 ; OPC8X[0x4] = OPC8_4 ; OPC8X[0x5] = OPC8_5 ; 
    OPC8X[0x6] = OPC8_6 ; OPC8X[0x7] = OPC8_7 ; OPC8X[0xE] = OPC8_E ; 

    // loads the opcodes starting with the half-byte "E"
    OPCEX[0x1] = OPCE_1 ; OPCEX[0xE] = OPCE_E ;

    // loads or separates the opcodes starting with the half-byte "F"
    OPCFX[0x3] = OPCF_3 ; OPCFX[0x5] = OPCF_5 ; OPCFX[0x7] = OPCF_7 ; 
    OPCFX[0x8] = OPCF_8 ; OPCFX[0x9] = OPCF_9 ; OPCFX[0xA] = OPCF_A ;
    OPCFX[0xE] = OPCF_E ;   

    // loads the opcodes starting with "F" and ending with "5"
    OPCF5[0x1] = OPCF5_1 ; OPCF5[0x5] = OPCF5_5 ; OPCF5[0x6] = OPCF5_6 ; 

    return ;

}

// decodes the instruction and executes it
void id_exe_st( uint16_t inst ) 
{
    // breaks the instruction in half bytes (and bigger parts needed by certain instructions)
    uint16_t F_HB  = from_inst( inst , F )  , X_HB   = from_inst( inst , X   ) ;
    uint16_t Y_HB  = from_inst( inst , Y )  , N_HB   = from_inst( inst , N   ) ; 
    uint16_t NN_HB = from_inst( inst , NN ) , NNN_HB = from_inst( inst , NNN ) ;

    // debug: prints the instruction
    // printf("Istruzione: %x%x%x%x\n\n", F_HB,X_HB,Y_HB,N_HB) ;

    // redirections the execution of the instruction to other functions
    OPCXX[ F_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB )                ; 
}


/****************************************************************************/
/**********************|  OPCODES & INSTRUCTIONS  |**************************/


// redirects the opcodes starting with "0" to their implementation
void OPC_0( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    OPC0X[ N_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB ) ;
    return                                                      ;
}


// redirects the opcodes starting with "8" to their implementation
void OPC_8( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    OPC8X[ N_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB ) ;
    return                                                      ;
}

// redirects the opcodes starting with  "E" to their implementation
void OPC_E( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    OPCEX[ N_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB ) ;
    return                                                      ;
}

// redirects the opcodes starting with "F" to their implementation
void OPC_F( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    OPCFX[ N_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB ) ;
    return                                                      ;
}

// redirects the opcodes starting with "F" and ending with "5" to their implementation
void OPCF_5( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    OPCF5[ Y_HB ]( F_HB , X_HB , Y_HB , N_HB , NN_HB , NNN_HB ) ;
    return                                                      ;
}


/*


    What follows is the implementation of some of the most basic instruction used to draw
    the IBM logo. The choice to implement these instructions first relies on the possibility 
    to check how the cpu works right now. Later, the CLI interface will be replaced by a 
    proper SDL2 window


*/


// clears the screen
void OPC0_0( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    for ( int i = 0 ; i < SCR_H ; i++ ) memset( DSP[ i ] , 0x00 , SCR_W ) ;
    return                                                                ;
}


// returns from a subroutine
void OPC0_E( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    PC = stack_pop( ) ;
    return            ; 
}


// jumps to the NNN address
void OPC_1( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    PC = NNN_HB ;
    return      ;
}


// jumps to the NNN location and saves the return address on top of the stack
void OPC_2( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    stack_push( PC ) ;
    PC = NNN_HB      ;
    return           ;
}


// skips one instruction if Vx is equal to NN
void OPC_3( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    if ( REG[ X_HB ] == NN_HB ) PC += 2 ;
    return ;
}

// skips one instruction if Vx is not equal to NN
void OPC_4( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    if ( REG[ X_HB ] != NN_HB ) PC += 2 ;
    return ;
}

// skips one instruction if Vx is equal to Vy
void OPC_5( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    if ( REG[ X_HB ] == REG[ Y_HB ] ) PC += 2 ;
    return ;
}


// stores NN into the X register
void OPC_6( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = NN_HB ;
    return              ;
}


// adds NN to the X register
void OPC_7( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] += NN_HB ;
    return               ;
}


// sets Vx to the value of Vy
void OPC8_0( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = REG[ Y_HB ] ;
    return ;
}

// applys the "logical or" operation to Vx and Vy and saves the result in Vx
void OPC8_1( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] |= REG[ Y_HB ] ;
    return ;
}

// applys the "logical and" operation to Vx and Vy and saves the result in Vx
void OPC8_2( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] &= REG[ Y_HB ] ;
    return ;
}

// applys the "logical xor" operation to Vx and Vy and saves the result in Vx
void OPC8_3( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] ^= REG[ Y_HB ] ;
    return ;
}

// adds Vy to Vx
void OPC8_4( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] += REG[ Y_HB ] ;
    return ;
}

// sets Vx to the result of Vx - Vy
void OPC8_5( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] -= REG[ Y_HB ] ;
    return ;
}

// ambiguous: sets Vx to Vx (or Vy) shifted to the right once
void OPC8_6( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = REG[ Y_HB ]           ; // optional line!!
    REG[ 0xF  ] = REG[ X_HB ] &  0x0001 ;
    REG[ X_HB ] = REG[ X_HB ] >> 1      ;
    return ;
}

// sets Vx to the result of Vy - Vx
void OPC8_7( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = REG[ Y_HB ] - REG[ X_HB ] ;
    return ;
}

// ambiguous: sets Vx to Vx (or Vy) shifted to the left once
void OPC8_E( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = REG[ Y_HB ]           ; // optional line!!
    REG[ 0xF  ] = REG[ X_HB ] &  0x0001 ;
    REG[ X_HB ] = REG[ X_HB ] << 1      ;
    return ;
}

// skips an instruction if Vx is not equal to Vy
void OPC_9( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    if ( REG[ X_HB ] != REG[ Y_HB ] ) PC += 2 ;
    return ;
}


// sets the I_REG to NNN
void OPC_A( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    I_REG = NNN_HB ;
    return         ;
}


// ambiguous: sets the program counter to NNN + V0
void OPC_B( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    PC = NNN_HB + REG[ 0x0 ] ; 
    return ;
}


// generates a random number, applys "logical and" between it and NN and sets Vx to the result
void OPC_C( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = rand( ) & NN_HB ; 
    return ;
}


// implements a pixel-by-pixel xor between N bytes starting from the I_REG value and the display's pixels
void OPC_D( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    // resets the collision flag 
    REG[ 0xF ] = 0 ;
    
    // gets the X (0-63) and the Y (0-31) coordinates to start drawing the sprite
    uint8_t spr_x = REG[ X_HB ] % SCR_W ;
    uint8_t spr_y = REG[ Y_HB ] % SCR_H ;
    uint8_t bit , drw_x , drw_y         ;

    // for every byte that needs to be drawn
    for( int spr_row = 0 ; spr_row < N_HB ; spr_row++ )
    {
        // loads the byte into a buffer
        uint8_t buffer = RAM[ I_REG + spr_row ] ;

        // for every bit in the buffer, starting from the MSB
        for( int bit_n = 7 ; bit_n >= 0 ; bit_n-- )
        {
            // calculates the coordinates
            bit   = ( buffer & ( 1 << bit_n ) ) >> bit_n ;
            drw_x = ( spr_x  + ( 7 - bit_n  ) )  % SCR_W ;
            drw_y = ( spr_y  +   spr_row      )  % SCR_H ;
            
            // updates the collision flag
            if ( ( DSP[ drw_y ][ drw_x ] ) && ( bit ) ) REG[ 0xF ] = 1 ;

            // updates the pixel's colour
            DSP[ drw_y ][ drw_x ] ^= bit ;
        }
    }

    // draws the updted display
    draw( ) ; 
}

// to be implemented
void OPCE_1( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    return ;
}


// to be implemented
void OPCE_E( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    return ;
}


// converts Vx in decimal and stores each digit in the RAM starting from the address found in the I register
void OPCF_3( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    uint8_t val = REG[ X_HB ]       ; 

    for( int i = 0 ; i < 3 ; i++ )
    {
        RAM[ I_REG + i ] = val % 10 ;
        val /= 10                   ; 
    }
    return                          ;
}

// sets the delay timer to Vx
void OPCF5_1( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    DEL_TIMER = REG[ X_HB ] ;
    return ;
}

// stores V0 to Vx (included) in the RAM starting from the address stored in the I register
void OPCF5_5( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    for ( int i = 0 ; i <= X_HB ; i++ )
    {
        RAM[ I_REG + i ] = REG[ i ] ;
    }
    return ;
}

// stores x+1 bytes from the RAM (starting from address I) in the first x+1 registers
void OPCF5_6( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    for ( int i = 0 ; i <= X_HB ; i++ )
    {
        REG[ i ] = RAM[ I_REG + i ]  ;
    }
    return ;
}

// sets Vx to the value of the delay timer
void OPCF_7( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    REG[ X_HB ] = DEL_TIMER ;
    return ;
}

// sets the sound timer to Vx
void OPCF_8( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    SND_TIMER = REG[ X_HB ] ; 
    return ;
}

// sets the I register value to the address of the character selected
void OPCF_9( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    I_REG = 0x050 + ( REG[ X_HB ] * 5 ) ;
    return ;
}

// to be implemented
void OPCF_A( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    return ;
}

// adds Vx to the I register 
void OPCF_E( uint16_t F_HB , uint16_t X_HB , uint16_t Y_HB , uint16_t N_HB , uint16_t NN_HB , uint16_t NNN_HB )
{
    I_REG += REG[ X_HB ] ;
    return ;
}
/**************************************************************************/
/**********************|  GRAPHICS (TEMPORARY)  |**************************/


// draws the display in the command line
void draw( void )
{
    // clears the display
    printf( CLR_DSP ) ;
    fflush( stdout )  ;

    // for every row
    for( int row = 0 ; row < SCR_H ; row++ )
    {
        // draws the row and goes to the next one
        for( int col = 0 ; col < SCR_W ; col++ )
        {
            if ( DSP[ row ][ col ] == ON ) printf( "#" ) ;
            else                           printf( " " ) ;
        }
        printf( "\n" ) ;
    }
}


