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
class AMFITRACK_Devices
{
public:
	static AMFITRACK_Devices& getInstance()
	{
		static AMFITRACK_Devices instance;
		return instance;
	}

	void initialize_amfitrack(void);
	void start_amfitrack_task(void);
	void stop_amfitrack_task(void);

	void setDeviceName(uint8_t DeviceID, char* name, uint8_t length);
	void setDeviceActive(uint8_t DeviceID);
	bool getDeviceActive(uint8_t DeviceID);
	void setDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t Pose);
	void getDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t* Pose);

private:
	char Name[MAX_NUMBER_OF_DEVICES][MAX_NAME_LENGTH]; // Array of character arrays to store device names
	bool DeviceActive[MAX_NUMBER_OF_DEVICES];

	lib_AmfiProt_Amfitrack_Pose_t Position[MAX_NUMBER_OF_DEVICES];
	uint8_t FieldQualityPose[MAX_NUMBER_OF_DEVICES];


	static void background_amfitrack_task(AMFITRACK_Devices*);

	AMFITRACK_Devices();
	~AMFITRACK_Devices();
	
};


#endif //AMFITRACK_DEVICES_HPP_