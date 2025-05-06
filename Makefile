CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20
TARGETS = udp_client udp_server tcp_client tcp_server

all: $(TARGETS)

udp_client: udp_client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

udp_server: udp_server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tcp_client: tcp_client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tcp_server: tcp_server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
