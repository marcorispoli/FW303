
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

    typedef struct lightStruct{
        int timer;
        bool status;
    }LIGHT_STRUCT_t;
    
    ext void mirrorInit(void); //!< Module initialization function
    ext _MOTOR_COMMAND_RETURN_t activateMirror(int index);//!< Mirror activation command to index 
    ext void lightActivation(bool status);
    ext void timeLightActivation(bool status, int timer);
    ext void light1sLoop(void);
    ext bool isLightOn(void);


#endif // _MOT_MIRROR_H