CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -g

# Source files (you can expand or automate this later)
SRCS := malloc.cpp ./Enemy/Enemy.cpp 

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := my_program

# Libraries: SDL2, SDL2_image, SDL2_ttf, OpenGL, GLEW
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW

# Include any additional directories if needed
# INCLUDES := -I./include

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Link the target
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

.PHONY: all cleanssssssss

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
