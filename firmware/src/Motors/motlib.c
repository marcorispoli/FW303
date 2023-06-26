
#define _MOTLIB_C

#include "application.h"
#include "motlib.h"

static volatile bool LATCH_SYNC = true;
static void latch_callback(TC_TIMER_STATUS status, uintptr_t context){
    TC0_TimerStop();
    LATCH_SYNC = true;    
}




void setLatch(_MOTOR_ID_t motid){
    
    // Wait for a pending LATCH    
    while(!LATCH_SYNC) ;
    LATCH_SYNC = false;
    
    // Set the BUS value before to latch the data to the target motor 
    unsigned char bus = *((unsigned char*) &motor_latch[motid]);
    if(bus & 0x1 ) uc_IA_Set();
    else uc_IA_Clear();
    if(bus & 0x2 ) uc_IB_Set();
    else uc_IB_Clear();
    if(bus & 0x4 ) uc_MS1_Set();
    else uc_MS1_Clear();
    if(bus & 0x8 ) uc_MS2_Set();
    else uc_MS2_Clear();
    if(bus & 0x10 ) uc_DIR_Set();
    else uc_DIR_Clear();
    if(bus & 0x20 ) uc_ENA_Set();
    else uc_ENA_Clear();
    if(bus & 0x40 ) uc_RST_Set();
    else uc_RST_Clear();
    if(bus & 0x80 ) uc_ENASTEP_Set();
    else uc_ENASTEP_Clear();
     
    // Latches the target motor    
    switch(motid){
        case MOTOR_M1: 
            uc_LATCH_M1_Set();
            TC0_TimerStart();
            while(LATCH_SYNC);
            uc_LATCH_M1_Clear();
        break;
        case MOTOR_M2: 
            uc_LATCH_M2_Set();
            TC0_TimerStart();
            while(!LATCH_SYNC) ;
            uc_LATCH_M2_Clear();
        break;
        case MOTOR_M3: 
            uc_LATCH_M3_Set();
            TC0_TimerStart();
            while(!LATCH_SYNC) ;
            uc_LATCH_M3_Clear();
        break;
        case MOTOR_M4: 
            uc_LATCH_M4_Set();
            TC0_TimerStart();
            while(!LATCH_SYNC) ;
            uc_LATCH_M4_Clear();
        break;
        case MOTOR_M5: 
            uc_LATCH_M5_Set();
            TC0_TimerStart();
            while(!LATCH_SYNC) ;
            uc_LATCH_M5_Clear();
        break;
    }
}

void enableStep(_MOTOR_ID_t motid, bool enable){
    if(motid > MOTOR_M5) return;
    
    // Enables/Disables the motor bus data
    if(enable) motor_latch[motid].ENASTEP = MOT_ENASTEP_ON;
    else motor_latch[motid].ENASTEP = MOT_ENASTEP_OFF;
    
    // Latches 
    setLatch(motid);
    
}

void motorOn(_MOTOR_ID_t motid, MOT_ILIM_MODE_t ilim, MOT_MICROSTEP_t ustep, MOT_DIRECTION_t dir ){
    if(motid > MOTOR_M5) return;
    
    motor_latch[motid].ILIM =   ilim;
    motor_latch[motid].uSTEP =  ustep;
    motor_latch[motid].DIR = dir;
     
    motor_latch[motid].MOTENA = MOT_ENA_ON;
    motor_latch[motid].RST =    MOT_RST_OFF;
    motor_latch[motid].ENASTEP = MOT_ENASTEP_ON;
            
    // Latches 
    setLatch(motid);
    
}
void motorOff(_MOTOR_ID_t motid, MOT_ILIM_MODE_t ilim){
    if(motid > MOTOR_M5) return;    
    //if(ilim > MOT_ILIM_LOW) ilim = MOT_ILIM_LOW;
    
    motor_latch[motid].ILIM =   ilim;     
    motor_latch[motid].MOTENA = MOT_ENA_ON;
    motor_latch[motid].RST =    MOT_RST_OFF;
    motor_latch[motid].ENASTEP = MOT_ENASTEP_OFF;
            
    // Latches 
    setLatch(motid);
    
}

void motorsInitialize(void){
    TC0_TimerCallbackRegister(latch_callback,0);    
    LATCH_SYNC = true;
    
    // Latch reset High
    uc_LATCH_CLR_Set();
    MOTOR_SLEEP_OFF;
            
    // Reset Latches
    uc_LATCH_M1_Clear();
    uc_LATCH_M2_Clear();
    uc_LATCH_M3_Clear();
    uc_LATCH_M4_Clear();
    uc_LATCH_M5_Clear();
    
    // Reset Steps
    uc_STEP_M1_Clear();
    uc_STEP_M2_Clear();
    uc_STEP_M3_Clear();
    uc_STEP_M4_Clear();
    uc_STEP_M5_Clear();
    
    for(int i=0; i< MOTOR_LEN; i++){ 
        motor_latch[i].ILIM =   MOT_ILIM_DISABLE;
        motor_latch[i].uSTEP =  MOT_uSTEP_1;
        motor_latch[i].DIR =    MOT_DIRCW;
        motor_latch[i].MOTENA = MOT_ENA_OFF;
        motor_latch[i].RST =    MOT_RST_ON;
        motor_latch[i].ENASTEP = MOT_ENASTEP_OFF;
        setLatch(i);
    }
    

}




