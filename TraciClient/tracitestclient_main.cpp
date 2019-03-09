/****************************************************************************/
/// @file    tracitestclient_main.h
/// @author  Friedemann Wesner
/// @author  Axel Wegener
/// @author  Michael Behrisch
/// @author  Daniel Krajzewicz
/// @date    2008/04/07
/// @version $Id$
///
/// Main method for TraCITestClient
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


#ifdef _MSC_VER
#include <windows_config.h>
#else
#include "config.h"
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include "traci_testclient/TraCITestClient.h"


int main()
{

	TraCITestClient client;
	client.V2X_run();

	return 0;
}

