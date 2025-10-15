pname := doomsday
builddir := build/

all:
	@mkdir -p $(builddir)
	c++ $(wildcard *.cpp) -o $(builddir)$(pname)

clean:
	rm -rf $(builddir)
