# OBJS specifies which files to compile as part of the project
OBJS = main.cpp src/game.cpp src/chess_board.cpp src/view_ops.cpp src/king.cpp src/queen.cpp src/rook.cpp src/bishop.cpp src/knight.cpp src/pawn.cpp src/piece.cpp src/chess_board_ops.cpp

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# add "-Wall -Werror -Wextra -pedantic" flag later
COMPILER_FLAGS = -w -g

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

# OBJ_NAME specifies the name of the executable
OBJ_NAME = Chess

# This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
