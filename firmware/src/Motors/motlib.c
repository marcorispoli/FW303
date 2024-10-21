#define _MOTLIB_C

#include "application.h"
#include "motlib.h"


 /**
     * \defgroup privateModuleMembers Private module members
     * \ingroup motorModule
     * This section describes private module member data and functions
     * 
     *  @{
     */
        #define LATCH_US_PULSE 10 //!< Defines the Latch pulse period in 1us units
        #define MOTOR_SLEEP_OFF uC_MOT_SLEEP_Set() //!<  Exit the Motor Sleep Mode macro
        #define MOTOR_SLEEP_ON uC_MOT_SLEEP_Clear() //!<  Set the Motor  Sleep Mode macro

        static volatile bool wait_flag; //!< This is the flag used for the 1us waiting routine
        
        
        static void latch_1us_callback(TC_TIMER_STATUS status, uintptr_t context); //!< This is the 1us TC0 callback routine
        static void wait_us(unsigned short utime); //!< This is the delay routine based on the 1us TC0 interrupt
        static void setLatch(_MOTOR_ID_t motid); //!< This is the Latch Pulse routine
         
    /// @}   privateModuleMembers





/**
 * This callback is called after 1us from the TCO-Start event.
 * 
 * The TC0 is set to One-Shot Timer mode: when started it rise a 
 * single callback event after 1us. This callback is used to 
 * implement the wait_us routine.
 * 
 * 
 * @param status see TC_TIMER_STATUS for details. (not used)
 * @param context this is a pointer to a contest. (not used)
 */
void latch_1us_callback(TC_TIMER_STATUS status, uintptr_t context){
    wait_flag = true;  
}

/**
 * This is the internal delay routine with 1us unit.
 * 
 * The routine can provide from 1 to 65536 us delay time.
 * 
 * This routine is not reentrant so it cannot be called 
 * inside an interrupt routine.
 * 
 * @param utime micro seconds delay time
 */
void wait_us(unsigned short utime){
    wait_flag = false;
    for(int i=0; i < utime; i++){
        TC0_TimerStart();
        while(!wait_flag);
    }  
    return;
}

void manageMotorLatch(void){
    for(int i=0; i <= LAST_MOTOR_ID; i++){
        if(!motor_latch_request[i]) continue;
        setLatch(i);
        motor_latch_request[i] = false;
    }
}

void requestLatch(_MOTOR_ID_t motid){
    motor_latch_request[motid] = true;
}


/**
 * This function Latches the Motor Bus data to the Motor driver.
 * 
 * The function evaluates the content of the  motor_latch[] 
 * variable, setting the BUS with the targeted motor bus data;
 * 
 * The function will latch the motor bus data pulsing (10us pulse)
 * the motor latch trigger line (uc_LATCH_Mx pin).  
 *  
 * @param motid this is the Motor identifier. (See _MOTOR_ID_t)
 */
void setLatch(_MOTOR_ID_t motid){
        
    // Set the BUS value before to latch the data to the target motor 
    unsigned char bus = *((unsigned char*) &motor_latch[motid]);
    if(bus & 0x1 ) uC_IA_Set();
    else uC_IA_Clear();
    if(bus & 0x2 ) uC_IB_Set();
    else uC_IB_Clear();
    if(bus & 0x4 ) uC_MS1_Set();
    else uC_MS1_Clear();
    if(bus & 0x8 ) uC_MS2_Set();
    else uC_MS2_Clear();
    if(bus & 0x10 ) uC_DIR_Set();
    else uC_DIR_Clear();
    if(bus & 0x20 ) uC_ENA_Set();
    else uC_ENA_Clear();
    if(bus & 0x40 ) uC_RST_Set();
    else uC_RST_Clear();
    if(bus & 0x80 ) uC_ENASTEP_Set();
    else uC_ENASTEP_Clear();
     
    // Latches the target motor    
    switch(motid){
        case MOTOR_LEFT_ID: 
            uC_LATCH_LEFT_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_LEFT_Clear();
        break;
        case MOTOR_RIGHT_ID: 
            uC_LATCH_RIGHT_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_RIGHT_Clear();
        break;
        case MOTOR_FRONT_ID: 
            uC_LATCH_FRONT_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_FRONT_Clear();
        break;
        case MOTOR_BACK_ID: 
            uC_LATCH_BACK_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_BACK_Clear();
        break;
        case MOTOR_TRAP_ID: 
            uC_LATCH_TRAP_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_TRAP_Clear();
        break;
        case MOTOR_MIRROR_ID: 
            uC_LATCH_MIRROR_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_MIRROR_Clear();
        break;
        case MOTOR_FILTER_ID: 
            uC_LATCH_FILTER_Set();
            wait_us(LATCH_US_PULSE);
            uC_LATCH_FILTER_Clear();
        break;
    }
}

/**
 * This module activates a target motor with the following modes:
 * + Torque: set the torque to be used;
 * + ustep: set the pulse entity to a micro-step mode;
 * + dir: set the activation direction;
 * 
 * The Function predisposes the motor to be moved. However only 
 * pulsing the uc_STEP_Mx pin will rotate the motor axes in the selected direction.
 * 
 * 
 * @param motid Selects the target motor from MOTOR_M1 to MOTOR_M5;
 * @param torque Selects the torque to be applied;
 * @param ustep Selects the value of a Pulse;
 * @param dir Selects the direction rotation;
 */
void motorOn(MOTOR_STRUCT_t* mot, MOT_ILIM_MODE_t torque, MOT_DIRECTION_t dir){
    
    // Clear the Step pin
    motorStep(mot, false);
    
    // Sets the motor bus lines
    motor_latch[mot->id].ILIM =   torque;
    motor_latch[mot->id].uSTEP =  mot->stepping_mode;
    motor_latch[mot->id].DIR = dir;
     
    motor_latch[mot->id].MOTENA = MOT_ENA_ON;
    motor_latch[mot->id].RST =    MOT_RST_OFF;
    motor_latch[mot->id].ENASTEP = MOT_ENASTEP_ON;
            
    requestLatch(mot->id);
 
}

/**
 * This function sets the motor in Holding mode.
 * 
 * When the Motor is set in holding mode, it remains enabled 
 * and the motor phases remain powered with the requested torque.
 * 
 * The Torque can be set only MOT_TORQUE_LOW or MOT_TORQUE_DISABLED 
 * to prevent overheating.
 * 
 * The Motor Steps are disabled in this mode and the motor axe cannot 
 * be activated.
 * 
 * 
 * @param motid Selects the target motor from MOTOR_M1 to MOTOR_M5; 
 * @param torque Selects the torque to be applied (limited value);
 */
void motorHold(MOTOR_STRUCT_t* mot, MOT_ILIM_MODE_t torque){
    
    
    // Limits the torque applicable to a LOW or DISABLE value
    if(torque > MOT_TORQUE_LOW) torque = MOT_TORQUE_LOW;
    
    // Set the motor bus lines
    motor_latch[mot->id].ILIM =   torque;     
    motor_latch[mot->id].MOTENA = MOT_ENA_ON;
    motor_latch[mot->id].RST =    MOT_RST_OFF;
    motor_latch[mot->id].ENASTEP = MOT_ENASTEP_OFF;
            
    // Latches 
    requestLatch(mot->id);
    
}

/**
 * This function sets the motor in Disable mode.
 * 
 * When the Motor is set in disable mode, the motor driver is disabled
 * and the step sequencer is reset. No torque is applied!
 *  
 * @param motid Selects the target motor from MOTOR_M1 to MOTOR_M5; 
 */
void motorDisable(MOTOR_STRUCT_t* mot){
    
    // Clears the step pin
    motorStep(mot, false);
    
     // Set the motor bus lines
    motor_latch[mot->id].MOTENA = MOT_ENA_OFF;
    motor_latch[mot->id].RST =    MOT_RST_ON;
    motor_latch[mot->id].ENASTEP = MOT_ENASTEP_OFF;
            
    // Latches 
    requestLatch(mot->id);
}

/**
 * This functions initialize the Module:
 * 
 * + It is registered the TC0 callback;
 * + The Motor driver exits from the sleep mode;
 * + The Latch pins are cleared;
 * + The Motors are initialized with a disable mode;
 * 
 */
void motorLibInitialize(void){

    // Register the callback for the TC0 timer
    TC0_TimerCallbackRegister(latch_1us_callback,0);    

    // Latch reset High
    uC_LATCH_CLR_Set();
    
    // Exits from the Motor sleep mode
    MOTOR_SLEEP_OFF;
            
    // Reset Latches
    uC_LATCH_LEFT_Clear();
    uC_LATCH_RIGHT_Clear();
    uC_LATCH_FRONT_Clear();
    uC_LATCH_BACK_Clear();
    uC_LATCH_TRAP_Clear();
    uC_LATCH_MIRROR_Clear();
    uC_LATCH_FILTER_Clear();
    
    
    // INitializes the Motor BUS lines
    for(int i=0; i< MOTOR_LEN; i++){         
        motor_latch_request[i] = false;
        motor_latch[i].ILIM =   MOT_TORQUE_DISABLE;
        motor_latch[i].uSTEP =  MOT_uSTEP_1;
        motor_latch[i].DIR =    MOT_DIRCW;
        motor_latch[i].MOTENA = MOT_ENA_OFF;
        motor_latch[i].RST =    MOT_RST_ON;
        motor_latch[i].ENASTEP = MOT_ENASTEP_OFF;
        setLatch(i);
    }
    
    // Initializes the motor structures
    rightMotorStruct.id = MOTOR_RIGHT_ID;
    leftMotorStruct.id = MOTOR_LEFT_ID;
    backMotorStruct.id = MOTOR_BACK_ID;
    frontMotorStruct.id = MOTOR_FRONT_ID;
    trapMotorStruct.id = MOTOR_TRAP_ID;
    mirrorMotorStruct.id = MOTOR_MIRROR_ID;
    filterMotorStruct.id = MOTOR_FILTER_ID;
    
    
}


bool optoGet(MOTOR_STRUCT_t* mot){
    switch(mot->id){
        case MOTOR_LEFT_ID: mot->opto_status = uC_OPTO_LEFT_Get();break;
        case MOTOR_RIGHT_ID: mot->opto_status = uC_OPTO_RIGHT_Get();break;
        case MOTOR_FRONT_ID: mot->opto_status = uC_OPTO_FRONT_Get();break; 
        case MOTOR_BACK_ID: mot->opto_status = uC_OPTO_BACK_Get();break;
        case MOTOR_TRAP_ID: mot->opto_status = uC_OPTO_TRAP_Get();break;
        case MOTOR_MIRROR_ID: mot->opto_status = uC_OPTO_MIRROR_Get();break;
        case MOTOR_FILTER_ID: mot->opto_status = uC_OPTO_FILTER_Get();break;
    }
    
    return mot->opto_status;
}

void motorStep(MOTOR_STRUCT_t* mot, bool stat){
    mot->step_polarity = stat;
    
    if(stat){
        mot->steps++;
        switch(mot->id){
            case MOTOR_LEFT_ID: uC_STEP_LEFT_Set();return;
            case MOTOR_RIGHT_ID: uC_STEP_RIGHT_Set();return;
            case MOTOR_FRONT_ID: uC_STEP_FRONT_Set();return;
            case MOTOR_BACK_ID: uC_STEP_BACK_Set();return;
            case MOTOR_TRAP_ID: uC_STEP_TRAP_Set();return;
            case MOTOR_MIRROR_ID: uC_STEP_MIRROR_Set();return;
            case MOTOR_FILTER_ID: uC_STEP_FILTER_Set();return;
        }
        
    }else{
        switch(mot->id){
            case MOTOR_LEFT_ID: uC_STEP_LEFT_Clear();return;
            case MOTOR_RIGHT_ID: uC_STEP_RIGHT_Clear();return;
            case MOTOR_FRONT_ID: uC_STEP_FRONT_Clear();return;
            case MOTOR_BACK_ID: uC_STEP_BACK_Clear();return;
            case MOTOR_TRAP_ID: uC_STEP_TRAP_Clear();return;
            case MOTOR_MIRROR_ID: uC_STEP_MIRROR_Clear();return;
            case MOTOR_FILTER_ID: uC_STEP_FILTER_Clear();return;
        }
    }
    
    return ;
}

bool isLatched(MOTOR_STRUCT_t* mot){
    return !motor_latch_request[mot->id];
}
