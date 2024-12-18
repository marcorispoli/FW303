
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "application.h"
#include "Protocol/protocol.h"
#include "Motors/motlib.h"
#include "Motors/format_collimation.h"
#include "Motors/filter.h"
#include "Motors/mirror.h"

 /** 
     * \defgroup appMainModule  Main Module 
     * \ingroup applicationModule
     *     
     * This section implements the data structures of the module
     *  @{
     */

static uint8_t trigger_time = 0;

#define _1024_ms_TriggerTime 0x1
#define _7820_us_TriggerTime 0x2
#define _15_64_ms_TriggerTime 0x4

/**
 * This is the callback generated by the RTC module at the interrupt event.
 * 
 * @param intCause this is the cause of the interrupt event;
 * @param context this is the data pointer passed with the  RTC_Timer32CallbackRegister() routine;
 */
static void rtcEventHandler (RTC_TIMER32_INT_MASK intCause, uintptr_t context)
{
    // Periodic Interval Handler: Freq = 1024 / 2 ^ (n+3)
    
    if (intCause & RTC_TIMER32_INT_MASK_PER0) trigger_time |= _7820_us_TriggerTime;  // 7.82ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER1) trigger_time |= _15_64_ms_TriggerTime; // 15.64ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER7) trigger_time |= _1024_ms_TriggerTime;  // 1024 ms Interrupt
    
}

static void manageTestButton(void);

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    // Registers the RTC interrupt routine to the RTC module
    RTC_Timer32CallbackRegister(rtcEventHandler, 0);
    RTC_Timer32Start(); // Start the RTC module
    
    // Application Protocol initialization
    ApplicationProtocolInit();
    
    // Initializes the motors with the common routines
    motorLibInitialize();   
    formatInit();
    filterInit();
    mirrorInit();
    FAN_Clear();
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
                
        // Protocol management
        ApplicationProtocolLoop();
               
        if(trigger_time & _1024_ms_TriggerTime){
            trigger_time &=~ _1024_ms_TriggerTime;            
            light1sLoop();            
            VITALITY_LED_Toggle();
            
        }        
 
        // Timer events activated into the RTC interrupt
        if(trigger_time & _7820_us_TriggerTime){
            trigger_time &=~ _7820_us_TriggerTime;
            manageMotorLatch();
            manageTestButton();
        }

        if(trigger_time & _15_64_ms_TriggerTime){
            trigger_time &=~ _15_64_ms_TriggerTime;      
            
 
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

void manageTestButton(void){
    unsigned char test_case = 0;
    static bool activated = false;
    int steps;
    
    
    if(uC_SW2_Get()) test_case|=0x4;
    if(uC_SW3_Get()) test_case|=0x2;
    if(uC_SW1_Get()) test_case|=0x1;
    
    if(uC_TEST_PUSH_Get() == activated) return;
    
    activated = uC_TEST_PUSH_Get();
    
     if(activated){
            activateMotor(5000, &rightMotorStruct);     
            activateMotor(5000, &leftMotorStruct);
    }else abortActivation();
    return;
    
    if(test_case == 0){
        if(activated){
            activateMotor(0, &rightMotorStruct);
            activateMotor(0, &leftMotorStruct);
            activateMotor(0, &frontMotorStruct);
            activateMotor(0, &backMotorStruct);
            activateMotor(0, &mirrorMotorStruct);
        }else abortActivation();
        return;
    }
    
    if(test_case & 0x1){
        steps = 5000;
        if(activated){
            activateMotor(steps, &rightMotorStruct);
            activateMotor(steps, &leftMotorStruct);
            activateMotor(steps, &frontMotorStruct);
            activateMotor(steps, &backMotorStruct);
        }else abortActivation();       
    }
    
    if(test_case & 0x2){
        steps = 5000;
        if(activated){
            activateMotor(steps, &mirrorMotorStruct);
        }else abortActivation();       
    }
    
    if(test_case & 0x4){
        if(activated){
            activateFilter(0, true);
            //if(SystemStatusRegister.format_filter_activity == FORMAT_SELECTED){
            //    activateFilter((SystemStatusRegister.format_filter_index+1)%5);
            //}else activateFilter(0);
        }else abortFilter();      
        
    }

    
}
/** @}*/
/*******************************************************************************
 End of File
*/

