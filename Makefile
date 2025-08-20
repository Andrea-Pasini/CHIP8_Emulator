# compiler choice
CC 		:= gcc

# gcc flags
CFLAGS  :=  -g3 -o0  -Iincludes 

# linking flags
#LDFLAGS := -l SDL2

# directories
SRC_DIR := source
OBJ_DIR := build/obj
BIN_DIR := build

# source code files ( every .c file in source directory )
SRCS 	:= $(wildcard $(SRC_DIR)/*.c) 

# object files ( one from every source file )
OBJS	:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# final executable file path
TARGET  := $(BIN_DIR)/chip8

###  build  ###

# final result
all: $(TARGET)

# linking instructions
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# compiling instructions ( takes every .c file from SRC_DIR and turns it in .o )
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

# eliminates previous versions
clean: 
	rm -rf $(OBJ_DIR) $(TARGET)

# tells make that all and clean aren't files
.PHONY: all clean	
	


