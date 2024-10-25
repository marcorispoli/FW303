
#ifndef _MOT_FORMAT_H    
#define _MOT_FORMAT_H

#include "definitions.h"
#include "motlib.h"

#undef ext
#undef ext_static

#ifdef _MOT_FORMAT_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

        ext void formatInit(void); //!< Module initialization function
        ext _MOTOR_COMMAND_RETURN_t activateFormatCollimation(int index);//!< format activation command to index 



#endif // _MOT_FORMAT_H