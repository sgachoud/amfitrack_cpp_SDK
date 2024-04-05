#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "lib_AmfiProt_API.hpp"
#include "Amfitrack.hpp"
#include "src/usb_connection.h"
#include <process.h>

using namespace std;

//#define AMFITRACK_DEBUG_INFO

static bool stop_running = false;

AMFITRACK::AMFITRACK()
{
    // Initialize Name with null characters
    for (int i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        memset(Name[i], 0, MAX_NAME_LENGTH);
        DeviceActive[i] = false;
        Position[i] = { .position_x_in_m = 0, .position_y_in_m = 0, .position_z_in_m = 0, .orientation_x = 0, .orientation_y = 0, .orientation_z = 0, .orientation_w = 0 };
    }
}


AMFITRACK::~AMFITRACK()
{

}

void AMFITRACK::background_amfitrack_task(AMFITRACK* inst)
{
    /* Creates instance of USB */
    usb_connection& usb = usb_connection::getInstance();
    AmfiProt_API& amfiprot_api = AmfiProt_API::getInstance();

#ifdef AMFITRACK_DEBUG_INFO
    std::cout << "Background thread started!" << std::endl;
#endif // AMFITRACK_DEBUG_INFO

    while (!stop_running)
    {
        usb.usb_run();
        amfiprot_api.amfiprot_run();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void AMFITRACK::start_amfitrack_task(void)
{
    stop_running = false;

#ifdef AMFITRACK_DEBUG_INFO
    std::cout << "Starting Background thread!" << std::endl;
#endif // AMFITRACK_DEBUG_INFO
    
    // Create a thread object
    std::thread background_thread(background_amfitrack_task, this);

    background_thread.detach();
}

void AMFITRACK::stop_amfitrack_task(void)
{
    stop_running = true;
}

void AMFITRACK::initialize_amfitrack(void)
{
    usb_connection& usb = usb_connection::getInstance();
    AmfiProt_API& amfiprot_api = AmfiProt_API::getInstance();
    /* Initialize USB conenction */
    usb.usb_init();
}

void AMFITRACK::setDeviceName(uint8_t DeviceID, char* name, uint8_t length)
{
    // Check for valid device ID and name length
    if (length >= MAX_NAME_LENGTH) return;
    for (uint8_t i = 0; i < length; i++)
    {
        Name[DeviceID][i] = name[i];
    }
    Name[DeviceID][length] = '\0'; // Ensure null termination
    
#ifdef AMFITRACK_DEBUG_INFO
    std::cout << Name[DeviceID] << std::endl;
#endif // AMFITRACK_DEBUG_INFO
}

void AMFITRACK::setDeviceActive(uint8_t DeviceID)
{
    DeviceActive[DeviceID] = true;
#ifdef AMFITRACK_DEBUG_INFO
    std::cout << "Device " << DeviceID << " is active" << std::endl;
#endif // AMFITRACK_DEBUG_INFO
    
}

bool AMFITRACK::getDeviceActive(uint8_t DeviceID)
{
    return DeviceActive[DeviceID];
}

void AMFITRACK::setDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t Pose)
{
    memcpy(&Position[DeviceID], &Pose, sizeof(lib_AmfiProt_Amfitrack_Pose_t));
#ifdef AMFITRACK_DEBUG_INFO
    std::cout << "Pose set!" << std::endl;
    //printf("Pose X %.3f | Y %.3f | Z %.3f \n", this->Pose[DeviceID].position_x_in_m, this->Pose[DeviceID].position_y_in_m, this->Pose[DeviceID].position_z_in_m);
#endif // AMFITRACK_DEBUG_INFO
    
}

void AMFITRACK::getDevicePose(uint8_t DeviceID, lib_AmfiProt_Amfitrack_Pose_t* Pose)
{
    if (!getDeviceActive(DeviceID)) return;
    memcpy(Pose, &Position[DeviceID], sizeof(lib_AmfiProt_Amfitrack_Pose_t));
}


void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SourceCalibration(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SourceMeasurement(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SensorMeasurement(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    AMFITRACK& AMFITRACK = AMFITRACK::getInstance();
    lib_AmfiProt_Amfitrack_Sensor_Measurement_t SensorMeasurement;
    memcpy(&SensorMeasurement, &frame->payload[0], sizeof(lib_AmfiProt_Amfitrack_Sensor_Measurement_t));
    lib_AmfiProt_Amfitrack_Pose_t tempPose;
    lib_AmfiProt_Amfitrack_decode_pose_i24(&SensorMeasurement.pose, &tempPose);
    AMFITRACK.setDevicePose(frame->header.source, tempPose);
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_RawBfield(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_NormalizedBfield(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_BfieldPhase(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_NormalizedBfieldImu(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SignData(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_PllData(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_RawFloats(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SetPhaseModulation(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_SourceCoilCalData(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::lib_AmfiProt_Amfitrack_handle_AlternativeProcessing(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}


void AmfiProt_API::libAmfiProt_handle_RequestProcedureSpec(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ReplyProcedureSpec(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestProcedureCall(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ReplyProcedureCall(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestDeviceID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RespondDeviceID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    AMFITRACK& AMFITRACK = AMFITRACK::getInstance();
    AMFITRACK.setDeviceActive(frame->payload[1]);
}

void AmfiProt_API::libAmfiProt_handle_SetTxID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestFirmwareVersion(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_FirmwareVersion(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_FirmwareStart(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_FirmwareData(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_FirmwareEnd(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestDeviceName(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ReplyDeviceName(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    AMFITRACK& AMFITRACK = AMFITRACK::getInstance();
    size_t str_length = strnlen((char*)(&(frame->payload[1])), MAX_PAYLOAD_SIZE - 1);
    AMFITRACK.setDeviceName(frame->header.destination, (char*)(&(frame->payload[1])), str_length);
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationValue(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ReplyConfigurationValue(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_SetConfigurationValue(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationName(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ReplyConfigurationName(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_LoadDefault(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_SaveAsDefault(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationNameAndUID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ConfigurationNameAndUID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationValueUID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ConfigurationValueUID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_SetConfigurationValueUID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationCategory(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ConfigurationCategory(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestConfigurationValueCount(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ConfigurationValueCount(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestCategoryCount(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_CategoryCount(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_Reboot(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_DebugOutput(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_ResetParameter(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}

void AmfiProt_API::libAmfiProt_handle_RequestFirmwareVersionPerID(void* handle, lib_AmfiProt_Frame_t* frame, void* routing_handle)
{
    /* NOTE: Overwrite in application-specific library */
}




