
VPATH = traci_testclient;utils/traci;foreign/tcpip;utils/common

all:Traci_Client.exe

Traci_Client.exe:tracitestclient_main.o TraCITestClient.o TraCIAPI.o socket.o storage.o SUMOTime.o StringUtils.o StringTokenizer.o StdDefs.o
	g++ -std=c++11 -g -o Traci_Client.exe tracitestclient_main.o TraCITestClient.o TraCIAPI.o socket.o storage.o SUMOTime.o StringUtils.o StringTokenizer.o StdDefs.o -L . -l ws2_32 -l xerces-c_3_2 -l xerces-c_3_2D -l websockets -L lws-x64-22747226/bin/Release

tracitestclient_main.o:tracitestclient_main.cpp
	g++ -std=c++11 -c -g $^

TraCITestClient.o:TraCITestClient.cpp
	g++ -std=c++11 -c -g $^ -I lws-x64-22747226/include -I wolfssl-3.15.3 -I wolfssl-3.15.3/wolfssl
	
TraCIAPI.o:TraCIAPI.cpp
	g++ -std=c++11 -c -g $^
	
socket.o:socket.cpp
	g++ -std=c++11 -c -g $^
	
storage.o:storage.cpp
	g++ -std=c++11 -c -g $^
	
SUMOTime.o:SUMOTime.cpp
	g++ -std=c++11 -c -g $^ -I.
	
StringUtils.o:StringUtils.cpp
	g++ -std=c++11 -c -g $^ -I.
	
StringTokenizer.o:StringTokenizer.cpp
	g++ -std=c++11 -c -g $^
	
StdDefs.cpp:StdDefs.o
	g++ -std=c++11 -c -g $^

clean:
	del Traci_Client.exe tracitestclient_main.o TraCITestClient.o socket.o storage.o SUMOTime.o TraCIAPI.o StringUtils.o StringTokenizer.o StdDefs.o