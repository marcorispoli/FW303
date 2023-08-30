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
        #define MOTOR_SLEEP_OFF MOT_SLEEP_Set() //!<  Exit the Motor Sleep Mode macro
        #define MOTOR_SLEEP_ON MOT_SLEEP_Clear() //!<  Set the Motor  Sleep Mode macro

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
    for(int i=0; i <= MOTOR_M5; i++){
        if(!motor_latch_request[i]) continue;
        setLatch(i);
        motor_latch_request[i] = false;
    }
}

void requestLatch(_MOTOR_ID_t motid){
    motor_latch_request[motid] = true;
}

bool isLatched(_MOTOR_ID_t motid){
    return !motor_latch_request[motid];
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
            wait_us(LATCH_US_PULSE);
            uc_LATCH_M1_Clear();
        break;
        case MOTOR_M2: 
            uc_LATCH_M2_Set();
            wait_us(LATCH_US_PULSE);
            uc_LATCH_M2_Clear();
        break;
        case MOTOR_M3: 
            uc_LATCH_M3_Set();
            wait_us(LATCH_US_PULSE);
            uc_LATCH_M3_Clear();
        break;
        case MOTOR_M4: 
            uc_LATCH_M4_Set();
            wait_us(LATCH_US_PULSE);
            uc_LATCH_M4_Clear();
        break;
        case MOTOR_M5: 
            uc_LATCH_M5_Set();
            wait_us(LATCH_US_PULSE);
            uc_LATCH_M5_Clear();
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
void motorOn(_MOTOR_ID_t motid, MOT_ILIM_MODE_t torque, MOT_MICROSTEP_t ustep, MOT_DIRECTION_t dir ){
    
    // Checks the motor id value
    if(motid > MOTOR_M5) return;
    
    // Sets the motor bus lines
    motor_latch[motid].ILIM =   torque;
    motor_latch[motid].uSTEP =  ustep;
    motor_latch[motid].DIR = dir;
     
    motor_latch[motid].MOTENA = MOT_ENA_ON;
    motor_latch[motid].RST =    MOT_RST_OFF;
    motor_latch[motid].ENASTEP = MOT_ENASTEP_ON;
            
    requestLatch(motid);
 
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
void motorHold(_MOTOR_ID_t motid, MOT_ILIM_MODE_t torque){
    
    // Checks the motor id value
    if(motid > MOTOR_M5) return;    
    
    // Limits the torque applicable to a LOW or DISABLE value
    if(torque > MOT_TORQUE_LOW) torque = MOT_TORQUE_LOW;
    
    // Set the motor bus lines
    motor_latch[motid].ILIM =   torque;     
    motor_latch[motid].MOTENA = MOT_ENA_ON;
    motor_latch[motid].RST =    MOT_RST_OFF;
    motor_latch[motid].ENASTEP = MOT_ENASTEP_OFF;
            
    // Latches 
    requestLatch(motid);
    
}

/**
 * This function sets the motor in Disable mode.
 * 
 * When the Motor is set in disable mode, the motor driver is disabled
 * and the step sequencer is reset. No torque is applied!
 *  
 * @param motid Selects the target motor from MOTOR_M1 to MOTOR_M5; 
 */
void motorDisable(_MOTOR_ID_t motid){
    
    // Checks the motor id value    
    if(motid > MOTOR_M5) return;    
    
     // Set the motor bus lines
    motor_latch[motid].MOTENA = MOT_ENA_OFF;
    motor_latch[motid].RST =    MOT_RST_ON;
    motor_latch[motid].ENASTEP = MOT_ENASTEP_OFF;
            
    // Latches 
    requestLatch(motid);
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
void motorsInitialize(void){

    // Register the callback for the TC0 timer
    TC0_TimerCallbackRegister(latch_1us_callback,0);    

    // Latch reset High
    uc_LATCH_CLR_Set();
    
    // Exits from the Motor sleep mode
    MOTOR_SLEEP_OFF;
            
    // Reset Latches
    uc_LATCH_M1_Clear();
    uc_LATCH_M2_Clear();
    uc_LATCH_M3_Clear();
    uc_LATCH_M4_Clear();
    uc_LATCH_M5_Clear();
    
    
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
    
    
}

void setTcPeriod(_MOTOR_ID_t motid, uint16_t period){
    switch(motid){
        case MOTOR_M1: 
            
            TC1_CompareStop();    
            TC1_Compare16bitPeriodSet(period);
            TC1_CompareStart();
            
        break;
        case MOTOR_M2: 
            TC2_CompareStop();    
            TC2_Compare16bitPeriodSet(period);
            TC2_CompareStart();
            
        break;
        case MOTOR_M3: 
            TC3_CompareStop();    
            TC3_Compare16bitPeriodSet(period);
            TC3_CompareStart();
            
        break;
        case MOTOR_M4: 
            TC4_CompareStop();    
            TC4_Compare16bitPeriodSet(period);
            TC4_CompareStart();
            
        break;
        case MOTOR_M5: 
            TC5_CompareStop();    
            TC5_Compare16bitPeriodSet(period);
            TC5_CompareStart();
            
        break;
    }
}

void motorRegisterTcCallback(_MOTOR_ID_t motid, void (*fun_ptr)(TC_COMPARE_STATUS status, uintptr_t context) ){
    switch(motid){
        case MOTOR_M1: 
            TC1_CompareCallbackRegister(fun_ptr, 0);
        break;
        case MOTOR_M2: 
            TC2_CompareCallbackRegister(fun_ptr, 0);
        break;
        case MOTOR_M3: 
           TC3_CompareCallbackRegister(fun_ptr, 0);
        break;
        case MOTOR_M4: 
            TC4_CompareCallbackRegister(fun_ptr, 0);

        break;
        case MOTOR_M5: 
            TC5_CompareCallbackRegister(fun_ptr, 0);
        break;
    }
}

void stopTc(_MOTOR_ID_t motid){
    switch(motid){
        case MOTOR_M1: 
            TC1_CompareStop();
        break;
        case MOTOR_M2: 
            TC2_CompareStop();
        break;
        case MOTOR_M3: 
           TC3_CompareStop();
        break;
        case MOTOR_M4: 
            TC4_CompareStop();

        break;
        case MOTOR_M5: 
            TC5_CompareStop();
        break;
    }
}

void setRampPeriod(_MOTOR_ID_t motid, uint16_t final, uint16_t ramp){
    uint16_t current_period;
    
    switch(motid){
        case MOTOR_M1: 
            current_period = TC1_Compare16bitPeriodGet();
            if(current_period > final){
                TC1_CompareStop();    
                TC1_Compare16bitPeriodSet(current_period - ramp);
                TC1_CompareStart();
            }
        break;
        case MOTOR_M2: 
            current_period = TC2_Compare16bitPeriodGet();
            if(current_period > final){
                TC2_CompareStop();    
                TC2_Compare16bitPeriodSet(current_period - ramp);
                TC2_CompareStart();
            }
            
        break;
        case MOTOR_M3: 
            current_period = TC3_Compare16bitPeriodGet();
            if(current_period > final){
                TC3_CompareStop();    
                TC3_Compare16bitPeriodSet(current_period - ramp);
                TC3_CompareStart();
            }

            
        break;
        case MOTOR_M4: 
            current_period = TC4_Compare16bitPeriodGet();
            if(current_period > final){
                TC4_CompareStop();    
                TC4_Compare16bitPeriodSet(current_period - ramp);
                TC4_CompareStart();
            }

        break;
        case MOTOR_M5: 
            current_period = TC5_Compare16bitPeriodGet();
            if(current_period > final){
                TC5_CompareStop();    
                TC5_Compare16bitPeriodSet(current_period - ramp);
                TC5_CompareStart();
            }

        break;
    }
}