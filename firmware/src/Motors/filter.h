
#ifndef _MOT_FILTER_H    
#define _MOT_FILTER_H

#include "definitions.h"
#include "motlib.h"

#undef ext
#undef ext_static

#ifdef _MOT_FILTER_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif
        ext void filterInit(void); //!< Module initialization function
        ext _MOTOR_COMMAND_RETURN_t activateFilter(int index);//!< Filter activation command to index 
        

#endif // _MOT_FILTER_H