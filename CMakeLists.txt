# CMakeList.txt : CMake project for Amfitrack_Cpp_SDK, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Amfitrack_API")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/amfiprotapi)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/lib_crc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/lib_generic_parameter)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/lib_macro)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/lib_queue)

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit
        set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output/MSVC/x64)
        set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x64)
    else()
        # 32-bit
        set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output/MSVC/x86)
        set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x86)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit
        set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output/GCC/x64)
        set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x64)
    else()
        # 32-bit
        set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output/GCC/x86)
        set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x86)
    endif()
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})

# Source files
set(SOURCES
    Amfitrack.cpp
    src/AmfitrackNode.cpp
    src/usb_connection.cpp
    lib/amfiprotapi/lib_AmfiProt.cpp
    lib/amfiprotapi/lib_AmfiProt_Amfitrack.cpp
    lib/amfiprotapi/lib_AmfiProt_API.cpp
    lib/lib_crc/lib_CRC.c
    lib/lib_generic_parameter/lib_Generic_Parameter.c
    lib/lib_queue/lib_Queue.c
)

# Header files
set(HEADERS
    Amfitrack.hpp
    src/AmfitrackNode.h
    src/usb_connection.h
    src/project_conf.h
    lib/amfiprotapi/lib_AmfiProt.hpp
    lib/amfiprotapi/lib_AmfiProt_Amfitrack.hpp
    lib/amfiprotapi/lib_AmfiProt_API.hpp
    lib/lib_crc/lib_CRC.h
    lib/hidapi/hidapi.h
    lib/lib_generic_parameter/lib_Generic_Parameter.h
    lib/lib_macro/lib_MACRO.h
    lib/lib_queue/lib_Queue.h
)

# Add library
add_library(Amfitrack_API ${SOURCES} ${HEADERS})

# Determine architecture and set hidapi paths
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit
    set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x64)
else()
    # 32-bit
    set(HIDAPI_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/hidapi/x86)
endif()

find_library(HIDAPI_LIB NAMES hidapi PATHS ${HIDAPI_LIB_PATH})

if (NOT HIDAPI_LIB)
    message(FATAL_ERROR "hidapi library not found in ${HIDAPI_LIB_PATH}")
else()
    message(STATUS "hidapi library found at: ${HIDAPI_LIB}")
endif()

# Link against the hidapi library
target_link_libraries(Amfitrack_API PRIVATE ${HIDAPI_LIB})

# Add custom command to copy DLL to output directory
add_custom_command(TARGET Amfitrack_API POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${HIDAPI_LIB_PATH}/hidapi.dll
    $<TARGET_FILE_DIR:Amfitrack_API>
    COMMENT "Copying hidapi.dll to the output directory"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Amfitrack_API PROPERTY CXX_STANDARD 20)
endif()

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Add MSVC-specific options here
  target_compile_options(Amfitrack_API PRIVATE /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Add G++-specific options here
  target_compile_options(Amfitrack_API PRIVATE -Wall -Wextra -pedantic)
endif()
