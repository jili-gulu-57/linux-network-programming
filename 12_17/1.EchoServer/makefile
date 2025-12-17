.PHONY:all
all:tcp_client tcp_server

tcp_client:TcpClient.cc
	g++ -o $@ $^ -std=c++11

tcp_server:TcpServer
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf tcp_client tcp_server