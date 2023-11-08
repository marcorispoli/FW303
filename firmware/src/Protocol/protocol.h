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
        static const unsigned char   MET_CAN_STATUS_REGISTERS =  4 ;        //!< Defines the total number of implemented STATUS registers 
        static const unsigned char   MET_CAN_DATA_REGISTERS   =  3 ;        //!< Defines the total number of implemented Application DATA registers 
        static const unsigned char   MET_CAN_PARAM_REGISTERS  =  43 ;       //!< Defines the total number of implemented PARAMETER registers 

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

         /// This is the list of the implemented DATA REGISTERS    
         typedef enum{
            DATA_CALIBRATION_FB_POSITION = 0, //!< This is the Front and Back calibration blades positions
            DATA_CALIBRATION_LR_POSITION, //!< This is the Left and Right calibration blades positions
            DATA_CALIBRATION_T_POSITION, //!< This is the Trap calibration blades positions
         }PROTO_DATA_t;
        
        #define GET_CALIB_FRONT_POSITION  ((unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_FB_POSITION, 0) + 256 * (unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_FB_POSITION, 1))
        #define GET_CALIB_BACK_POSITION  ((unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_FB_POSITION, 2) + 256 * (unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_FB_POSITION, 3))
        #define GET_CALIB_LEFT_POSITION  ((unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_LR_POSITION, 0) + 256 * (unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_LR_POSITION, 1))
        #define GET_CALIB_RIGHT_POSITION  ((unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_LR_POSITION, 2) + 256 * (unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_LR_POSITION, 3))
        #define GET_CALIB_TRAP_POSITION  ((unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_T_POSITION, 0) + 256 * (unsigned short) MET_Can_Protocol_GetData(DATA_CALIBRATION_T_POSITION, 1))
        
        
    /// @}   DataRegisterGroup

    /** \defgroup ParamRegisterGroup PARAMETER Registers Definition
     *  
     *  This section describes the implementation of the Application PARAMETER Registers 
     *  @{
     */
        
        /// This is the list of the implemented PARAMETER REGISTERS        
        typedef enum{
            PARAM_FT_2D_COLLI = 0, //!< This is the parameter setting the Front and Trap 2D positions           
            PARAM_LR_STANDARD1,//!< This is the parameter setting the Custom1 Left and Right blades positions
            PARAM_B_STANDARD1,//!< This is the parameter setting the Custom1 Back blade positions
            PARAM_LR_STANDARD2,//!< This is the parameter setting the Custom2 Left and Right blades positions
            PARAM_B_STANDARD2,//!< This is the parameter setting the Custom2 Back blade positions
            PARAM_LR_STANDARD3,//!< This is the parameter setting the Custom3 Left and Right blades positions
            PARAM_B_STANDARD3,//!< This is the parameter setting the Custom3 Back blade positions
            PARAM_LR_STANDARD4,//!< This is the parameter setting the Custom4 Left and Right blades positions
            PARAM_B_STANDARD4,//!< This is the parameter setting the Custom4 Back blade positions
            PARAM_LR_STANDARD5,//!< This is the parameter setting the Custom5 Left and Right blades positions
            PARAM_B_STANDARD5,//!< This is the parameter setting the Custom5 Back blade positions
            PARAM_LR_STANDARD6,//!< This is the parameter setting the Custom6 Left and Right blades positions
            PARAM_B_STANDARD6,//!< This is the parameter setting the Custom6 Back blade positions
            PARAM_LR_STANDARD7,//!< This is the parameter setting the Custom7 Left and Right blades positions
            PARAM_B_STANDARD7,//!< This is the parameter setting the Custom7 Back blade positions
            PARAM_LR_STANDARD8,//!< This is the parameter setting the Custom8 Left and Right blades positions
            PARAM_B_STANDARD8,//!< This is the parameter setting the Custom8 Back blade positions
            PARAM_LR_STANDARD9,//!< This is the parameter setting the Custom9 Left and Right blades positions
            PARAM_B_STANDARD9,//!< This is the parameter setting the Custom9 Back blade positions
            PARAM_LR_STANDARD10,//!< This is the parameter setting the Custom10 Left and Right blades positions
            PARAM_B_STANDARD10,//!< This is the parameter setting the Custom10 Back blade positions
            PARAM_LR_STANDARD11,//!< This is the parameter setting the Custom11 Left and Right blades positions
            PARAM_B_STANDARD11,//!< This is the parameter setting the Custom11 Back blade positions
            PARAM_LR_STANDARD12,//!< This is the parameter setting the Custom12 Left and Right blades positions
            PARAM_B_STANDARD12,//!< This is the parameter setting the Custom12 Back blade positions
            PARAM_LR_STANDARD13,//!< This is the parameter setting the Custom13 Left and Right blades positions
            PARAM_B_STANDARD13,//!< This is the parameter setting the Custom13 Back blade positions
            PARAM_LR_STANDARD14,//!< This is the parameter setting the Custom14 Left and Right blades positions
            PARAM_B_STANDARD14,//!< This is the parameter setting the Custom14 Back blade positions
            PARAM_LR_STANDARD15,//!< This is the parameter setting the Custom15 Left and Right blades positions
            PARAM_B_STANDARD15,//!< This is the parameter setting the Custom15 Back blade positions
            PARAM_LR_STANDARD16,//!< This is the parameter setting the Custom16 Left and Right blades positions
            PARAM_B_STANDARD16,//!< This is the parameter setting the Custom16 Back blade positions
            PARAM_LR_STANDARD17,//!< This is the parameter setting the Custom17 Left and Right blades positions
            PARAM_B_STANDARD17,//!< This is the parameter setting the Custom17 Back blade positions
            PARAM_LR_STANDARD18,//!< This is the parameter setting the Custom18 Left and Right blades positions
            PARAM_B_STANDARD18,//!< This is the parameter setting the Custom18 Back blade positions
            PARAM_LR_STANDARD19,//!< This is the parameter setting the Custom19 Left and Right blades positions
            PARAM_B_STANDARD19,//!< This is the parameter setting the Custom19 Back blade positions
            PARAM_LR_STANDARD20,//!< This is the parameter setting the Custom20 Left and Right blades positions
            PARAM_B_STANDARD20,//!< This is the parameter setting the Custom20 Back blade positions
        
        }PROTO_PARAMETERS_t;

         typedef enum{
             FORMAT_NOT_STANDARD = 0,
             FORMAT_STANDARD1,
             FORMAT_STANDARD2,
             FORMAT_STANDARD3,
             FORMAT_STANDARD4,
             FORMAT_STANDARD5,
             FORMAT_STANDARD6,
             FORMAT_STANDARD7,
             FORMAT_STANDARD8,
             FORMAT_STANDARD9,
             FORMAT_STANDARD10,
             FORMAT_STANDARD11,
             FORMAT_STANDARD12,
             FORMAT_STANDARD13,
             FORMAT_STANDARD14,
             FORMAT_STANDARD15,
             FORMAT_STANDARD16,
             FORMAT_STANDARD17,
             FORMAT_STANDARD18,
             FORMAT_STANDARD19,
             FORMAT_STANDARD20,
             FORMAT_STANDARD_LAST_SELECTABLE,
         }PROTO_FORMAD_2D_INDEX;
        ext bool getStandard2DBladeParameters(unsigned char index, unsigned short* front, unsigned short* back,unsigned short* left, unsigned short* right, unsigned short* trap); //!< This function returns the blades configuration from the selection index
        ext bool getCalibBladeParameters(unsigned short* front, unsigned short* back,unsigned short* left, unsigned short* right, unsigned short* trap);//!< This function returns the blades for calibration
        
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