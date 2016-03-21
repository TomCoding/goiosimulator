# CXXFLAGS  = -std=c++11 -O3 -ffast-math -g -Wall -pedantic
CXXFLAGS  = -std=c++11 -O3 -ffast-math -g -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef #-Weffc++

TARGET    = goiovalues
SOURCES   = $(wildcard *.cpp)
HEADERS   = $(wildcard *.h)
OBJECTS   = $(SOURCES:%.cpp=%.o)

all: $(TARGET) $(SOURCES) $(HEADERS)

$(TARGET): $(OBJECTS) $(HEADERS)
	@echo "(CXX) $(TARGET)"
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	@echo "(CXX) $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o

distclean: clean
	-rm $(TARGET)
