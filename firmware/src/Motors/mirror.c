#define _MOT_MIRROR_C

#include "application.h"
#include "motlib.h"
#include "mirror.h"
#include "../Protocol/protocol.h"


#define TC3_BASE_CLOCK 3000000  // TC3 module clock source (verify in the MCC configuration))
#define TC3_FREQ 96000          // TC3 scaled period

#define STEPs_TO_PERIOD(speed,mode) (( (TC3_FREQ / (MICROSTEP(mode)) )  / (speed)) - 1) // Converts Step/second to period pulses
#define TIME_ms_TO_RAMP(ms) (ms * TC3_FREQ/ 2000) // Converts ms acceleration in ramp period

// Defines the motor performances
#define MIRROR_STEPPING_MODE MOT_uSTEP_16
#define SPEED_MIRROR_STEP_PER_SEC 300
#define RAMP_MIRROR_ms_TIME 500
#define MIRROR_DIRECTION_HOME MOT_DIRCW
#define MIRROR_DIRECTION_FIELD MOT_DIRCCW

static bool command_activated = false;
static int  current_index = -1;
static int  target_index = -1;

static void mirrorCallback(TC_COMPARE_STATUS status, uintptr_t context); //!< Timer Callback for the format collimation
static void mirrorPositioning(MOTOR_STRUCT_t* pMotor, bool init);

static LIGHT_STRUCT_t lightStruct;

/**
 * Mirror Initialization function.
 */
void mirrorInit(void){
    
    // Command execution data initialization    
    command_activated = false;
    current_index = -1;
    target_index = -1;
    
    // Updates the Status register
    SystemStatusRegister.format_mirror_activity = FORMAT_UNDEFINED;
    SystemStatusRegister.in_field_position  = 1;
    
    
    // TC3 Setup
    TC3_CompareCallbackRegister(mirrorCallback, 0);// Registers the working callback to the TC3 timer
    TC3_CompareStop();    
    TC3_Compare16bitPeriodSet((long) TC3_BASE_CLOCK / (long) TC3_FREQ);
    
    
    // Sets the Motor performances
    mirrorMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_MIRROR_STEP_PER_SEC, MIRROR_STEPPING_MODE);
    mirrorMotorStruct.init_period = mirrorMotorStruct.run_period + TIME_ms_TO_RAMP(RAMP_MIRROR_ms_TIME);
    mirrorMotorStruct.direction_home = MIRROR_DIRECTION_HOME;
    mirrorMotorStruct.direction_field = MIRROR_DIRECTION_FIELD;
    mirrorMotorStruct.stepping_mode = MIRROR_STEPPING_MODE;
    mirrorMotorStruct.command_running = false;
    mirrorMotorStruct.command_running = false;
    mirrorMotorStruct.command_error = 0;
    mirrorMotorStruct.command_sequence = 0;
    
    // Disables the filter motor
    motorDisable(&mirrorMotorStruct);
    
    // Light initialization 
    lightStruct.status = false;
    lightStruct.timer = 0;
    SystemStatusRegister.collimation_light = 0;
    uC_LED_ON_Set();
    
    encodeStatusRegister(&SystemStatusRegister);
    return;
}


_MOTOR_COMMAND_RETURN_t activateMirror(int index){
    
    // The current index is equal to the requested index
    if(current_index == index) return MOT_RET_IN_TARGET;
    
    // A command is executing
    if(command_activated) return MOT_RET_ERR_BUSY;    
    if(index < 0) return MOT_RET_ERR_INVALID_TARGET;    
    if(index > 1) return MOT_RET_ERR_INVALID_TARGET;
    
    // Upload the target position to the motors
    if(index == 0) mirrorMotorStruct.target_steps = 0;
    else mirrorMotorStruct.target_steps = protocolGetMirror();
    
    // Init command flag
    command_activated = true;
    current_index = -1;
    target_index = index;
    
    // Updates the Status register
    SystemStatusRegister.format_mirror_activity = FORMAT_EXECUTING;
    SystemStatusRegister.in_field_position = target_index;
    encodeStatusRegister(&SystemStatusRegister);
     
    // Initializes the position procedures
    mirrorPositioning(&mirrorMotorStruct, true);
       
    // Start the timer
    TC3_CompareStart();
    return MOT_RET_STARTED;
}



/**
 * 
 * 
 */
void mirrorCallback(TC_COMPARE_STATUS status, uintptr_t context){
   
    mirrorPositioning(&mirrorMotorStruct, false);   
   
    if(mirrorMotorStruct.command_running) return;
    TC3_CompareStop(); 
    
    if(mirrorMotorStruct.command_error ){
        
        // Command terminated in error condition
        current_index = -1;
        target_index = -1;
        
        // Updates the Status register
        SystemStatusRegister.format_mirror_activity = FORMAT_UNDEFINED;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandError(ERROR_IN_MIRROR_POSITIONING);
        
    }else{
        // Command terminated successfully
        current_index = target_index;
        
        // Updates the Status register
        SystemStatusRegister.format_mirror_activity = FORMAT_SELECTED;
        SystemStatusRegister.in_field_position = current_index;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandExecuted(current_index,0);
    }
    
    command_activated = false;
    return;
}   



void mirrorPositioning(MOTOR_STRUCT_t* pMotor, bool init){   
   
     // Initialize the sequence
    if(init){

        // Initializes the ramp
        pMotor->time_count = 0; 
        pMotor->period = pMotor->init_period;

        if(optoGet(pMotor)) pMotor->command_sequence = 2; // Already in zero position
        else{
            pMotor->command_sequence = 1;
            motorOn(pMotor, MOT_TORQUE_HIGH, pMotor->direction_home );          
        }      

        pMotor->command_running = true;
        pMotor->command_error = 0;
        return ;
    }
    
    // Returns if not running (already terminated the positioning or not used)
    if(!pMotor->command_running) return;
   
   // The falling edge of the step is not counted
   if(pMotor->step_polarity) {
       motorStep(pMotor, false);
       return ;
   }

    // Ramp/Speed handling: if the scheduled time is not expired no action will be taken
    if(pMotor->time_count >= pMotor->period){
       pMotor->time_count = 0; 
       pMotor->period--;
       if(pMotor->period <= pMotor->run_period) pMotor->period = pMotor->run_period;
    }else{ 
        pMotor->time_count++;
        return ;
    }

   switch(pMotor->command_sequence){
       
       case 1:// Waits the Motor has been actually latched            
            if(!isLatched(pMotor)) return ;

            // Home position reached
            if(optoGet(pMotor)){                
                pMotor->command_sequence++;
                return ;
            }
           
            // Steps
            motorStep(pMotor, true);
           
            // Handling a timeout here  
            return ;
           
       case 2: // Prepares for the In-field activation
           
           // Initializes the ramp
            pMotor->time_count = 0; 
            pMotor->period = pMotor->init_period;
            
            motorOn(pMotor, MOT_TORQUE_HIGH, pMotor->direction_field );                        
            pMotor->steps = 0; // Reset the steps to be counted for the target
            
            pMotor->command_sequence++;
            return ;
       
       case 3: // Waits to exit from the photocell
            if(!isLatched(pMotor)) return ;
            
            if(!optoGet(pMotor)){
                pMotor->command_sequence++;
                pMotor->steps = 0;
                return ;
            }
            
            // Steps
            motorStep(pMotor, true);
            
            // Handle a timeout here
            
            return ;
            
       case 4: // Steps to the requested target
            
            // Command successfully completed
            if(pMotor->steps >= pMotor->target_steps){
                motorDisable(pMotor);
                pMotor->command_error = 0;
                pMotor->command_running = false;
                pMotor->command_sequence = 0;
                return ;
            }
            
            // Steps
            motorStep(pMotor, true);
            
            
           // Handles a timeout here

            return ;
                       
   }
   
   // Invalid condition
    motorDisable(pMotor);
    pMotor->command_error = 1;
    pMotor->command_running = false;
    pMotor->command_sequence = 0;
    return ;

   
}


void light1sLoop(void){
    if(!lightStruct.status) return;    

    if(lightStruct.timer){ 
        lightStruct.timer--;
        if(!lightStruct.timer){
            lightStruct.status = false;
            uC_LED_ON_Set();
            SystemStatusRegister.collimation_light = 0;
            encodeStatusRegister(&SystemStatusRegister);
            return;   
        }
    }
    return;

}

void lightActivation(bool status){
    
    if(status){
        lightStruct.timer = 20;
        uC_LED_ON_Clear();
        lightStruct.status = true;
        SystemStatusRegister.collimation_light = 1;
        encodeStatusRegister(&SystemStatusRegister);
        return;        
    }
    
    lightStruct.status = false;
    uC_LED_ON_Set();
    lightStruct.timer = 0;
    SystemStatusRegister.collimation_light = 0;
    encodeStatusRegister(&SystemStatusRegister);
    return;        
}