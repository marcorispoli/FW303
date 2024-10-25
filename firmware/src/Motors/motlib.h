
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
 * \defgroup MOTORS Motor Management Module
 * \ingroup libraryModules
 * 
 * This Module implements the basic routines to get access to the 
 * motor drivers.
 * 
 * ## OVERVIEW
 * 
 * The Collimator device handles up to seven stepper motors 
 * in order to control:
 * + The collimation format:
 *  + The Right blade motor driver;
 *  + The Left blade motor driver;
 *  + The Front blade motor driver;
 *  + The Back blade motor driver;
 *  + The Trap blade motor driver;
 * + The mirror positioning;
 * + The filter selection;
 * 
 * \Warning Not all the motors can be activated at the same time:
 * actually the board is dimensioned to handle 5 motor activated at the same time.
 * 
 * Each of the motor driver share the same schematics
 * and most of the digital lines that control the motor drivers 
 * are shared in a parallel bus.
 * 
 * This library module provides the data structure and the 
 * helper function that allow to control every single driver.
 * 
 * In particolar:
 * + the module handles the Mux lines and the necessary timing 
 * to get access to the driver BUS;
 * + provides the data structure to handle the motor implementation;
 * + provides an interface to set the motor driver:
 *   + set the torque limitation;
 *   + enable disable the driver;
 *   + set the microstepping mode;
 *   + set the rotation direction;
 * 
 * 
 * ## Dependencies
 * 
 * 
 * 
 * 
 * ## Driver Control Digital Bus
 * 
 * The board makes use of octal digital buffers in order to share 
 * the common microcontroller lines. 
 * 
 * ### Current Limitation
 * 
 * The board makes use of two digital lines to set the  
 * voltage reference that controls the limitation current level
 * on the motor driver.
 * 
 * |uC_IA|uC_IB|Torque|Driver Voltage reference|
 * |:--|:--|:--|:--|
 * |0|0|HIGH TORQUE|1.52V|
 * |0|1|LOW TORQUE|0.28V|
 * |1|0|MED TORQUE|1.39V|
 * |1|1|NO TORQUE|0V|
 * 
 * 
 *  
 * 
 * The module control the driver current limitation through two digital
 * lines of the control BUS 
 * 
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
        static const unsigned char ustep[4] = {1,2,4,16};
        
        #define MICROSTEP(x) (((unsigned char) x) < 4) ? ustep[(unsigned char) x] : 1

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

        #define MOTOR_LEN 7 //!< Defines the maximum number of implemented motors
        
        /// This enumeration type defines the identifier of the stepper motors
        typedef enum{
            MOTOR_LEFT_ID = 0,   //!< Left Motor identifier
            MOTOR_RIGHT_ID,       //!< Right Motor identifier
            MOTOR_FRONT_ID,       //!< Front Motor identifier
            MOTOR_BACK_ID,       //!< Back Motor identifier
            MOTOR_TRAP_ID,       //!< Trap Motor identifier 
            MOTOR_FILTER_ID,       //!< Filter Motor identifier 
            MOTOR_MIRROR_ID,        //!< Mirror Motor identifier 
            LAST_MOTOR_ID = MOTOR_MIRROR_ID    
        }_MOTOR_ID_t;


        /// This structure defines the Motor data BUS that will be latched
        typedef struct{
            unsigned char ILIM:2;   //!< IA and IB outputs setting the current limit
            unsigned char uSTEP:2;  //!< MS1 and MS2 outputs setting the uStepping mode
            unsigned char DIR:1;    //!< Motor direction output
            unsigned char MOTENA:1; //!< Motor enable output
            unsigned char RST:1;    //!< Motor reset output
            unsigned char ENASTEP:1;//!< Motor Enable Step output
        }_MOTOR_DATA_t;
     
        /// This is the enumeration of motor activation return codes
        typedef enum{
            MOT_RET_IN_TARGET =0, //!< Thecommand is already in the requested target position
            MOT_RET_STARTED = 1,  //!< The command is started;
            MOT_RET_ERR_BUSY = 2,//!< A command is already running;
            MOT_RET_ERR_INVALID_TARGET = 3,//!< An invalid target has been requested                     
        }_MOTOR_COMMAND_RETURN_t;
     
        
        /// Module data structure
        typedef struct{
            
            int id;     //!< Motor Id 
            unsigned short steps;  //!< Number of steps executed in the current positioning
            unsigned short target_steps;   //!< This is the target position
            bool step_polarity;//!< This is the status of the STEP output pin

            // Ramp implementation
            int period; //!< The current pwm period
            int time_count;    //!< Timer implemening the pwm
            int init_period; //!< This is the innitial period for the ramp            
            int run_period;    //!< Minimum period (speed)
            
            MOT_DIRECTION_t direction_home; //!< This is the setup direction for home
            MOT_DIRECTION_t direction_field;//!< This is the setup direction for field
            MOT_MICROSTEP_t stepping_mode;//!< This is the setup stepping mode
            
            int     command_error;   //!< error code if > 0
            bool    command_running; //!< true if the positioning is not completed
            uint8_t command_sequence;//!< This is the execution sequence index
            
            bool opto_status; //!< This is the last read  status of the zero photocell 
            
        }MOTOR_STRUCT_t;
        
        ext _MOTOR_DATA_t motor_latch[MOTOR_LEN]; //!< Array of the Motor Bus lines
        ext bool motor_latch_request[MOTOR_LEN]; //!< Array of the Motor Bus line request
        
        ext MOTOR_STRUCT_t leftMotorStruct;
        ext MOTOR_STRUCT_t rightMotorStruct;
        ext MOTOR_STRUCT_t backMotorStruct;
        ext MOTOR_STRUCT_t frontMotorStruct;
        ext MOTOR_STRUCT_t trapMotorStruct;
        ext MOTOR_STRUCT_t mirrorMotorStruct;
        ext MOTOR_STRUCT_t filterMotorStruct;

        ext void motorLibInitialize(void); //!< Module initialization function        
        ext void manageMotorLatch(void);
        
        ext bool isLatched(MOTOR_STRUCT_t* mot);
        ext void motorDisable(MOTOR_STRUCT_t* mot);//!< Disables the motor
        ext void motorOn(MOTOR_STRUCT_t* mot, MOT_ILIM_MODE_t torque, MOT_DIRECTION_t dir);//!< Activate the motor in a defined mode
        ext void motorHold(MOTOR_STRUCT_t* mot, MOT_ILIM_MODE_t torque); //!< Switch Off the motor with an active torque
        ext void motorStep(MOTOR_STRUCT_t* mot, bool stat);        
        ext bool optoGet(MOTOR_STRUCT_t* mot);
        
        
#endif // _MOTLIB_H