
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11
SRCS := malloc.cpp ./Enemy/Enemy.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := my_program
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
