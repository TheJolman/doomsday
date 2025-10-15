pname := doomsday
builddir := build/

all:
	c++ $(wildcard *.cpp) -o $(builddir)/$(pname)
