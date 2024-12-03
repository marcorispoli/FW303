
#ifndef _MOT_FILTER_H    
#define _MOT_FILTER_H

#include "definitions.h"
#include "motlib.h"

#undef ext
#undef ext_static

/*!
 * \defgroup FILTER Filter Management Module
 * 
 * # Filter Layout
 * 
 * The following filter layout shall be considered in the selection process:
 *      
 * \image html FilterLayout.bmp
 */ 
 
#ifdef _MOT_FILTER_C
    #define ext
    #define ext_static static 
    // 404, 322, 247, 166,84
    #define  SLOT0_MAX_STEP 440
    #define  SLOT0_MIN_STEP 359
    #define  SLOT1_MAX_STEP 360
    #define  SLOT1_MIN_STEP 281
    #define  SLOT2_MAX_STEP 280
    #define  SLOT2_MIN_STEP 201
    #define  SLOT3_MAX_STEP 200
    #define  SLOT3_MIN_STEP 121
    #define  SLOT4_MAX_STEP 120
    #define  SLOT4_MIN_STEP 40


    

#else
    #define ext extern
    #define ext_static extern
#endif
        ext void filterInit(void); //!< Module initialization function
        ext void abortFilter(void);
        ext _MOTOR_COMMAND_RETURN_t activateFilter(int index, bool force);//!< Filter activation command to index 
        

#endif // _MOT_FILTER_H