# AMFITRACK C++ SDK



## How to use:
This is a code snippet of how the AMFITRACK C++ SDK is started

    /* Create instance of amfitrack */ 
    AMFITRACK& AMFITRACK = AMFITRACK::getInstance(); 
    /* Initializes the USB and connects to the devices */
    AMFITRACK.initialize_amfitrack();
    /* Starts the main thread, that read the data */
    AMFITRACK.start_amfitrack_task();


## Tested with:

- MSVC Compiler (Version 14.37)
- GNU/GCC Compiler (Version 13.2)