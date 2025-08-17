

// stack functions prototypes
int8_t   stack_empty( void                          ) ;
void     stack_init ( void                          ) ;
void     stack_push ( uint16_t val                  ) ; 
uint16_t stack_pop  ( void                          ) ;

// RAM functions prototype
void     ram_init   ( void                          ) ;
void     load_font  ( void                          ) ;
void     load_rom   ( char* path                    ) ;

// pipeline prototypes
uint16_t if_st      ( void                          ) ; 
void     load_opc   ( void                          ) ;
uint16_t from_inst  ( uint16_t inst , inst_part ind ) ;
void     id_exe_st  ( uint16_t inst                 ) ;

// opcode prototypes
void OPC_0(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_1(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_2(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_3(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_4(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_5(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_6(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_7(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_8(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_9(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_A(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_B(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_C(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_D(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_E(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC_F(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// prototypes for redirected opcodes (starting with 0)
void OPC0_0(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC0_E(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// prototypes for redirected opcodes (starting with 8)
void OPC8_0(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_1(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_2(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_3(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_4(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_5(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_6(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_7(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPC8_E(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// prototypes for redirected opcodes (starting with E)
void OPCE_1(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCE_E(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// prototypes for redirected opcodes (starting with F)
void OPCF_3(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_5(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_7(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_8(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_9(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_A(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF_E(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// prototypes for redirected opcodes (starting with F and ending with 5)
void OPCF5_1(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF5_5(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);
void OPCF5_6(uint16_t F_HB, uint16_t X_HB, uint16_t Y_HB, uint16_t N_HB, uint16_t NN_HB, uint16_t NNN_HB);

// CLI prototypes
void draw( void ) ;

