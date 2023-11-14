# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Output binary name
TARGET = a4

# Source files
SRCS = a4.cpp CSR.cpp 

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Targets and dependencies
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)