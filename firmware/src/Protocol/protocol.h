#ifndef _PROTOCOL_H    
#define _PROTOCOL_H

#include "definitions.h"  
#include "application.h"  
#include "Shared/CAN/MET_can_protocol.h"

#undef ext
#undef ext_static

#ifdef _PROTOCOL_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif



/*!
 * \defgroup protocolModule CAN Communication Protocol Module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the functions of the PCB/22-303 Software Communication protocol specifications.
 * 
 * ## Dependencies
 * 
 * This module requires the following library modules:
 * - Shared/CAN/MET_Can_Protocol.c
 * - Shared/CAN/MET_Can_Protocol.h
 * 
 * 
 * ## Protocol Communication setting
 *  
 * The Application implements the communication protocol  
 * described in the PCB/22-303 Software Communication protocol specifications.
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
        // Can Module Definitions
        static const unsigned char   MET_CAN_APP_DEVICE_ID    =  0x12 ;     //!< Application DEVICE CAN Id address
        static const unsigned char   MET_CAN_STATUS_REGISTERS =  2 ;        //!< Defines the total number of implemented STATUS registers 
        static const unsigned char   MET_CAN_DATA_REGISTERS   =  0 ;        //!< Defines the total number of implemented Application DATA registers 
        static const unsigned char   MET_CAN_PARAM_REGISTERS  =  54 ;       //!< Defines the total number of implemented PARAMETER registers 

     /// @}   moduleConstants

    /**
     * \defgroup moduleApiInterface CAN Protocol API interface
     * 
     * This section describes the functions implementing the Protocol module.
     * 
     *  @{
     */
        /// This is the Protocol initialization function
        ext void ApplicationProtocolInit ( void);

        /// This is the Main Loop protocol function
        ext void  ApplicationProtocolLoop(void);

     /// @}   moduleApiInterface
    

     /** \defgroup ErrorRegisterGroup ERROR REGISTER Definition
     *  
     *  This section describes the implementation of the Protocol Error Register
     *  @{
     */

        #define PERS0_LEFT_BLADE_ERROR    0x1
        #define PERS0_RIGHT_BLADE_ERROR   0x2
        #define PERS0_FRONT_BLADE_ERROR   0x4
        #define PERS0_BACK_BLADE_ERROR    0x8
        #define PERS0_TRAP_BLADE_ERROR    0x10


        
    /// @}   ErrorRegisterGroup
         
    /** \defgroup StatusRegisterGroup STATUS REGISTERS Definition
     *  
     *  This section describes the implementation of the Application STATUS Registers 
     *  @{
     */
        
     /// This is the list of the implemented STATUS REGISTERS    
     typedef enum{
        SYSTEM_STATUS_REGISTER = 0, //!< This is the Collimator internal status
        SYSTEM_CURRENT_FB_POSITION, //!< This is the Collimator current Front and Back position
        SYSTEM_CURRENT_LR_POSITION, //!< This is the Collimator current Left and Right position
        SYSTEM_CURRENT_T_POSITION, //!< This is the Collimator current Trap position
                
     }PROTO_STATUS_t;
    
    #define SYSTEM_COLLIMATION_STATUS_BYTE 0
    #define SYSTEM_FORMAT_INDEX_BYTE 1
    #define SYSTEM_TOMO_PULSE_BYTE 2
    #define SYSTEM_FLAGS_BYTE 3

    
    typedef enum{
        SYSTEM_COLLI_STATUS_OPEN_FORMAT = 0, //!< The Collimator is in Open Position
        SYSTEM_COLLI_STATUS_STANDARD_FORMAT, //!< The Collimator is in Format 2D mode and one of the standard format has been currently selected
        SYSTEM_COLLI_STATUS_CALIB_FORMAT, //!< The Collimator is in Calibration mode and a non standard position is selected
        SYSTEM_COLLI_STATUS_TOMO_MODE, //!< The Collimator is in Tomo Mode
        SYSTEM_COLLI_STATUS_OUT_POSITION, //!< The Collimator is in a wrong position
        SYSTEM_COLLI_STATUS_2D_EXECUTING, //!< The Collimator is executing a 2D collimation
                
    }PROTO_COLLIMATION_STATUS_t;
    
    #define SETBYTE_SYSTEM_COLLIMATION_STATUS(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_COLLIMATION_STATUS_BYTE, val) //!< This is the Collimation status byte
    #define GETBYTE_SYSTEM_COLLIMATION_STATUS  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_COLLIMATION_STATUS_BYTE) //!< This is the Collimation status byte
    #define SETBYTE_SYSTEM_2D_INDEX(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_FORMAT_INDEX_BYTE, val) //!< This is the 2D Index status byte
    #define GETBYTE_SYSTEM_2D_INDEX  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_FORMAT_INDEX_BYTE) //!< This is the 2D Index status byte
    #define SETBYTE_SYSTEM_TOMO_PULSE(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_TOMO_PULSE_BYTE, val) //!< This is the Tomo Pulse status byte
    #define GETBYTE_SYSTEM_TOMO_PULSE  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_TOMO_PULSE_BYTE) //!< This is the Tomo Pulse status byte
    #define SETBIT_SYSTEM_FLAGS_ERROR(val)  MET_Can_Protocol_SetStatusBit(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x1, val) //!< This bit is set when an error i s set into the Error register
    #define TESTBIT_SYSTEM_FLAGS_ERROR  MET_Can_Protocol_TestStatus(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x1) //!< This bit is set when an error i s set into the Error register

    
    #define SET_CURRENT_FRONT(vl,vh) {MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_FB_POSITION, 0, vl); MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_FB_POSITION, 1, vh);} //!< Sets the current position of the Front Blade
    #define SET_CURRENT_BACK(vl,vh) {MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_FB_POSITION, 2, vl); MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_FB_POSITION, 3, vh);} //!< Sets the current position of the Back Blade
    #define SET_CURRENT_LEFT(vl,vh) {MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_LR_POSITION, 0, vl); MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_LR_POSITION, 1, vh);} //!< Sets the current position of the Left Blade
    #define SET_CURRENT_RIGHT(vl,vh) {MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_LR_POSITION, 2, vl); MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_LR_POSITION, 3, vh);} //!< Sets the current position of the Right Blade
    #define SET_CURRENT_TRAP(vl,vh) {MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_T_POSITION, 0, vl); MET_Can_Protocol_SetStatusReg(SYSTEM_CURRENT_T_POSITION, 1, vh);} //!< Sets the current position of the Trap Blade
    
     /// @}   StatusRegisterGroup

     
    /** \defgroup DataRegisterGroup DATA REGISTERS Definition
     *  
     *  This section describes the implementation of the Protocol DATA Registers 
     *  @{
     */

         
        
    /// @}   DataRegisterGroup

    /** \defgroup ParamRegisterGroup PARAMETER Registers Definition
     *  
     *  This section describes the implementation of the Application PARAMETER Registers 
     *  @{
     */
        #define MAX_FORMAT_INDEX 20
        
        ext unsigned short protocolGetFormatLeft(int index); //!< This function returns the steps for the Left blade for the Index-format
        ext unsigned short protocolGetFormatRight(int index); //!< This function returns the steps for the Right blade for the Index-format
        ext unsigned short protocolGetFormatBack(int index); //!< This function returns the steps for the Back blade for the Index-format
        ext unsigned short protocolGetFormatFront(int index); //!< This function returns the steps for the Front blade for the Index-format
        ext unsigned short protocolGetFormatTrap(int index); //!< This function returns the steps for the Trap blade for the Index-format
        ext unsigned short protocolGetFilter(int index); //!< This function returns the steps for the Trap blade for the Index-format
        ext unsigned short protocolGetMirror(void); //!< This function returns the steps for the Trap blade for the Index-format
        
    /// @}   ParamRegisterGroup
        
        
     /** \defgroup CommandGroup COMMAND Execution Definition
     *  
     *  This section describes the Application Command Execution codes
     *  @{
     */
        
    /// This is the list of the implemented COMMANDS
    typedef enum{
      RESERVED_COMMAND = 0,    
      SET_OPEN_FORMAT,        
      SET_STANDARD_FORMAT,        
      SET_CALIBRATION_FORMAT,        
      SET_TOMO_MODE,        
    }PROTO_COMMAND_ENUM_t;
    
    /// This is the list of the implemented ERRORS
    typedef enum{
        COMMAND_ERROR_FORMAT_2D_SELECTION_FAILED = MET_CAN_COMMAND_APPLICATION_ERRORS,            
    }PROTO_COMMAND_ERROR_ENUM_t;

     /// @}   CommandGroup

        


         
/** @}*/ // protocolModule
#endif 