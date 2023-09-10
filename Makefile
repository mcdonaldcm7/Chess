# Source directory
SRC_DIR = src

# OBJS specifies which source files to compile
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SOURCES:.cpp=.o)

# Compiler and flags
CC := g++
#CFLAGS = -Wall -Werror -Wextra -pedantic -g
CFLAGS := -w -g

# Libraries to link
LIBS := -lSDL2 -lSDL2_image

# Executable name
EXECUTABLE := Chess

# Build target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) main.o
	$(CC) $(OBJS) main.o $(LIBS) -o $@

# Rule to compile source files to object files
%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile the main.cpp file
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

# Clean target
clean:
	rm -f $(OBJS) main.o $(EXECUTABLE)
