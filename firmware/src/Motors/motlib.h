
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

/*!
 * \defgroup motorModule Motor Common Routines module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the common routines used to 
 * drive the Stepper motors in the application.
 * 
 * # Dependencies
 * 
 * ## TC0 Peripheral timer setting
 * 
 * This module requires to set the TC0 peripheral timer to operate 
 * in one shot mode with 1us of overflow interrupt. 
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
        /// This enumeration type defines the Motor Current Limit levels
        typedef enum{
            MOT_TORQUE_HIGH = 0,  //!< Motor High torque mode:  Vref =  1.52V
            MOT_TORQUE_LOW,       //!< Motor Low torque mode: Vref = 0.28V
            MOT_TORQUE_MED,       //!< Motor Medium torque mode: Vref =  1.39V       
            MOT_TORQUE_DISABLE    //!< Motor No torque mode: Vref = 0V 
        }MOT_ILIM_MODE_t;

        /// This enumeration type defines the uStep mode selection
        typedef enum{
            MOT_uSTEP_1 = 0, //!< Motor with Full Step mode: 1 pulse corresponds to a full step;
            MOT_uSTEP_2,     //!< Motor with Half Step mode: 1 pulse corresponds to a 1/2 step;
            MOT_uSTEP_4,     //!< Motor with 1/4  Step mode: 1 pulse correspond to a 1/4 step;
            MOT_uSTEP_16     //!< Motor with 1/16 Step mode: 1 pulse correspond to a 1/16 step;
        }MOT_MICROSTEP_t;

        /** This enumeration type defines the activation direction
         *  
         *  NOTE: the clockwise or counterclockwise is not necessarily 
         *  related to the Motor axes. It is only an internal assumption 
         *  that can have different impact in the real motor rotation.
         */ 
        typedef enum{
            MOT_DIRCW = 0,  //!< Motor clockwise
            MOT_DIRCCW      //!< Motor counterclockwise
        }MOT_DIRECTION_t;

        /// This enumeration type defines the Motor Enable/Disable flag
        typedef enum{
            MOT_ENA_ON = 0, //!< Motor Enabled label
            MOT_ENA_OFF     //!< Motor Disabled label
        }MOT_ENA_t;

        /// This enumeration type defines the Motor reset flag
        typedef enum{
            MOT_RST_ON = 0, //!< Motor Reset active
            MOT_RST_OFF     //!< Motor Reset not active
        }MOT_RST_t;

        /// This enumeration type defines the Enable Step flag
        typedef enum{
            MOT_ENASTEP_OFF = 0, //!< Motor Step Disabled
            MOT_ENASTEP_ON  //!< Motor Step Enabled
        }MOT_ENASTEP_t;

        #define MOTOR_LEN 5 //!< Defines the maximum number of implemented motors
        
        /// This enumeration type defines the identifier of the stepper motors
        typedef enum{
            MOTOR_M1 = 0,   //!< Motor M1 identifier
            MOTOR_M2,       //!< Motor M2 identifier
            MOTOR_M3,       //!< Motor M3 identifier
            MOTOR_M4,       //!< Motor M4 identifier
            MOTOR_M5,       //!< Motor M5 identifier 
        }_MOTOR_ID_t;

     /// @}   moduleConstants

     /**
     * \defgroup moduleStructures Module Data and Structure definition
     * 
     * This section describes the module structures
     * 
     *  @{
     */

        /// This structure defines the Motor data BUS that will be latched
        typedef struct{
            unsigned char ILIM:2;   //!< IA and IB outputs setting the current limit
            unsigned char uSTEP:2;  //!< MS1 and MS2 outputs setting the uStepping mode
            unsigned char DIR:1;    //!< Motor direction output
            unsigned char MOTENA:1; //!< Motor enable output
            unsigned char RST:1;    //!< Motor reset output
            unsigned char ENASTEP:1;//!< Motor Enable Step output
        }_MOTOR_DATA_t;
     
        ext _MOTOR_DATA_t motor_latch[MOTOR_LEN]; //!< Array of the Motor Bus lines
        ext bool motor_latch_request[MOTOR_LEN]; //!< Array of the Motor Bus line request
        
     /// @}   moduleStructures  


     /**
     * \defgroup publicModuleApi Public API Module definition
     * 
     * This section describes the API of the module
     * 
     *  @{
     */
        
        ext void motorsInitialize(void); //!< Module initialization function
        ext void motorDisable(_MOTOR_ID_t motid);//!< Disables the motor
        ext void motorOn(_MOTOR_ID_t motid, MOT_ILIM_MODE_t torque, MOT_MICROSTEP_t ustep, MOT_DIRECTION_t dir );//!< Activate the motor in a defined mode
        ext void motorHold(_MOTOR_ID_t motid, MOT_ILIM_MODE_t torque); //!< Switch Off the motor with an active torque
        ext void manageMotorLatch(void);
        ext bool isLatched(_MOTOR_ID_t motid);
        ext void stopTc(_MOTOR_ID_t motid);
        ext void motorRegisterTcCallback(_MOTOR_ID_t motid, void (*fun_ptr)(TC_COMPARE_STATUS status, uintptr_t context) );
        ext void setTcPeriod(_MOTOR_ID_t motid, uint16_t period);
        ext void setRampPeriod(_MOTOR_ID_t motid, uint16_t final, uint16_t ramp);
        
    /// @}   publicModuleApi 



/** @}*/ // motorModule

#endif // _MOTLIB_H