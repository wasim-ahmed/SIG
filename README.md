# SIG (SUMO Interface with Google Maps)

This project can be used to visualize SUMO interface on Google Map. The SUMO is used to create a traffic simulator, SUMO data is retrieved by TRACI.
Data is then passed to Web Server/WebSocket program, which then pushes the data to Web Socket Server for getting it plotted on Google Map. 

# SetUp

1. Start the server.exe located in "\Webserver\simple-libwebsockets-example-master" . This will start the Web server.
2. Open the web browser on "localhost:8000" . Do not wait for map to load , proceed to next step. 
3. Start the SUMO from "\SUMO" by using command "sumo-gui -c icw.sumo.cfg --remote-port 8800" from terminal. This will start the SUMO server.
4. Start the TRACI from "\TraciClient" by starting "Traci_Client.exe". This will start the TRACI Client & establish the connection between SUMO & TRACI.
5. Increase the delay time to around 200 and start the simulation from SUMO GUI.
6. Go back to browser and you should have visualization on Google map.

Note : Currently project is configured for 2 vehicles only named "veh1" & "veh2", it can be easily extended for more (code changes will be required). extension in progress ! 

![alt text](https://github.com/wasim-ahmed/SIG/blob/master/SIG%20Diagram.jpg)

# Build Info

Projects are build with MinGW 4.8.1 on Windows 10 .

This project is a combination of 3 sub projects:

1. Webserver

	This project contains the code for Web Server & Web Socket Server. It uses libwebsockets & wolfssl project. We can create its binary from cmd line.
	It receives the data from TraciClient (via Web Socket Client) & pass the data to HTML/JavaScript code for plotting it on Google Map. Remember to 
	place required DLL's where executables are kept. 	
	
	buid the server.exe using commands :
	
	gcc -c server.c -I ..\lws-x64-22747226\include -I ..\wolfssl-3.15.3 -I ..\wolfssl-3.15.3\wolfssl
	
	gcc -o server.exe server.o -l ws2_32 -l websockets -L ..\lws-x64-22747226\bin\Release
	
2. SUMO

	SUMO needs to be configured on the system. Then by writing the configuration file we can create the desired traffic simulations.
	Note: Code is not required in this project , only configuration files needs to be created.
	
3. TraciClient

	This communicates with SUMO through a socket to get the data. Then using the web Socket api's it pass the data to WebServer program.
	This is coded in C++ , build the project by using makefile. Note that this project requires libwebsocket, wolfssl & xerses projects.
	Remember to place required DLL's where executables are kept	
	

NOTE : Executables present in the project will run as is (as the DLL's required at run time is present) , it is only when we need to build the project, we will require the compiler. 

# Software Components

1. libwebsockets binary for windows @ https://ci.appveyor.com/project/lws-team/libwebsockets 
2. WolfSSL @ https://www.wolfssl.com/
3. SUMO @ https://sumo.dlr.de/wiki/Downloads
4. Xerces @ https://sourceforge.net/projects/sumo/files/dependencies/ 

Special Thanks to 

1. https://github.com/iamscottmoyers/simple-libwebsockets-example
