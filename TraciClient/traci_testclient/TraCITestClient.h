/****************************************************************************/
/// @file    TraCITestClient.h
/// @author  Friedemann Wesner
/// @author  Daniel Krajzewicz
/// @author  Axel Wegener
/// @author  Michael Behrisch
/// @date    2008/04/07
/// @version $Id$
///
/// A test execution class
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2001-2016 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef TRACITESTCLIENT_H
#define TRACITESTCLIENT_H

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include "../config.h"
#endif

#include <string>
#include <sstream>//?
#include <vector>

#include "../foreign/tcpip/socket.h"
#include "../utils/common/SUMOTime.h"
#include "../utils/traci/TraCIAPI.h"


// ===========================================================================
// class definitions
// ===========================================================================

struct Coordinates
{
   	double latitude;
  	double longitude;
};

class TraCITestClient : public TraCIAPI {
private:
	 
	 const int port = 8800;
	 const std::string host = "localhost"; //ip address of SUMO Server 

protected: //?

    //void commandClose();//?

public:

    TraCIAPI TraCIAPI_Obj;
	
	Coordinates position;
	
    void V2X_run();	
	
	void V2X_simulationEngine(double);//handle end time
   
    Coordinates V2X_getRealPosition(std::string vehicleID);//modify it for only getting lat long
	
	void V2X_disableAutoLaneChange(std::string vehicleID);
	
    void V2X_setSpeedMode(std::string vehicleID);//disables the speed down at junctions
	
	void V2X_setLane(std::string vehicleID,std::string roadName);
	
	void commandClose();
	
	//Functions not utilized in ICW demo
  /*double V2X_getSpeed(std::string vehicleID);

    void V2X_stopVehicle(std::string vehicleID,std::string roadName,double positionOnLane,int laneID,int stopDuration);

    void V2X_resumeVehicle(std::string vehicleID);

    void V2X_slowDown(std::string vehicleID,double speed,int effectTime);

    void V2X_changeSpeed(std::string vehicleID,double speed);//this command is little tricky needs to be given twice

    void V2X_subscribeVehicleData(std::string vehicleID,std::string beginTime,std::string endTime);

    struct subscribedVehicleData
    {
    	double speed;
    	double angle;
    	double lanePosition;
    	Position pos_data;
    	std::string vehicleName;
    };
	
    subscribedVehicleData subscribed_Vehicle_Data;

    std::vector<subscribedVehicleData> subscribed_Vehicle_Data_Information;

    subscribedVehicleData V2X_getsubscribedVehicleData();

	bool V2X_validateSubscription(tcpip::Storage& inMsg);
	
	 std::string V2X_getVehicleID(std::string vehicleID);

	*/
	 ~TraCITestClient();

};

#endif
