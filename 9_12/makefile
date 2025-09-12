.PHONY:all
all:mykill myprocess

myprocess:myprocess.cc
	g++ -o $@ $^ -std=c++11

mykill:mykill.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f mykill myprocess