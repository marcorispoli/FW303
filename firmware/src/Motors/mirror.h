
#ifndef _MOT_MIRROR_H    
#define _MOT_MIRROR_H

#include "definitions.h"
#include "motlib.h"

#undef ext
#undef ext_static

#ifdef _MOT_MIRROR_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

    ext void mirrorInit(void); //!< Module initialization function
    ext _MOTOR_COMMAND_RETURN_t activateMirror(int index);//!< Mirror activation command to index 
        


#endif // _MOT_MIRROR_H