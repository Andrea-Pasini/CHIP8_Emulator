/*
*
*   HARDWARE SIMULATION MODULE
*
*/

/* TO-DO NEXT: fetch, decode, and execute and other initialization functions ( i.e. registers ) */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


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



/****************************************************************/
/**********************| CUSTOM-TYPES |**************************/


// custom types
typedef struct stack_struct
{
    uint8_t sp                ;
    uint8_t cells[ STK_SIZE ] ;
}   stack_t                   ;

// part of instruction for "from_inst"
typedef enum 
{
    F         ,
    X         ,
    Y         ,   
    N         ,
    NN        ,
    NNN       ,
}   inst_part ; 


/**************************************************************/
/**********************| PROTOTYPES |**************************/


// stack functions prototypes
int8_t   stack_empty( void                          ) ;
void     stack_init ( void                          ) ;
void     stack_push ( uint8_t val                   ) ; 
uint8_t  stack_pop  ( void                          ) ;

// RAM functions prototype
void     ram_init   ( void                          ) ;
void     load_font  ( void                          ) ;
void     load_rom   ( char* path                    ) ;

// pipeline prototypes
uint16_t fetch      ( void                          ) ; 
uint8_t  from_inst  ( uint16_t inst , inst_part ind ) ;




/********************************************************************/
/**********************| GLOBAL_VARIABLES |**************************/


// hardware components ( as global variables )
stack_t STK             ;
uint8_t PC              ;
uint8_t SND_TIMER       ;
uint8_t DEL_TIMER       ;
uint8_t RAM[ RAM_SIZE ] ;
uint8_t REG[ REG_NUM  ] ;


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
    memset( STK.cells , 0x00 , STK_SIZE ) ;
    return                                ;
}


// pushes a value in the stack
void stack_push( uint8_t val ) 
{
    // in case of overflow it doesn't signal it
    if ( STK.sp < STK_SIZE ) STK.cells[ STK.sp++ ] = val ;
    return                                               ; 
} 


// returns the value on top of the stack
uint8_t stack_pop ( void ) 
{
    return STK.cells[ STK.sp-- ] ; 
}


/*********************************************************/
/**********************|  RAM  |**************************/


// initializes the RAM
void ram_init( void )
{
    load_font( )    ;
    PC = ENTRY_PT   ; 
}


// loads the chip-8 standard font in the RAM
void load_font( void )
{
    RAM[ 0x050 ] = 0xF0 , RAM[ 0x051 ] = 0x90 , RAM[ 0x052 ] = 0x90 , RAM[ 0x053 ] = 0xF0 , RAM[ 0x054 ] = 0xF0 ; // 0
    RAM[ 0x055 ] = 0x20 , RAM[ 0x056 ] = 0x60 , RAM[ 0x057 ] = 0x20 , RAM[ 0x058 ] = 0x20 , RAM[ 0x059 ] = 0x70 ; // 1
    RAM[ 0x05A ] = 0xF0 , RAM[ 0x05B ] = 0x10 , RAM[ 0x05C ] = 0xF0 , RAM[ 0x05D ] = 0x80 , RAM[ 0x05E ] = 0xF0 ; // 2
    RAM[ 0x05F ] = 0xF0 , RAM[ 0x060 ] = 0x10 , RAM[ 0x061 ] = 0xF0 , RAM[ 0x062 ] = 0x10 , RAM[ 0X063 ] = 0xF0 ; // 3

    RAM[ 0x064 ] = 0x90 , RAM[ 0x065 ] = 0x90 , RAM[ 0x066 ] = 0xF0 , RAM[ 0x067 ] = 0x10 , RAM[ 0x068 ] = 0x10 ; // 4
    RAM[ 0x069 ] = 0xF0 , RAM[ 0x06A ] = 0x90 , RAM[ 0x06B ] = 0xF0 , RAM[ 0x06C ] = 0x10 , RAM[ 0x06D ] = 0xF0 ; // 5
    RAM[ 0x06E ] = 0xF0 , RAM[ 0x06F ] = 0x80 , RAM[ 0x070 ] = 0xF0 , RAM[ 0x071 ] = 0x90 , RAM[ 0x072 ] = 0xF0 ; // 6
    RAM[ 0x073 ] = 0xF0 , RAM[ 0x074 ] = 0x10 , RAM[ 0x075 ] = 0x20 , RAM[ 0x076 ] = 0x40 , RAM[ 0x077 ] = 0x40 ; // 7

    RAM[ 0x078 ] = 0xF0 , RAM[ 0x079 ] = 0x90 , RAM[ 0x07A ] = 0xF0 , RAM[ 0x07B ] = 0x90 , RAM[ 0x07C ] = 0xF0 ; // 8
    RAM[ 0x07D ] = 0xF0 , RAM[ 0x07E ] = 0x90 , RAM[ 0x07F ] = 0xF0 , RAM[ 0x080 ] = 0x10 , RAM[ 0x081 ] = 0xF0 ; // 9
    RAM[ 0x082 ] = 0xF0 , RAM[ 0x083 ] = 0x90 , RAM[ 0x084 ] = 0xF0 , RAM[ 0x085 ] = 0x90 , RAM[ 0x086 ] = 0x90 ; // A
    RAM[ 0x087 ] = 0xE0 , RAM[ 0x088 ] = 0x90 , RAM[ 0x089 ] = 0xE0 , RAM[ 0x08A ] = 0x90 , RAM[ 0x08B ] = 0xE0 ; // B

    RAM[ 0x08C ] = 0xF0 , RAM[ 0x08D ] = 0x80 , RAM[ 0x08E ] = 0x80 , RAM[ 0x08F ] = 0x80 , RAM[ 0x090 ] = 0xF0 ; // C
    RAM[ 0x091 ] = 0xE0 , RAM[ 0x092 ] = 0x90 , RAM[ 0x093 ] = 0x90 , RAM[ 0x094 ] = 0x90 , RAM[ 0x095 ] = 0xE0 ; // D
    RAM[ 0x096 ] = 0xF0 , RAM[ 0x097 ] = 0x80 , RAM[ 0x098 ] = 0xF0 , RAM[ 0x099 ] = 0x80 , RAM[ 0x09A ] = 0xF0 ; // E
    RAM[ 0x09B ] = 0xF0 , RAM[ 0x09C ] = 0x80 , RAM[ 0x09D ] = 0xF0 , RAM[ 0x09E ] = 0x80 , RAM[ 0x09F ] = 0xF0 ; // F

    return                                                                                                      ;
}


// loads the game's code in the RAM starting from address 0x200
void load_rom( char* path )
{
    // opens the file and declares variables
    uint16_t buffer                        ; 
    uint16_t RAM_ptr = 0x200               ;
    FILE*    ROM     = fopen( path , rb )  ;

    // for every instruction 
    while( ( buffer = fread( 1 , sizeof(uint16_t) , ROM ) != EOF ) )
    {
        // copies the first 8-bits of the instruction in the RAM
        RAM[ RAM_ptr++ ] = (uint8_t) ( (buffer & 0xFF00) >> 8 ) ;

        // copies the last 8-bits  of the instruction in the RAM
        RAM[ RAM_ptr++ ] = (uint8_t) ( (buffer & 0x00FF)      ) ;
    }

    return ;
}

/************************************************************************/
/**********************|  PIPELINE FUNCTIONS  |**************************/

// reads 2 bytes from memory and returns them as a 16-BIT number
uint16_t fetch( void ) ; 
{
    uint16_t instruction = ( RAM[ PC++ ] << 8 ) + [ RAM[ PC++ ] ] ; 
    return   instruction                                          ; 
}

/* 

    For the decode stage of the pipeline i first need to learn about function
    pointers, in order to create tidier looking code. My idea is to create an
    array-of-arrays in which the i can identify the first nibble and then 
    recognize the opcode ( more on this later )

*/

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

void decode( uint16_t inst ) 
{
    uint16_t F = from_inst( inst , F ) , X  = from_inst( inst , X  ) , Y   = from_inst( inst , Y   ) ;
    uint16_t N = from_inst( inst , N ) , NN = from_inst( inst , NN ) , NNN = from_inst( inst , NNN ) ;
}