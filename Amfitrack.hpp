//-----------------------------------------------------------------------------
//                              AMFITECH APS
//                          ALL RIGHTS RESERVED
//
// $URL: $
// $Rev: $
// $Date: $
// $Author: $
//
// Description
// TODO Write a description here
//
//-----------------------------------------------------------------------------

#ifndef AMFITRACK_DEVICES_HPP_
#define AMFITRACK_DEVICES_HPP_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <memory>
#include <thread>
#include "lib/amfiprotapi/lib_AmfiProt_API.hpp"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MAX_NAME_LENGTH 64
#define MAX_NUMBER_OF_DEVICES 254
//-----------------------------------------------------------------------------
// Type declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables and constants
//-----------------------------------------------------------------------------
class AMFITRACK
{
public:
	static AMFITRACK& getInstance()
	{
		static AMFITRACK instance;
		return instance;
	}

	/* Call first to initialize USB and connect to devices */
	void initialize_amfitrack(void);

	/* Starts the main thread, that reads data from all connected devices */
	void start_amfitrack_task(void);

	/* Stops the main thread */
	void stop_amfitrack_task(void);

	void setDeviceName(uint8_t DeviceID, char* name, uint8_t length);

	void setDeviceActive(uint8_t DeviceID);
	/* Return if a device is connected */
	bool getDeviceActive(uint8_t DeviceID);

	void setDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t Pose);
	/* Get the pose for a specific device */
	void getDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t* Pose);

private:
	char Name[MAX_NUMBER_OF_DEVICES][MAX_NAME_LENGTH]; // Array of character arrays to store device names
	bool DeviceActive[MAX_NUMBER_OF_DEVICES];
	time_t DeviceLastTimeSeen[MAX_NUMBER_OF_DEVICES];

	lib_AmfiProt_Amfitrack_Pose_t Position[MAX_NUMBER_OF_DEVICES];


	static void background_amfitrack_task(AMFITRACK*);
	void checkDeviceDisconnected(uint8_t DeviceID);

	AMFITRACK();
	~AMFITRACK();
};


#endif //AMFITRACK_DEVICES_HPP_