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
 * \defgroup CANPROT CAN Communication Protocol Module
 * \ingroup applicationModule
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
 */


/// \ingroup CANPROT 
/// This structure of a generic protocol register 
typedef struct {
    unsigned char idx;
    unsigned char d0;
    unsigned char d1;
    unsigned char d2;
    unsigned char d3;
}REGISTER_STRUCT_t;
    

/**
 * \addtogroup CANPROT 
 * 
 * ## Protocol Setting
 * 
 * Can ID address: \ref MET_CAN_APP_DEVICE_ID  
 * Number of STATUS registers: MET_CAN_STATUS_REGISTERS
 * Number of DATA registers: MET_CAN_DATA_REGISTERS
 * Number of PARAM registers: MET_CAN_PARAM_REGISTERS
 * 
 */ 

/// \ingroup CANPROT 
/// Protocol Definition Data
typedef enum{
    MET_CAN_APP_DEVICE_ID    =  0x12,      //!< Application DEVICE CAN Id address
    MET_CAN_STATUS_REGISTERS =  2,        //!< Defines the total number of implemented STATUS registers 
    MET_CAN_DATA_REGISTERS   =  0,        //!< Defines the total number of implemented Application DATA registers 
    MET_CAN_PARAM_REGISTERS  =  54       //!< Defines the total number of implemented PARAMETER registers 
}PROTOCOL_DEFINITION_DATA_t;

/**
 * \addtogroup CANPROT
 * 
 * ## Application API
 * 
 * + ApplicationProtocolInit() : this is the module initialization;
 * + ApplicationProtocolLoop() : this is the workflow routine to be placed into the application main loop;
 * + encodeStatusRegister() : This is the function to encode a Status register to the Can Register array 
 * + decodeParamRegister() : This is the function to update and decodea Param register with the actual Can Registers content
 * + protocolGet2DFormat() : This is the function to get the 2D format from the Can Parameters
 * + protocolGetFilter() : This is the function to get the Filter positionfrom the Can Parameters
 * + protocolGetMirror() : This is the function to get the Mirror position from the Can Parameters
 * 
 */


/**
 * \ingroup CANPROT 
 * Protocol Initialization routine
 */
ext void ApplicationProtocolInit ( void);

/// \ingroup CANPROT 
/// This is the Main Loop protocol function
ext void  ApplicationProtocolLoop(void);

/// \ingroup CANPROT 
/// This is the function to get the 2D format from the Can Parameters
ext bool protocolGet2DFormat(int slot, unsigned short* left, unsigned short* right, unsigned short* front, unsigned short* back, unsigned short* trap );

/// \ingroup CANPROT 
/// This is the function to get the Filter positionfrom the Can Parameters
ext bool protocolGetFilter(int slot, unsigned short* flt);

/// \ingroup CANPROT 
/// This is the function to get the Mirror position from the Can Parameters
ext unsigned short protocolGetMirror(void);

/// \ingroup CANPROT 
/// This is the function to encode a Status register to the Can Register array
ext void encodeStatusRegister(void* reg);

/// \ingroup CANPROT 
/// This is the function to update and decodea Param register with the actual Can Registers content
ext void decodeParamRegister(void* reg);

//________________________________________ STATUS REGISTER DEFINITION SECTION _

/**
 * \addtogroup CANPROT
 * 
 * ## STATUS register description
 * 
 * There are the following Status registers:\n
 * (See \ref STATUS_INDEX_t enum table)
 * 
 * |IDX|NAME|DESCRIPTION|
 * |:--|:--|:--|
 * |0|SYSTEM|\ref SYSTEM_STATUS_t|
 * |1|TUBE|\ref TUBE_STATUS_t|
 *   
 */

/// \ingroup CANPROT
/// Defines the address table for the System Registers 
typedef enum{
    SYSTEM_STATUS_IDX = 0, //!< This is the Collimator internal status
    TUBE_STATUS_IDX,       //!< This is the register with the Tube monitored data
}STATUS_INDEX_t;


    /// \ingroup CANPROT
    /// Defines the current device activity status.
    ///
    /// The possible devices that are rapresented with this enumeration table are:
    /// + Collimation Format;
    /// + Mirror Format;
    /// + Filter Format;
    ///
    typedef enum{
        FORMAT_UNDEFINED = 0, //!< The given format is undefined (unknown position of the blades or not in the required target position)
        FORMAT_EXECUTING = 1, //!< The format is executing a position to a target 
        FORMAT_SELECTED = 2,  //!< The format is successfully selected
    }FORMAT_ACTIVITY_t;
    
    #define MAX_FORMAT_INDEX 20
    #define MAX_FILTER_INDEX 5

    /**
     * \addtogroup CANPROT
     * 
     * ### SYSTEM STATUS REGISTER
     * 
     * + Description: SYSTEM_STATUS_t;
     * + IDX: \ref SYSTEM_STATUS_IDX;
     * 
     * This register provides the current status of the collimator devices:
     * + Collimation format: front, back, right, left, trap blades;
     * + Mirror Format: the mirror acitvities;
     * + Filter selector: the filter selction activity;
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0.0 - 0.2|2D Format Collimation Activity|This is the current 2D format collimation activity \ref FORMAT_ACTIVITY_t|
     * |0.3 - 0.7|Target Format Index|This is the current active format target index|
     * |1.0 - 1.2|Filter Activity|This is the current Filter activity \ref FORMAT_ACTIVITY_t|
     * |1.3 - 1.7|Target Filter Index|This is the current active target filter index|
     * |2.0 - 2.2|Mirror Activity|This is the current Mirror position activity \ref FORMAT_ACTIVITY_t|
     * |2.3|InOut|1 = In-Field, 0 = Out-Field|
     * |2.4|Light|1 = Light ON, 0 = Light OFF|
     * |3|-|-|
     * 
     */ 

    /// \ingroup CANPROT
    /// System Status description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char format_2d_activity:3;      //!< This is the current 2D format collimation activity \ref FORMAT_ACTIVITY_t
        unsigned char format_selected_index:5;  //!< This is the current target format selected index
        
        unsigned char format_filter_activity:3;      //!< This is the current Filter activity \ref FORMAT_ACTIVITY_t
        unsigned char format_filter_index:5;    //!< This is the current target filter selected index

        unsigned char format_mirror_activity:3; //!< This is the current Mirror activity \ref FORMAT_ACTIVITY_t
        unsigned char in_field_position:1;      //!< Set if the Mirror is In-FIELD or cleared if the Mirror is Out Field
        unsigned char collimation_light:1;      //!< Set if the collimation light is ON
        
        unsigned char d3; 
    }SYSTEM_STATUS_t;

    
    
     /**
     * \addtogroup CANPROT
     * 
     * ### TUBE STATUS REGISTER
     * 
     * + Description: TUBE_STATUS_t;
     * + IDX: \ref TUBE_STATUS_IDX;
     * 
     * This register provides the Tube temperature monitoring:
     * + Bulb Temperature: the temperature measured by the Bulb sensor;
     * + Stator Temperature: the temperature measured near the stator windings;
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|Stator Temperature|Percentage of Stator temperature respect the maximum allowed|
     * |1|Bulb Temperature|Percentage of Bulb temperature respect the maximum allowed|
     * |2|-|-|
     * |3|-|-|
     * 
     */ 

    /// \ingroup CANPROT
    /// Tube temperature monitoring description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char stator;      //!< This is the Stator percent temperature
        unsigned char bulb;      //!< This is the Bulb percent temperature
        unsigned char d2; 
        unsigned char d3; 
    }TUBE_STATUS_t;
    
     #ifdef _PROTOCOL_C
        /// \ingroup CANPROT
        /// Declaration of the System Status Register  global variables
        SYSTEM_STATUS_t SystemStatusRegister = {.idx=SYSTEM_STATUS_IDX};

        /// \ingroup CANPROT
        /// Declaration of the Tube Status Register  global variables
        TUBE_STATUS_t TubeStatusRegister = {.idx=TUBE_STATUS_IDX};
        
    #else
        extern SYSTEM_STATUS_t SystemStatusRegister;
        extern TUBE_STATUS_t TubeStatusRegister;        
    #endif  
        
        
//________________________________________ DATA REGISTER DEFINITION SECTION _   

/**
* \addtogroup CANPROT
* 
* ## DATA register description
* 
* No Data registers are implemented
*
*/
        
//________________________________________ PARAM REGISTER DEFINITION SECTION _     
 
 /**
 * \addtogroup CANPROT
 * 
 * ## PARAMETER register description
 * 
 * There are the following PARAMETER registers:\n
 * (See \ref PARAMETER_INDEX_t enum table)
 * 
 * |IDX|NAME|DESCRIPTION|
 * |:--|:--|:--|
 * |0 to 19|FB_Collimation|Front and Back collimation positions for indexes 0 to 19|
 * |20 to 39|LR_Collimation|Left and Right collimation positions for indexes 0 to 19|
 * |40 to 49|Trap_Collimation|Trap collimation positions for indexes 0 to 19|
 * |50 to 52|Filter Slots|Position of the filter slots from 0 to 6|
 * |53 |Mirror Position|In-Field Mirror position|
 *   
 */
        
/// \ingroup CANPROT
/// Defines the address table for the System Registers 
typedef enum{
    PARAM_FB_FIRST_IDX = 0, //!< This is the first address of the FB array
    PARAM_LR_FIRST_IDX = 20, //!< This is the first address of the LR array
    PARAM_TRAP_FIRST_IDX = 40, //!< This is the first address of the Trap array
    PARAM_FILTER_FIRST_IDX = 50, //!< This is the first address of the Filter array
    PARAM_MIRROR_IDX = 53, //!< This is the Mirror InField position index
}PARAM_INDEX_t;
        
    /**
     * \addtogroup CANPROT
     * 
     * ### FRONT-BACK PARAMETER REGISTER
     * 
     * + Description: PARAM_FB_t;
     * + IDX: \ref PARAM_FB_FIRST_IDX to ref PARAM_FB_FIRST_IDX + 20;
     * 
     * This register provides the position of the Front and back blades 
     * along the possible formats.
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|FL|Front low byte|
     * |1|FH|Front high byte|
     * |2|BL|Back low byte|
     * |3|BH|Back high byte|
     * 
     */ 

    /// \ingroup CANPROT
    /// Front-Back Parameter array description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char FL; //!< Front low byte
        unsigned char FH; //!< Front high byte
        unsigned char BL; //!< Back low byte
        unsigned char BH; //!< Back high byte
        
    }PARAM_FB_t;
    
    /**
     * \addtogroup CANPROT
     * 
     * ### LEFT-RIGHT PARAMETER REGISTER
     * 
     * + Description: PARAM_LR_t;
     * + IDX: \ref PARAM_LR_FIRST_IDX to ref PARAM_LR_FIRST_IDX + 20;
     * 
     * This register provides the position of the Left and Right blades 
     * along the possible formats.
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|LL|Left low byte|
     * |1|LH|Left high byte|
     * |2|RL|Right low byte|
     * |3|RH|Right high byte|
     * 
     */ 

    /// \ingroup CANPROT
    /// Left-Right Parameter array description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char LL; //!< Left low byte
        unsigned char LH; //!< Left high byte
        unsigned char RL; //!< Right low byte
        unsigned char RH; //!< Right high byte
        
    }PARAM_LR_t;

    /**
     * \addtogroup CANPROT
     * 
     * ### TRAP PARAMETER REGISTER
     * 
     * + Description: PARAM_TRAP_t;
     * + IDX: \ref PARAM_TRAP_FIRST_IDX to ref PARAM_TRAP_FIRST_IDX + 10;
     * 
     * This register provides the position of the Trap blades.
     * Each parameter provides two trap indexes. 
     * 
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|T0L|Trap-0 low byte|
     * |1|T0H|Trap-0 high byte|
     * |2|T1L|Trap-1 low byte|
     * |3|T1H|Trap-1 high byte|
     * 
     */ 

    /// \ingroup CANPROT
    /// Trap Parameter array description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char T0L; //!< Trap-0 low byte
        unsigned char T0H; //!< Trap-0 high byte
        unsigned char T1L; //!< Trap-1 low byte
        unsigned char T1H; //!< Trap-1 high byte
        
    }PARAM_TRAP_t;

    /**
     * \addtogroup CANPROT
     * 
     * ### FILTER PARAMETER REGISTER
     * 
     * + Description: PARAM_FILTER_t;
     * + IDX: \ref PARAM_FILTER_FIRST_IDX to ref PARAM_FILTER_FIRST_IDX + 3;
     * 
     * This register provides the position of two filter slots.
     * 
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|F0L|Filter-0 low byte|
     * |1|F0H|Filter-0 high byte|
     * |2|F1L|Filter-1 low byte|
     * |3|F1H|Filter-1 high byte|
     * 
     */ 

    /// \ingroup CANPROT
    /// Filter Parameter array description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char F0L; //!< Filter-0 low byte
        unsigned char F0H; //!< Filter-0 high byte
        unsigned char F1L; //!< Filter-1 low byte
        unsigned char F1H; //!< Filter-1 high byte
        
    }PARAM_FILTER_t;

    /**
     * \addtogroup CANPROT
     * 
     * ### MIRROR PARAMETER REGISTER
     * 
     * + Description: PARAM_MIRROR_t;
     * + IDX: \ref PARAM_MIRROR_IDX;
     * 
     * This register provides the position of the mirror when set In-Field.
     * 
     * 
     * |BYTE.BIT|NAME|DESCRIPTION|
     * |:--|:--|:--|
     * |0|ML|Mirror low byte|
     * |1|MH|Mirror high byte|
     * |2|-|-|
     * |3|-|-|
     * 
     */ 

    /// \ingroup CANPROT
    /// Mirror Parameter description structure
    typedef struct {
        const unsigned char idx; //!< Address constant 

        unsigned char ML; //!< Filter-0 low byte
        unsigned char MH; //!< Filter-0 high byte
        unsigned char d2; 
        unsigned char d3; 
        
    }PARAM_MIRROR_t;
    
    /// \ingroup CANPROT
    /// Declaration of the FB Param register array global variables
    ext PARAM_FB_t FrontBackParamRegisterArray[20];
    
    /// \ingroup CANPROT
    /// Declaration of the LR Param register array global variables
    ext PARAM_LR_t LeftRightParamRegisterArray[20];       

    /// \ingroup CANPROT
    /// Declaration of the Trap Param register array global variables
    ext PARAM_TRAP_t TrapParamRegisterArray[10];
        
    /// \ingroup CANPROT
    /// Declaration of the Filter Param register array global variables
    ext PARAM_FILTER_t FilterParamRegisterArray[3];

    /// \ingroup CANPROT
    /// Declaration of the Mirror Param register global variables
    ext PARAM_MIRROR_t MirrorParamRegister;


        
//_______________________________________ PROTOCOL COMMANDS DEFINITION SECTION _ 
        
 /**
 * \addtogroup CANPROT
 * 
 * ## PROTOCOL COMMANDS DESCRIPTION
 * 
 */     

/// \ingroup CANPROT
/// This is the list of the implemented COMMANDS
typedef enum{
   CMD_ABORT = 0,      //!< Abort Command
   CMD_SET_FORMAT = 1, //!< MAIN-CPU requests for a standard 2D collimation
   CMD_SET_FILTER = 2, //!< MAIN-CPU requests for filter selection
   CMD_SET_MIRROR = 3, //!< MAIN-CPU requests for Mirror activation
   CMD_SET_LIGHT = 4,  //!< MAIN-CPU requests for Light activation
}PROTOCOL_COMMANDS_t;

/// \ingroup CANPROT
/// This is the list of the application COMMAND Errors
typedef enum{
   ERROR_IN_FORMAT_POSITIONING = MET_CAN_COMMAND_APPLICATION_ERRORS,      //!< An error during the format collimation has been signaled  
   ERROR_IN_FILTER_POSITIONING,//!< An error during the filter selection has been signaled
   ERROR_IN_MIRROR_POSITIONING,//!< An error during the mirror selection has been signaled
}PROTOCOL_COMMANDS_ERRORS_t;

#endif 
    