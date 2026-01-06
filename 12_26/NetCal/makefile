make:all
all:Client Server

Client:Client.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

Server:Main.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

.PHONY:clean
clean:
	rm -f Client Server