/****************************************************************************/
/// @file    TraCITestClient.cpp
/// @author  Friedemann Wesner
/// @author  Daniel Krajzewicz
/// @author  Laura Bieker
/// @author  Axel Wegener
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    2008/04/07
/// @version $Id$
///
/// A test execution class
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2008-2016 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include "../config.h"
#endif

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>

//#define _BSD_SOURCE
#include "sys/time.h"

#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUILD_TCPIP
#include "../foreign/tcpip/storage.h"
#include "../foreign/tcpip/socket.h"

#include "../traci-server/TraCIConstants.h"
#include "../utils/common/SUMOTime.h"
#include "../utils/common/ToString.h"
#include "TraCITestClient.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS



// ===========================================================================
// method definitions
// ===========================================================================

static struct lws *web_socket = NULL;

#define EXAMPLE_RX_BUFFER_BYTES (31) //number of bytes to be transferred , change same on server side 

double g_lat1,g_long1,g_lat2,g_long2;

static int callback_example( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
	
	switch( reason )
	{
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			lws_callback_on_writable( wsi );
			break;

		case LWS_CALLBACK_CLIENT_RECEIVE:
			/* Handle incomming messages here. */
			break;

		case LWS_CALLBACK_CLIENT_WRITEABLE:
		{
			unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
			unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
			/*size_t n = sprintf( (char *)p, "%u", rand() );*/
			
			size_t n = sprintf( (char *)p,"%.4f,%.4f,%.4f,%.4f",g_lat1,g_long1,g_lat2,g_long2);
			/*printf("%.4f : %.4f\n",coordinates[i][0] ,coordinates[i][0]);
				printf("Bytes written:%d \tData Written:%s\n\n",n,p);*/
			lws_write( wsi, p, n, LWS_WRITE_TEXT );
		
			break;
		}

		case LWS_CALLBACK_CLOSED:
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			web_socket = NULL;
			break;

		default:
			break;
	}
	

	return 0;
}


enum protocols
{
	PROTOCOL_EXAMPLE = 0,
	PROTOCOL_COUNT
};

static struct lws_protocols protocols[] =
{
	{
		"example-protocol",
		callback_example,
		0,
		EXAMPLE_RX_BUFFER_BYTES,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};

void TraCITestClient::V2X_run()
{
    //Connect to the SUMO Server
	
	try {
            TraCIAPI::connect(host, port);
    } catch (tcpip::SocketException& e) {
            std::cout<<"Connection Not Established with the Server"<<std::endl;     
    }

	struct lws_context_creation_info info;
	memset( &info, 0, sizeof(info) );

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	struct lws_context *context = lws_create_context( &info );

	time_t old = 0;
	
		
	//sumo vehicle vectors
	static double timer = 0;
	std::vector<std::string> allveh; 
	std::vector<std::string>::iterator it;
	bool veh1_avail = false,veh2_avail = false;

	while(1)
	{
		timer++;
		V2X_simulationEngine(timer);
		
		double simulation_time = TraCIAPI::simulation.getTime();

		if(simulation_time > 2)//code check not to print location if no vehicles are there on road.
		{
			//take the vehicle data
			allveh = TraCIAPI::vehicle.getIDList();
			for(it = allveh.begin();it!=allveh.end();it++)
			{
				//std::cout<<"vehicle id:"<<*it<<std::endl;
				if(*it == "veh1")
				{
					veh1_avail = true;//currently checking for veh1 can be checked for more vehicles also
				}
				if(*it == "veh2")
				{
					veh2_avail = true;//currently checking for veh1 can be checked for more vehicles also
				}
				
			}
				
			if (veh1_avail == true)
			{			
				/*
				libsumo::TraCIPosition posi = TraCIAPI::vehicle.getPosition("veh1");
				std::cout<<posi.x<<"\t";
				std::cout<<posi.y<<"\t";
				std::cout<<posi.z<<std::endl;
				std::cout<<"veh1 speed:"<<TraCIAPI::vehicle.getSpeed("veh1")<<std::endl;
				*/
				
				position = V2X_getRealPosition("veh1");
				std::cout<<"veh1"<<std::endl;
				std::cout<<position.latitude<<"\t";
				std::cout<<position.longitude<<std::endl;
				g_lat1 = position.latitude;
				g_long1 = position.longitude;
			}
			else{
				std::cout<<"-----------vehicle 1 has left--------------"<<std::endl;
			}
			position = {0};
			if (veh2_avail == true)
			{	
				
				position = V2X_getRealPosition("veh2");
				std::cout<<"veh2"<<std::endl;
				std::cout<<position.latitude<<"\t";
				std::cout<<position.longitude<<std::endl;
				g_lat2 = position.latitude;
				g_long2 = position.longitude;
			
			}
			else{
				std::cout<<"-----------vehicle 2 has left--------------"<<std::endl;
			}
			veh1_avail = false;
			veh2_avail = false;
			
			//Pass to server for potting
			
			struct timeval tv;
			gettimeofday( &tv, NULL );

			/* Connect if we are not connected to the server. */
			if( !web_socket && tv.tv_sec != old )
			{
				struct lws_client_connect_info ccinfo = {0};
				ccinfo.context = context;
				ccinfo.address = "localhost";
				ccinfo.port = 8000;
				ccinfo.path = "/";
				ccinfo.host = lws_canonical_hostname( context );
				ccinfo.origin = "origin";
				ccinfo.protocol = protocols[PROTOCOL_EXAMPLE].name;
				web_socket = lws_client_connect_via_info(&ccinfo);
			}

			if( tv.tv_sec != old )
			{
				/* Send a random number to the server every second. */
				lws_callback_on_writable( web_socket );
				old = tv.tv_sec;
			}
			//Sleep(5000);

			lws_service( context, /* timeout_ms = */ 250 );
			
		}

			allveh.clear();
				
		}
		lws_context_destroy( context );
       //Close the required stuff
        commandClose();//closes the storage
        close();//closes the socket
	}


void TraCITestClient::V2X_simulationEngine(double time)
{
	send_commandSimulationStep(time);
    tcpip::Storage inMsg;

    try {
        std::string acknowledgement;
        check_resultState(inMsg, CMD_SIMSTEP, false, &acknowledgement);
    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_simulationEngine error"<<std::endl;
    }

}

Coordinates TraCITestClient::V2X_getRealPosition(std::string vehicleID)
{

    int domID=0xa4;
    int varID=0x42;
	
	double x,y;
//reinventing the wheel
	
	send_commandGetVariable(domID, varID, vehicleID);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_getRealPosition error"<<std::endl;

    }
    check_commandGetResult(inMsg, domID, -1, false);
    // report result state
    try {
        int variableID = inMsg.readUnsignedByte();
        std::string objectID = inMsg.readString();

        int valueDataType = inMsg.readUnsignedByte();

        x = inMsg.readDouble();
        y = inMsg.readDouble();

    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_getRealPosition error"<<std::endl;

    }

	//the above part can be reduced to one single command
   // libsumo::TraCIPosition pos = TraCIAPI::vehicle.getPosition("veh1");


    tcpip::Storage* tmp = new tcpip::Storage;
    tmp->writeByte(TYPE_COMPOUND);
    tmp->writeInt(2);
    tmp->writeByte(POSITION_2D);
    tmp->writeDouble(x);
    tmp->writeDouble(y);
    tmp->writeByte(TYPE_UBYTE);
    tmp->writeUnsignedByte(POSITION_LON_LAT);
    send_commandGetVariable(CMD_GET_SIM_VARIABLE, POSITION_CONVERSION, "",tmp);


    tcpip::Storage inMsgX;
    try {
        std::string acknowledgement;
        check_resultState(inMsgX, CMD_GET_SIM_VARIABLE, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
	std::cout<<"get V2X_getRealPosition error"<<std::endl;

    }
    check_commandGetResult(inMsgX, CMD_GET_SIM_VARIABLE, -1, false);
    // report result state
    try {

        int variableID = inMsgX.readUnsignedByte();
        std::string objectID = inMsgX.readString();

        int valueDataType = inMsgX.readUnsignedByte();

        position.longitude = inMsgX.readDouble();
        position.latitude = inMsgX.readDouble();

    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_getRealPosition error"<<std::endl;

    }

    return position;
}

void TraCITestClient::V2X_setLane(std::string vehicleID,std::string roadName)
{
    int domID=0xc4;
    int varID=0x31;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_STRING);
    tmp.writeString(roadName);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_setLane error"<<std::endl;
    }
}

void TraCITestClient::V2X_disableAutoLaneChange(std::string vehicleID)
{
	int domID=0xc4;
    int varID=0xb6;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_INTEGER);
    tmp.writeInt(0);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
		std::cout<<"get V2X_disableAutoLaneChange error"<<std::endl;
    }
}


void TraCITestClient::V2X_setSpeedMode(std::string vehicleID)
{
	int domID=0xc4;
    int varID=0xb3;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_INTEGER);
    tmp.writeInt(0);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
    
    } catch (tcpip::SocketException& e) {
       std::cout<<"get V2X_setSpeedMode error"<<std::endl;
    }
}

void
TraCITestClient::commandClose() {
    send_commandClose();
    try {
        tcpip::Storage inMsg;
        std::string acknowledgement;
        check_resultState(inMsg, CMD_CLOSE, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
		std::cout<<"get commandClose error"<<std::endl;
    }
}

TraCITestClient::~TraCITestClient() {

	//close the socket
}
