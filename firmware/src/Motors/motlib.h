
#ifndef _MOTLIB_H    
#define _MOTLIB_H

#include "definitions.h"

#undef ext
#undef ext_static

#ifdef _MOTLIB_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

typedef enum{
    MOT_ILIM_HIGH = 0,  // 1.52V
    MOT_ILIM_LOW,       // 0.28V
    MOT_ILIM_MED,       // 1.39V       
    MOT_ILIM_DISABLE    // 0
}MOT_ILIM_MODE_t;

typedef enum{
    MOT_uSTEP_1 = 0,
    MOT_uSTEP_2,
    MOT_uSTEP_4,
    MOT_uSTEP_16
}MOT_MICROSTEP_t;

typedef enum{
    MOT_DIRCW = 0,
    MOT_DIRCCW
}MOT_DIRECTION_t;

typedef enum{
    MOT_ENA_ON = 0,
    MOT_ENA_OFF
}MOT_ENA_t;

typedef enum{
    MOT_RST_ON = 0,
    MOT_RST_OFF
}MOT_RST_t;

typedef enum{
    MOT_ENASTEP_OFF = 0,
    MOT_ENASTEP_ON
}MOT_ENASTEP_t;


#define MOTOR_SLEEP_OFF MOT_SLEEP_Set()
#define MOTOR_SLEEP_ON MOT_SLEEP_Clear()

#define MOTOR_LEN 5
typedef enum{
    MOTOR_M1 = 0,
    MOTOR_M2,
    MOTOR_M3,
    MOTOR_M4,
    MOTOR_M5,        
}_MOTOR_ID_t;

typedef struct{
    unsigned char ILIM:2;
    unsigned char uSTEP:2;
    unsigned char DIR:1;
    unsigned char MOTENA:1;
    unsigned char RST:1;
    unsigned char ENASTEP:1;
}_MOTOR_DATA_t;

ext _MOTOR_DATA_t motor_latch[MOTOR_LEN];
ext void motorsInitialize(void);
ext void setLatch(_MOTOR_ID_t motid);
ext void enableStep(_MOTOR_ID_t motid, bool enable);
ext void motorOn(_MOTOR_ID_t motid, MOT_ILIM_MODE_t ilim, MOT_MICROSTEP_t ustep, MOT_DIRECTION_t dir );
ext void motorOff(_MOTOR_ID_t motid, MOT_ILIM_MODE_t ilim);

#endif // _MOTLIB_H