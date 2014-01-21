CXX 	 = g++
CXXFLAGS = -W -Wall -c -DPORT_NUM=2000 -DIP_ADDR=\"192.168.0.38\"
LDFLAGS	 = -lSDL2 -lSDL2_net
OBJECTS  = packet_processing.o

all: packet_processing.o
	$(CXX) $(OBJECTS) -o clienttest $(LDFLAGS)

packet-processing.o: packet_processing.cpp
	$(CXX) $(CXXFLAGS) packet_processing.cpp

