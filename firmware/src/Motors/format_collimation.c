#define _MOT_FORMAT_C

#include "application.h"
#include "motlib.h"
#include "format_collimation.h"
#include "../Protocol/protocol.h"


#define TC1_BASE_CLOCK 3000000  // TC1 module clock source (verify in the MCC configuration))
#define TC1_FREQ 96000          // TC1 scaled period

#define STEPs_TO_PERIOD(speed,mode) (( ( TC1_FREQ / ( 2 * MICROSTEP(mode)) )  / (speed)) - 1) // Converts Step/second to period pulses
#define TIME_ms_TO_RAMP(ms) (ms * TC1_FREQ/ 2000) // Converts ms acceleration in ramp period

// Defines the motor performances
#define LR_STEPPING_MODE MOT_uSTEP_16
#define SPEED_LR_STEP_PER_SEC 1000
#define INIT_SPEED_LR_STEP_PER_SEC 50
#define LR_RAMP_RATE 10

#define FRONT_STEPPING_MODE MOT_uSTEP_16
#define SPEED_FRONT_STEP_PER_SEC 1000
#define INIT_SPEED_FRONT_STEP_PER_SEC 50
#define FRONT_RAMP_RATE 10

#define BACK_STEPPING_MODE MOT_uSTEP_16
#define SPEED_BACK_STEP_PER_SEC 1000
#define INIT_SPEED_BACK_STEP_PER_SEC 50
#define BACK_RAMP_RATE 10

#define TRAP_STEPPING_MODE MOT_uSTEP_16
#define SPEED_TRAP_STEP_PER_SEC 300
#define INIT_SPEED_TRAP_STEP_PER_SEC 50
#define TRAP_RAMP_RATE 10

#define MAX_LR_STEPS_SUM 10000 // The maximum sum from the positions to prevent left and right contact
#define MAX_LR_STEPS 8000     // The maximum position for both left and right

static bool command_activated = false;
static int  current_index = -1;
static int  target_index = -1;


static void formatCallback(TC_COMPARE_STATUS status, uintptr_t context); //!< Timer Callback for the format collimation
static void motorPositioning(MOTOR_STRUCT_t* pMotor);


/**
 * Format Initialization function.
 */
void formatInit(void){
    
    // Command execution data initialization    
    command_activated = false;
    current_index = -1;
    target_index = -1;
    
    // Updates the Status register
    SystemStatusRegister.format_2d_activity = FORMAT_UNDEFINED;
    SystemStatusRegister.format_selected_index = 0;
    encodeStatusRegister(&SystemStatusRegister);
    

    // TC1 Setup
    TC1_CompareCallbackRegister(formatCallback, 0);// Registers the working callback to the TC1 timer
    TC1_CompareStop();    
    TC1_Compare16bitPeriodSet((long) TC1_BASE_CLOCK / (long) TC1_FREQ);
    
    
    // Sets the Motor performances
    leftMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_LR_STEP_PER_SEC, LR_STEPPING_MODE);
    leftMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_LR_STEP_PER_SEC, LR_STEPPING_MODE);
    leftMotorStruct.ramp_rate = (leftMotorStruct.init_period - leftMotorStruct.run_period)/LR_RAMP_RATE + 1;
    leftMotorStruct.direction_home = MOT_DIRCW;
    leftMotorStruct.direction_field = MOT_DIRCCW;
    leftMotorStruct.stepping_mode = LR_STEPPING_MODE;
    leftMotorStruct.command_running = false;
    leftMotorStruct.command_running = false;
    leftMotorStruct.command_error = 0;
    leftMotorStruct.command_sequence = 0;
       
    rightMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_LR_STEP_PER_SEC, LR_STEPPING_MODE);
    rightMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_LR_STEP_PER_SEC, LR_STEPPING_MODE);
    rightMotorStruct.ramp_rate = (rightMotorStruct.init_period - rightMotorStruct.run_period)/LR_RAMP_RATE + 1;
    rightMotorStruct.direction_home = MOT_DIRCW;
    rightMotorStruct.direction_field = MOT_DIRCCW;
    rightMotorStruct.stepping_mode = LR_STEPPING_MODE;
    rightMotorStruct.command_running = false;
    rightMotorStruct.command_error = 0;
    rightMotorStruct.command_sequence = 0;
    
    backMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_BACK_STEP_PER_SEC, BACK_STEPPING_MODE);
    backMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_BACK_STEP_PER_SEC, BACK_STEPPING_MODE);
    backMotorStruct.ramp_rate = (backMotorStruct.init_period - backMotorStruct.run_period)/BACK_RAMP_RATE + 1;
    backMotorStruct.direction_home = MOT_DIRCCW;
    backMotorStruct.direction_field = MOT_DIRCW;
    backMotorStruct.stepping_mode = BACK_STEPPING_MODE;
    backMotorStruct.command_running = false;
    backMotorStruct.command_error = 0;
    backMotorStruct.command_sequence = 0;
    
    frontMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_FRONT_STEP_PER_SEC, FRONT_STEPPING_MODE);
    frontMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_FRONT_STEP_PER_SEC, FRONT_STEPPING_MODE);
    frontMotorStruct.ramp_rate = (frontMotorStruct.init_period - frontMotorStruct.run_period)/FRONT_RAMP_RATE + 1;
    frontMotorStruct.direction_home = MOT_DIRCW;
    frontMotorStruct.direction_field = MOT_DIRCCW;
    frontMotorStruct.stepping_mode = FRONT_STEPPING_MODE;
    frontMotorStruct.command_running = false;
    frontMotorStruct.command_error = 0;
    frontMotorStruct.command_sequence = 0;
    
    trapMotorStruct.run_period = STEPs_TO_PERIOD(SPEED_TRAP_STEP_PER_SEC, TRAP_STEPPING_MODE);
    trapMotorStruct.init_period = STEPs_TO_PERIOD(INIT_SPEED_TRAP_STEP_PER_SEC, TRAP_STEPPING_MODE);
    trapMotorStruct.ramp_rate = (trapMotorStruct.init_period - trapMotorStruct.run_period)/TRAP_RAMP_RATE + 1;
    trapMotorStruct.direction_home = MOT_DIRCCW;
    trapMotorStruct.direction_field = MOT_DIRCW;
    trapMotorStruct.stepping_mode = TRAP_STEPPING_MODE;
    trapMotorStruct.command_running = false;
    trapMotorStruct.command_error = 0;
    trapMotorStruct.command_sequence = 0;
    
    // Disables all the motors
    motorDisable(&leftMotorStruct);
    motorDisable(&rightMotorStruct);
    motorDisable(&backMotorStruct);
    motorDisable(&frontMotorStruct);
    motorDisable(&trapMotorStruct);

    return;
}


_MOTOR_COMMAND_RETURN_t activateFormatCollimation(int index){
    
    // The current index is equal to the requested index
    if(current_index == index) return MOT_RET_IN_TARGET;
    
    // A command is executing
    if(command_activated) return MOT_RET_ERR_BUSY;    
    if(index < 0) return MOT_RET_ERR_INVALID_TARGET;    
    if(index >= MAX_FORMAT_INDEX) return MOT_RET_ERR_INVALID_TARGET;
    
    // Upload the target position to the motors
    if(!protocolGet2DFormat(index, &leftMotorStruct.target_steps, &rightMotorStruct.target_steps, &frontMotorStruct.target_steps, &backMotorStruct.target_steps, &trapMotorStruct.target_steps)) return MOT_RET_ERR_INVALID_TARGET;
    
    // Verify the target validity
    if(leftMotorStruct.target_steps > MAX_LR_STEPS) leftMotorStruct.target_steps = MAX_LR_STEPS;
    if(rightMotorStruct.target_steps > MAX_LR_STEPS) rightMotorStruct.target_steps = MAX_LR_STEPS;
    if((leftMotorStruct.target_steps + rightMotorStruct.target_steps) > MAX_LR_STEPS_SUM){
        leftMotorStruct.target_steps -= (leftMotorStruct.target_steps + rightMotorStruct.target_steps - MAX_LR_STEPS_SUM)/2;
        rightMotorStruct.target_steps -= (leftMotorStruct.target_steps + rightMotorStruct.target_steps - MAX_LR_STEPS_SUM)/2;  
    }
    
    // Init command flag
    command_activated = true;
    current_index = -1;
    target_index = index;
    
    // Updates the Status register
    SystemStatusRegister.format_2d_activity = FORMAT_EXECUTING;
    SystemStatusRegister.format_selected_index = target_index;
    encodeStatusRegister(&SystemStatusRegister);
        
    // Initializes the position procedures
    activationInitialize(&leftMotorStruct, false);
    activationInitialize(&rightMotorStruct, false);
    activationInitialize(&backMotorStruct, false);
    activationInitialize(&leftMotorStruct, false);
    activationInitialize(&frontMotorStruct, true);
    //activationInitialize(&trapMotorStruct, true);
    
    return MOT_RET_STARTED;
}



/**
 * This procedure polls the activation of the five blades.
 * The pick current is then shared in the time reducing the maximum pick power.
 * 
 */
void formatCallback(TC_COMPARE_STATUS status, uintptr_t context){
   
    
    motorPositioning(&leftMotorStruct);   
    motorPositioning(&rightMotorStruct);
    motorPositioning(&backMotorStruct);
    motorPositioning(&frontMotorStruct);
    motorPositioning(&trapMotorStruct);
    
    if(leftMotorStruct.command_running) return;
    if(rightMotorStruct.command_running) return;
    if(backMotorStruct.command_running) return;
    if(frontMotorStruct.command_running) return;
    if(trapMotorStruct.command_running) return;
       
    
    TC1_CompareStop(); 
    if( (leftMotorStruct.command_error + 
            rightMotorStruct.command_error + 
            backMotorStruct.command_error + 
            frontMotorStruct.command_error + 
            trapMotorStruct.command_error) > 0){
        
        // Command terminated in error condition
        current_index = -1;
        target_index = -1;
        
        // Updates the Status register
        SystemStatusRegister.format_2d_activity = FORMAT_UNDEFINED;
        SystemStatusRegister.format_selected_index = 0;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandError(ERROR_IN_FORMAT_POSITIONING);
        
    }else{
        // Command terminated successfully
        current_index = target_index;
        
        // Updates the Status register
        SystemStatusRegister.format_2d_activity = FORMAT_SELECTED;
        SystemStatusRegister.format_selected_index = current_index;
        encodeStatusRegister(&SystemStatusRegister);

        MET_Can_Protocol_returnCommandExecuted(current_index,0);
    }
    
    command_activated = false;
    return;
}   



void motorPositioning(MOTOR_STRUCT_t* pMotor){   
   
    
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
                pMotor->command_sequence++;
                return;
            }
            
            // Steps
            motorStep(pMotor, true);
            
            
           // Handles a timeout here

            return ;
           
           default:
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
