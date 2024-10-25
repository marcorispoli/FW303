#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"
#include "../Motors/motlib.h"
#include "../Motors/format_collimation.h"
#include "../Motors/filter.h"
#include "../Motors/mirror.h"

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
    
    // Set to zero all the default parameters
    for(int i=0; i< MET_CAN_PARAM_REGISTERS; i++) {
        MET_Can_Protocol_SetDefaultParameter(i,0,0,0,0);
    }
    
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
    
}

/**
 * This is the Command Handler implementation
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){
    
    
    switch(cmd){
        case MET_COMMAND_ABORT:  // This is the Library mandatory 
            MET_Can_Protocol_returnCommandAborted();
            break;
        
        case CMD_SET_FORMAT:
            
            switch(activateFormatCollimation(d0)){
                case MOT_RET_IN_TARGET:
                    MET_Can_Protocol_returnCommandExecuted(d0,0);
                    break;
                case MOT_RET_STARTED:
                    MET_Can_Protocol_returnCommandExecuting();
                    break;
                case MOT_RET_ERR_BUSY:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                    break;
                case MOT_RET_ERR_INVALID_TARGET:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_INVALID_DATA);
                    break;
            }
            
            break;
            
         case CMD_SET_FILTER:
            
            switch(activateFilter(d0)){
                case MOT_RET_IN_TARGET:
                    MET_Can_Protocol_returnCommandExecuted(d0,0);
                    break;
                case MOT_RET_STARTED:
                    MET_Can_Protocol_returnCommandExecuting();
                    break;
                case MOT_RET_ERR_BUSY:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                    break;
                case MOT_RET_ERR_INVALID_TARGET:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_INVALID_DATA);
                    break;
            }
            
            break;
            
            case CMD_SET_MIRROR:
            
            switch(activateMirror(d0)){
                case MOT_RET_IN_TARGET:
                    MET_Can_Protocol_returnCommandExecuted(d0,0);
                    break;
                case MOT_RET_STARTED:
                    MET_Can_Protocol_returnCommandExecuting();
                    break;
                case MOT_RET_ERR_BUSY:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
                    break;
                case MOT_RET_ERR_INVALID_TARGET:
                    MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_INVALID_DATA);
                    break;
            }
            
        case CMD_SET_LIGHT:
            if(d0) lightActivation(true);
            else lightActivation(false);
            MET_Can_Protocol_returnCommandExecuted(d0,0);
            break;
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}
        

bool protocolGet2DFormat(int slot, unsigned short* left, unsigned short* right, unsigned short* front, unsigned short* back, unsigned short* trap ){
    if(slot >= 20) return false;
    
    decodeParamRegister(&FrontBackParamRegisterArray[slot]);
    decodeParamRegister(&LeftRightParamRegisterArray[slot]);
    decodeParamRegister(&TrapParamRegisterArray[slot/2]);
    
    *front = (unsigned short) FrontBackParamRegisterArray[slot].FL + 256 * (unsigned short) FrontBackParamRegisterArray[slot].FH;
    *back = (unsigned short) FrontBackParamRegisterArray[slot].BL + 256 * (unsigned short) FrontBackParamRegisterArray[slot].BH;
    *left = (unsigned short) LeftRightParamRegisterArray[slot].LL + 256 * (unsigned short) LeftRightParamRegisterArray[slot].LH;
    *right = (unsigned short) LeftRightParamRegisterArray[slot].RL + 256 * (unsigned short) LeftRightParamRegisterArray[slot].RH;
    
    if(slot%2) *trap = (unsigned short) TrapParamRegisterArray[slot/2].T1L + 256 * (unsigned short) TrapParamRegisterArray[slot/2].T1H;
    else *trap = (unsigned short) TrapParamRegisterArray[slot/2].T0L + 256 * (unsigned short) TrapParamRegisterArray[slot/2].T0H;
    
    return true;
    
}

bool protocolGetFilter(int slot, unsigned short* flt){
    if(slot >= 3) return false;
    
    decodeParamRegister(&FilterParamRegisterArray[slot/2]);
    if(slot%2) *flt = (unsigned short) FilterParamRegisterArray[slot/2].F1L + 256 * (unsigned short) FilterParamRegisterArray[slot/2].F1H;
    else *flt = (unsigned short) FilterParamRegisterArray[slot/2].F0L + 256 * (unsigned short) FilterParamRegisterArray[slot/2].F0H;
    return true;
}    

unsigned short protocolGetMirror(void){
    decodeParamRegister(&MirrorParamRegister);
    return (unsigned short) MirrorParamRegister.ML + 256 * (unsigned short) MirrorParamRegister.MH;
} 


void encodeStatusRegister(void* reg){
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 0, ((REGISTER_STRUCT_t*) reg)->d0 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 1, ((REGISTER_STRUCT_t*) reg)->d1 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 2, ((REGISTER_STRUCT_t*) reg)->d2 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 3, ((REGISTER_STRUCT_t*) reg)->d3 );
}

void decodeParamRegister(void* reg){
     int register_offset;
     
    if((long) reg == (long) &MirrorParamRegister){
        register_offset = PARAM_MIRROR_IDX;
    }
    
    if( ((long) reg >= (long) FilterParamRegisterArray) && ((long) reg < (long) FilterParamRegisterArray + 3 * sizeof(PARAM_FILTER_t)) ) {
        register_offset = PARAM_FILTER_FIRST_IDX + ((long) reg - (long) FilterParamRegisterArray) / sizeof(PARAM_FILTER_t);
    }
    
    if( ((long) reg >= (long) TrapParamRegisterArray) && ((long) reg < (long) TrapParamRegisterArray + 10 * sizeof(PARAM_TRAP_t)) ) {
        register_offset = PARAM_TRAP_FIRST_IDX + ((long) reg - (long) TrapParamRegisterArray) / sizeof(PARAM_TRAP_t);        
    }

    if( ((long) reg >= (long) LeftRightParamRegisterArray) && ((long) reg < (long) LeftRightParamRegisterArray + 20 * sizeof(PARAM_LR_t)) ) {
        register_offset = PARAM_LR_FIRST_IDX + ((long) reg - (long) LeftRightParamRegisterArray) / sizeof(PARAM_LR_t); 
    }

    if( ((long) reg >= (long) FrontBackParamRegisterArray) && ((long) reg < (long) FrontBackParamRegisterArray + 20 * sizeof(PARAM_FB_t)) ) {
        register_offset = PARAM_FB_FIRST_IDX + ((long) reg - (long) FrontBackParamRegisterArray) / sizeof(PARAM_FB_t); 
    }
    
   
    ((REGISTER_STRUCT_t*) reg)->d0 = MET_Can_Protocol_GetParameter(register_offset, 0);
    ((REGISTER_STRUCT_t*) reg)->d1 = MET_Can_Protocol_GetParameter(register_offset, 1);
    ((REGISTER_STRUCT_t*) reg)->d2 = MET_Can_Protocol_GetParameter(register_offset, 2);
    ((REGISTER_STRUCT_t*) reg)->d3 = MET_Can_Protocol_GetParameter(register_offset, 3);
    return;
   
    
}