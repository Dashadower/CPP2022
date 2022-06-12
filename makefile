
CXX=g++
CXXFLAGS=-lncurses -g

DEPENDS:=$(wildcard *.h)

$(info found header files $(DEPENDS))

snake: main.cpp $(DEPENDS)
	$(CXX) $< $(CXXFLAGS) -o $@