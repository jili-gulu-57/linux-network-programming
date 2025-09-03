.PHONY:all
all:client server

client:client.cpp
	g++ -o $@ $^ -std=c++11

server:server.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f client
	rm -f server