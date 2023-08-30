
#ifndef _MOT_BACK_H    
#define _MOT_BACK_H

#include "definitions.h"

#undef ext
#undef ext_static

#ifdef _MOT_BACK_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup backMotorModule Back Motor Common Routines module
 *
 * \ingroup applicationModule
 * 
 * 
 * 
 * # Dependencies
 * 
 * 
 *  @{
 * 
 */

    /**
     * \defgroup moduleConstants Constants module definition
     * 
     * This section describes the module constants
     * 
     *  @{
     */
       

     /// @}   moduleConstants

     /**
     * \defgroup moduleStructures Module Data and Structure definition
     * 
     * This section describes the module structures
     * 
     *  @{
     */

       /// Module data structure
        typedef struct{

            // Filter positioning data
            bool    command_activated;  //!< A command is in execution
            bool    position_valid;      //!< The current position is valid
            uint32_t target_position;   //!< Define the calibrated position 

            // Slot detection        
            bool     opto_status; //!< This is a copy of the current Opto status
            uint32_t current_pulses;//!< This is the current pulse count during transitions

            // Speed regulation
            uint16_t init_period;   //!< This is the initial period for the PWM
            uint16_t final_period;  //!< This is the final period for the PWM
            uint16_t ramp_rate;     //!< This is the ramp value (delta period for every period) PWM

            uint8_t command_sequence;//!< This is the execution sequence index
            bool     running;        //!< This is the motor activated flag            

            uint32_t  measured_light_slot; //!< Measures the light slot pulses for diagnosys and test
            uint32_t  measured_dark_slot; //!< Measures the dark slot pulses for diagnosys and test

        }BACK_MOTOR_t;
        
     /// @}   moduleStructures  


     /**
     * \defgroup publicModuleApi Public API Module definition
     * 
     * This section describes the API of the module
     * 
     *  @{
     */
        ext void motorBackInit(void); //!< Module initialization function
        ext void activateBackCollimation(void);//!< Activates the collimation
        ext void motorBackTest(void);
        ext void motorBackInit(void);
        
    /// @}   publicModuleApi 


/** @}*/ // backMotorModule

#endif // _MOT_BACK_H