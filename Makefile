pname := doomsday
builddir := build/

all:
	@mkdir -p $(builddir)
	c++ $(wildcard *.cpp) -o $(builddir)$(pname)

debug:
	@mkdir -p $(builddir)
	c++ $(wildcard *.cpp) -g -o $(builddir)$(pname)

clean:
	rm -rf $(builddir)
