
CXX=g++
CXXFLAGS=-lncursesw -std=c++14

DEPENDS:=$(wildcard *.h)

$(info found header files $(DEPENDS))

snake: main.cpp $(DEPENDS)
	$(CXX) $< $(CXXFLAGS) -o $@
