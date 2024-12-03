#define _MOT_FILTER_C

#include "application.h"
#include "motlib.h"
#include "filter.h"
#include "../Protocol/protocol.h"


#define TC2_BASE_CLOCK 3000000  // TC2 module clock source (verify in the MCC configuration))
#define TC2_FREQ 96000          // TC2 scaled period

#define STEPs_TO_PERIOD(speed,mode) (( ( TC2_FREQ / ( 2 * MICROSTEP(mode)) )  / (speed)) - 1) // Converts Step/second to period pulses
#define TIME_ms_TO_RAMP(ms) (ms * TC2_FREQ/ 2000) // Converts ms acceleration in ramp period

// Defines the motor performances
#define FILTER_STEPPING_MODE MOT_uSTEP_4
#define SPEED_FILTER_STEP_PER_SEC 800
#define INIT_SPEED_FILTER_STEP_PER_SEC 50
#define FILTER_RAMP_RATE 20

#define FILTER_DIRECTION_HOME MOT_DIRCW
#define FILTER_DIRECTION_FIELD MOT_DIRCCW

static bool command_activated = false;
static int  current_index = -1;
static int  target_index = -1;
static int  min_slot = 0;
static int  max_slot = 0;

static volatile int  blades = 0;
static void filterCallback(TC_COMPARE_STATUS status, uintptr_t context); //!< Timer Callback for the format collimation
static void filterPositioning(MOTOR_STRUCT_t* pMotor);


/**
 * Format Initialization function.
 */
void filterInit(void){
    
    // Command execution data initialization    
    command_activated = false;
    current_index = -1;
    target_index = -1;
    
    // Updates the Status register
    SystemStatusRegister.format_filter_activity = FORMAT_UNDEFINED;
    SystemStatusRegister.format_filter_index = 0;
    encodeStatusRegister(&SystemStatusRegister);
    
    // TC2 Setup
    TC2_CompareCallbackRegister(filterCallback, 0);// Registers the working callback to the TC2 timer
    TC2_CompareStop();    
    TC2_Compare16bitPeriodSet((long) TC2_BASE_CLOCK / (long) TC2_FREQ);
    
    
    // Sets the Motor performances
    filterMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_FILTER_STEP_PER_SEC, FILTER_STEPPING_MODE);
    filterMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_FILTER_STEP_PER_SEC, FILTER_STEPPING_MODE);
    filterMotorStruct.ramp_rate = (filterMotorStruct.init_period - filterMotorStruct.run_period)/FILTER_RAMP_RATE + 1;   
    
    filterMotorStruct.direction_home = FILTER_DIRECTION_HOME;
    filterMotorStruct.direction_field = FILTER_DIRECTION_FIELD;
    filterMotorStruct.stepping_mode = FILTER_STEPPING_MODE;
    filterMotorStruct.command_running = false;
    filterMotorStruct.command_running = false;
    filterMotorStruct.command_error = 0;
    filterMotorStruct.command_sequence = 0;
    
    // Disables the filter motor
    motorDisable(&filterMotorStruct);
    return;
}


_MOTOR_COMMAND_RETURN_t activateFilter(int index, bool force){
    
    // The current index is equal to the requested index
    if((!force)&&(current_index == index)) return MOT_RET_IN_TARGET;
    
    // A command is executing
    if(command_activated) return MOT_RET_ERR_BUSY;    
    if(index < 0) return MOT_RET_ERR_INVALID_TARGET;    
    if(index >= MAX_FILTER_INDEX) return MOT_RET_ERR_INVALID_TARGET;
    
    // Upload the target position to the motors
    if(!protocolGetFilter(index, &filterMotorStruct.target_steps)) return MOT_RET_ERR_INVALID_TARGET;
    
    // Init command flag
    command_activated = true;
    current_index = -1;
    target_index = index;
    
    // Assignes the slot steps
    switch(target_index){
        case 0:
            min_slot = SLOT0_MIN_STEP;
            max_slot = SLOT0_MAX_STEP;
            break;
        case 1:
            min_slot = SLOT1_MIN_STEP;
            max_slot = SLOT1_MAX_STEP;
            break;
        case 2:
            min_slot = SLOT2_MIN_STEP;
            max_slot = SLOT2_MAX_STEP;
            break;
        case 3:
            min_slot = SLOT3_MIN_STEP;
            max_slot = SLOT3_MAX_STEP;
            break;
        case 4:
            min_slot = SLOT4_MIN_STEP;
            max_slot = SLOT4_MAX_STEP;
            break;
            
    }
           
    // Updates the Status register
    SystemStatusRegister.format_filter_activity = FORMAT_EXECUTING;
    SystemStatusRegister.format_filter_index = target_index;
    encodeStatusRegister(&SystemStatusRegister);
     
    // Initializes the position procedures
    activationInitialize(&filterMotorStruct, true);
    return MOT_RET_STARTED;
}



/**
 * This procedure polls the activation of the five blades.
 * The pick current is then shared in the time reducing the maximum pick power.
 * 
 */
void filterCallback(TC_COMPARE_STATUS status, uintptr_t context){
   
    filterPositioning(&filterMotorStruct);   
   
    if(filterMotorStruct.command_running) return;
    TC2_CompareStop(); 
    
    if(filterMotorStruct.command_error ){
        
        // Command terminated in error condition
        current_index = -1;
        target_index = -1;
        
        // Updates the Status register
        SystemStatusRegister.format_filter_activity = FORMAT_UNDEFINED;
        SystemStatusRegister.format_filter_index = 0;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandError(ERROR_IN_FILTER_POSITIONING);
        
    }else{
        // Command terminated successfully
        current_index = target_index;
        
        // Updates the Status register
        SystemStatusRegister.format_filter_activity = FORMAT_SELECTED;
        SystemStatusRegister.format_filter_index = current_index;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandExecuted(current_index,0);
    }
    
    command_activated = false;
    return;
}   



void filterPositioning(MOTOR_STRUCT_t* pMotor){  
    
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
       pMotor->period-=pMotor->ramp_rate;
       if(pMotor->period <= pMotor->run_period) pMotor->period = pMotor->run_period;
    }else{ 
        pMotor->time_count++;
        return ;
    }
   
   switch(pMotor->command_sequence){
       case 1: 
           if(!isLatched(pMotor)) return ;
            
           // The procedure shall start from a point where the photocell is not triggered
           if(optoGet(pMotor)){  
                // Continue to step until the photocell is released
                motorStep(pMotor, true);
                return ;
           }
           
           // Now the photocell has been released
           pMotor->command_sequence++;
           return;
           
       case 2: // Find the first occurrence of a new blade            
            
            if(!optoGet(pMotor)){                
                motorStep(pMotor, true);            
                return ;
            }
            
            // Photocell trigger: counts the length of the blade
            pMotor->steps = 0;
            pMotor->command_sequence++;
            return ;
       
       case 3: // Waits to exit from the photocell
            if(optoGet(pMotor)){
                motorStep(pMotor, true);
                return ;
            }
            
            // The photocell has been released: check what is the current blade dimension            
            blades = pMotor->steps;
            pMotor->steps = 0;
            
            // Target reached
            if((blades >= min_slot) && (blades <= max_slot)){                
                pMotor->command_sequence = 5;
                return;
            }
            
            // Continue to find another blade: before executes extra steps to 
            // be sure that the photocell remains free
            pMotor->command_sequence++;
            return ;
       
       // Executes an extra steps sequence before to restart the new research
       case 4:
           if(pMotor->steps>10){
               // Restarts a new find sequence for the next blade
               pMotor->command_sequence = 2;
               return ;
           }
           
           // Extra Steps executing
           motorStep(pMotor, true);
           return;
           
       case 5: // Blade found!
            
            // Command successfully completed: goes to the keeping time
            if(pMotor->steps >= pMotor->target_steps){
                pMotor->command_sequence++;
                return ;
            }
            
            // Steps
            motorStep(pMotor, true);
            // Handles a timeout here
            return ;
       
       
           
       default:
           // Keeping time to dissipate the rotation inertia
           pMotor->command_sequence++;
           
           // Keeps the torque to dissipate the rotating energy inertia
           if(pMotor->command_sequence > 500){
                motorDisable(pMotor);
                pMotor->command_error = 0;
                pMotor->command_running = false;
                pMotor->command_sequence = 0;
           }
           return;
   }
   
   // Invalid condition
    motorDisable(pMotor);
    pMotor->command_error = 1;
    pMotor->command_running = false;
    pMotor->command_sequence = 0;
    return ;
   
}

void abortFilter(void){
    if(!filterMotorStruct.command_running) return;
    
    TC2_CompareStop();         
    motorDisable(&filterMotorStruct);
    filterMotorStruct.command_running = false;
    filterMotorStruct.command_error = 1;
    filterMotorStruct.command_sequence = 0;
   
    current_index = -1;
    target_index = -1;
    command_activated = false;
    
    // Updates the Status register
    SystemStatusRegister.format_filter_activity = FORMAT_UNDEFINED;
    SystemStatusRegister.format_filter_index = 0;
    encodeStatusRegister(&SystemStatusRegister);

    MET_Can_Protocol_returnCommandError(ERROR_IN_FILTER_POSITIONING);
    
}