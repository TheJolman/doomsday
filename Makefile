pname := doomsday
builddir := build/

CXX ?= clang++
CXXFLAGS := -Wall -std=c++20

all:
	@mkdir -p $(builddir)
	$(CXX) $(CXXFLAGS) $(wildcard *.cpp) -o $(builddir)$(pname)

debug:
	@mkdir -p $(builddir)
	$(CXX) $(CXXFLAGS) $(wildcard *.cpp) -g -o $(builddir)$(pname)-debug

clean:
	rm -rf $(builddir)
