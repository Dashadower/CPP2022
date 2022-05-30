
CXX=g++
CXXFLAGS=-lncurses

DEPENDS:=$(wildcard *.h)

$(info found header files $(DEPENDS))

snake: main.cpp $(DEPENDS)
	$(CXX) $< $(CXXFLAGS) -o $@