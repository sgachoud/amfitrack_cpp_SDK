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

#ifndef LIB_GENERIC_PARAMETER_LIB_GENERIC_PARAMETER_H_
#define LIB_GENERIC_PARAMETER_LIB_GENERIC_PARAMETER_H_


#ifdef _MSC_VER
#define __PACKED_STRUCT struct 
#define __weak  
#define __packed
#pragma pack(1)
#elif defined(__MINGW32__) ||  defined(__MINGW64__)
#define __PACKED_STRUCT_GENERIC struct __attribute__((packed))
#define __weak  
#define __packed __attribute__((packed))
#else
#define __PACKED_STRUCT struct
#define __weak
#define __PACKED __attribute__((packed))
#endif

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#ifdef USE_PROJECT_CONF
#include "project_conf.h"
#endif

#ifdef __cplusplus
#include <cstdint>
#else
/// \cond
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef __PACKED_STRUCT
//#include <cmsis_compiler.h>
#endif
/// \endcond
#endif	// __cplusplus

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Type declarations
//-----------------------------------------------------------------------------
typedef struct lib_Generic_Parameter_Value lib_Generic_Parameter_Value_t;

typedef enum
{
    lib_Generic_Parameter_Type_void = 0,
	lib_Generic_Parameter_Type_char = 1,
	lib_Generic_Parameter_Type_s8 = 2,
	lib_Generic_Parameter_Type_u8 = 3,

	lib_Generic_Parameter_Type_s16LE = 4,
	lib_Generic_Parameter_Type_s16BE = 5,
	lib_Generic_Parameter_Type_u16LE = 6,
	lib_Generic_Parameter_Type_u16BE = 7,

	lib_Generic_Parameter_Type_s32LE = 8,
	lib_Generic_Parameter_Type_s32BE = 9,
	lib_Generic_Parameter_Type_u32LE = 10,
	lib_Generic_Parameter_Type_u32BE = 11,

	lib_Generic_Parameter_Type_s64LE = 12,
	lib_Generic_Parameter_Type_s64BE = 13,
	lib_Generic_Parameter_Type_u64LE = 14,
	lib_Generic_Parameter_Type_u64BE = 15,

	lib_Generic_Parameter_Type_f32LE = 16,
	lib_Generic_Parameter_Type_f32BE = 17,
	lib_Generic_Parameter_Type_f64LE = 18,
	lib_Generic_Parameter_Type_f64BE = 19,

    lib_Generic_Parameter_Type_bool = 20,

	lib_Generic_Parameter_Type_ProcedureCall = 100,
} lib_Generic_Parameter_Type_t;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
bool lib_Generic_Parameter_ValueIsEqual(lib_Generic_Parameter_Value_t v1, lib_Generic_Parameter_Value_t v2);
uint8_t lib_Generic_Parameter_SerializeValueAndType(lib_Generic_Parameter_Value_t value, void *pDest, uint8_t maxLength);	// Returns number of bytes written, 0 if there's not enough room in destination for the parameter
uint8_t lib_Generic_Parameter_SizeWithType(lib_Generic_Parameter_Value_t value);

//-----------------------------------------------------------------------------
// Variables and constants
//-----------------------------------------------------------------------------
__PACKED_STRUCT_GENERIC lib_Generic_Parameter_Value
{
	uint8_t type;
	union {
		bool b;

		char ch;

		int8_t s8;
		uint8_t u8;

		int16_t s16;
		uint16_t u16;

		int32_t s32;
		uint32_t u32;

        int64_t s64;
        uint64_t u64;

		float f32;
		double f64;
	};
};

#ifdef __cplusplus
} // extern "C" end
#endif

#endif
