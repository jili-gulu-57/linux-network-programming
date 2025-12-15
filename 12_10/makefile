.PHONY:all
all: chat_client chat_server

chat_client:ChatClient.cc
	g++ -o $@ $^ -std=c++17
chat_server:ServerMain.cc
	g++ -o $@ $^ -std=c++17 -lpthread

.PHONY:clean
clean:
	rm -f chat_client chat_server