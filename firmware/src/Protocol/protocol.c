#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"
#include "../Motors/motlib.h"

static void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ); //!< This is the Command protocol callback
static volatile unsigned char current_command = 0;
/**
 * This function initializes the CAN Protocol module.
 * 
 * The function calls the library API  MET_Can_Protocol_Init() in order to:
 * + Set the Device Identifier;
 * + The number of implemented STATUS registers;
 * + The number of implemented DATA registers;
 * + The number of implemented PARAMETER registers;
 * + The Application revision 
 * + The protocol command handler;
 * 
 * The function initializes the Parameters with the default value   
 * with the library MET_Can_Protocol_SetDefaultParameter() function.
 * 
 */
void ApplicationProtocolInit ( void )
{
     
    // Initialize the Met Can Library
    MET_Can_Protocol_Init(MET_CAN_APP_DEVICE_ID, MET_CAN_STATUS_REGISTERS, MET_CAN_DATA_REGISTERS, MET_CAN_PARAM_REGISTERS, APPLICATION_MAJ_REV, APPLICATION_MIN_REV, APPLICATION_SUB_REV, ApplicationProtocolCommandHandler);
    
    // Assignes the default parameter here ..
    // MET_Can_Protocol_SetDefaultParameter(PARAM_CODE,0,0,0,0);
    
    // Assignes the Protocol Status initial value
    SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_OUT_POSITION);
    SETBYTE_SYSTEM_2D_INDEX(FORMAT_NOT_STANDARD);
    
}
  
/**
 * This function shall be called by the MAIN loop application 
 * in order to manage the reception/transmission protocol activities.
 * 
 * The function merely call the library function  MET_Can_Protocol_Loop().
 */
void inline ApplicationProtocolLoop(void){
    
    // Handles the transmission/reception protocol
    MET_Can_Protocol_Loop();        
    if(!current_command) return;
    
    // Format 2D command execution monitoring
    if((current_command == SET_STANDARD_FORMAT) ||(current_command == SET_OPEN_FORMAT) || (current_command == SET_CALIBRATION_FORMAT)) {
        if(motorsIsRunning()) return;
        
        
        // Sets the current activation error if there is one
        unsigned char error_pers0 = 0;
        if(leftIsError()) error_pers0 |= PERS0_LEFT_BLADE_ERROR;
        if(rightIsError()) error_pers0 |= PERS0_RIGHT_BLADE_ERROR;
        if(backIsError()) error_pers0 |= PERS0_BACK_BLADE_ERROR;
        if(frontIsError()) error_pers0 |= PERS0_FRONT_BLADE_ERROR;
        if(trapIsError()) error_pers0 |= PERS0_TRAP_BLADE_ERROR;
        MET_Can_Protocol_SetErrors(0, 0, &error_pers0, 0);
        
        // Command completed: returns the protocol command execution code 
        if(error_pers0) {
           SETBIT_SYSTEM_FLAGS_ERROR(true); // Sets the error flag bit in the status register
           SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_OUT_POSITION);
           MET_Can_Protocol_returnCommandError(COMMAND_ERROR_FORMAT_2D_SELECTION_FAILED); 
        }else{ 
           SETBIT_SYSTEM_FLAGS_ERROR(false);// Clears the error flag bit in the status register
           if(current_command == SET_STANDARD_FORMAT) SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_STANDARD_FORMAT);
           else if(current_command == SET_OPEN_FORMAT) SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_OPEN_FORMAT);
           else if(current_command == SET_CALIBRATION_FORMAT) SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_CALIB_FORMAT);
           
           MET_Can_Protocol_returnCommandExecuted(GETBYTE_SYSTEM_2D_INDEX,0);
        }
        
        current_command = 0;
        return;
    }
}

/**
 * This is the Command Handler implementation
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){
    unsigned short left,right,back,front,trap;
    unsigned char vl, vh;
    
    switch(cmd){
        case MET_COMMAND_ABORT:  // This is the Library mandatory 
            current_command = cmd;
            break;
        
        case SET_OPEN_FORMAT:
            // Activate the format collimation
            if(!selectFormat2D(0, 0, 0, 0, 0)){
                MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                return;
            }

            // Activates the command completion monitoring
            SETBYTE_SYSTEM_2D_INDEX(FORMAT_NOT_STANDARD);
            SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_2D_EXECUTING);
            SET_CURRENT_FRONT(0,0);
            SET_CURRENT_BACK(0,0);
            SET_CURRENT_LEFT(0,0);
            SET_CURRENT_RIGHT(0,0);
            SET_CURRENT_TRAP(0,0);
            
            MET_Can_Protocol_returnCommandExecuting();
            current_command = cmd;
        break;
        
        case SET_STANDARD_FORMAT:
            
            // Gets the position of the 2D format from the index received into d0 register
            if(!getStandard2DBladeParameters(d0, &front, &back, &left, &right, &trap)){ 
                MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_INVALID_DATA);
                return;
            }
            
            // Activate the format collimation
            if(!selectFormat2D(left, right, front, back, trap)){
                MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                return;
            }
            
            vl = (front & 0xFF); vh = ((front>>8) & 0xFF);SET_CURRENT_FRONT(vl,vh);
            vl = (back & 0xFF); vh = ((back>>8) & 0xFF);SET_CURRENT_BACK(vl,vh);
            vl = (left & 0xFF); vh = ((left>>8) & 0xFF);SET_CURRENT_LEFT(vl,vh);
            vl = (right & 0xFF); vh = ((right>>8) & 0xFF);SET_CURRENT_RIGHT(vl,vh);
            vl = (trap & 0xFF); vh = ((trap>>8) & 0xFF);SET_CURRENT_TRAP(vl,vh);
           
            
            // Activates the command completion monitoring
            SETBYTE_SYSTEM_2D_INDEX(d0);
            SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_2D_EXECUTING);
            MET_Can_Protocol_returnCommandExecuting();
            current_command = cmd;
            break;
            
         case SET_CALIBRATION_FORMAT:
            
            front = GET_CALIB_FRONT_POSITION;
            back  = GET_CALIB_BACK_POSITION;
            left  = GET_CALIB_LEFT_POSITION;
            right  = GET_CALIB_RIGHT_POSITION;
            trap = GET_CALIB_TRAP_POSITION;
            
            // Activate the format collimation
            if(!selectFormat2D(left, right, front, back, trap)){
                MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                return;
            }
            
            // Activates the command completion monitoring
            SETBYTE_SYSTEM_2D_INDEX(FORMAT_NOT_STANDARD);
            SETBYTE_SYSTEM_COLLIMATION_STATUS(SYSTEM_COLLI_STATUS_2D_EXECUTING);
            vl = (front & 0xFF); vh = ((front>>8) & 0xFF);SET_CURRENT_FRONT(vl,vh);
            vl = (back & 0xFF); vh = ((back>>8) & 0xFF);SET_CURRENT_BACK(vl,vh);
            vl = (left & 0xFF); vh = ((left>>8) & 0xFF);SET_CURRENT_LEFT(vl,vh);
            vl = (right & 0xFF); vh = ((right>>8) & 0xFF);SET_CURRENT_RIGHT(vl,vh);
            vl = (trap & 0xFF); vh = ((trap>>8) & 0xFF);SET_CURRENT_TRAP(vl,vh);
           
            
            MET_Can_Protocol_returnCommandExecuting();
            current_command = cmd;
            break;    
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}
 
bool getStandard2DBladeParameters(unsigned char index, unsigned short* front, unsigned short* back,unsigned short* left, unsigned short* right, unsigned short* trap){
    unsigned char ilr, ib;
    
    if(index < FORMAT_STANDARD1) return false;
    if(index >= FORMAT_STANDARD_LAST_SELECTABLE) return false;
    
    *front = (unsigned short) MET_Can_Protocol_GetParameter(PARAM_FT_2D_COLLI,0) + (unsigned short) MET_Can_Protocol_GetParameter(PARAM_FT_2D_COLLI,1) * 256;
    *trap  = (unsigned short) MET_Can_Protocol_GetParameter(PARAM_FT_2D_COLLI,2) + (unsigned short) MET_Can_Protocol_GetParameter(PARAM_FT_2D_COLLI,3) * 256;
   
    ilr = (index - FORMAT_STANDARD1) * 2 + PARAM_LR_STANDARD1;
    ib = (index - FORMAT_STANDARD1) * 2 + PARAM_B_STANDARD1;
    *left = (unsigned short) MET_Can_Protocol_GetParameter(ilr,0) + (unsigned short) MET_Can_Protocol_GetParameter(ilr,1) * 256;
    *right = (unsigned short) MET_Can_Protocol_GetParameter(ilr,2) + (unsigned short) MET_Can_Protocol_GetParameter(ilr,3) * 256;
    *back = (unsigned short) MET_Can_Protocol_GetParameter(ib,0) + (unsigned short) MET_Can_Protocol_GetParameter(ib,1) * 256;            
    return true;

    
}

bool getCalibBladeParameters(unsigned short* front, unsigned short* back,unsigned short* left, unsigned short* right, unsigned short* trap){
    
    *front = 0;
    *trap  = 0;
    *left = 0;
    *right = 0;
    *back = 0;
       
    return true;
 
}