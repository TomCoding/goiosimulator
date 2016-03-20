VERBOSE=1

CXX       = g++
CXXFLAGS := -std=c++11 -g -Wall -pedantic

TARGET    = goiovalues
SOURCES   = $(wildcard *.cpp)
HEADERS   = $(wildcard *.h)
# OBJECTS   = $(SOURCES:%.cpp=%.o)

all: $(TARGET)
#
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o $(TARGET) $^

clean:
	-rm -f *.o
	-rm $(TARGET)
