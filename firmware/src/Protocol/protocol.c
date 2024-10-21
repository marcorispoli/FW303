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
    
    // Set to zero the blades as default
    for(int i=0; i< MAX_FORMAT_INDEX * 2; i++)  MET_Can_Protocol_SetDefaultParameter(i,0,0,0,0);
    for(int i=MAX_FORMAT_INDEX * 2; i< MAX_FORMAT_INDEX * 2 + MAX_FORMAT_INDEX/2; i++)  MET_Can_Protocol_SetDefaultParameter(i,0,0,0,0);
    
    // Set to zero the filters as default
    for(int i=MAX_FORMAT_INDEX * 2 + MAX_FORMAT_INDEX/2 ; i< MAX_FORMAT_INDEX * 2 + MAX_FORMAT_INDEX/2 + 3; i++)  MET_Can_Protocol_SetDefaultParameter(i,0,0,0,0);
    
    // Sets to zero the mirror
    MET_Can_Protocol_SetDefaultParameter(MAX_FORMAT_INDEX * 2 + MAX_FORMAT_INDEX/2 + 3 ,0,0,0,0);
    
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
            current_command = cmd;
            break;
        
        case SET_OPEN_FORMAT:
            // Activate the format collimation
            
            
            MET_Can_Protocol_returnCommandExecuting();
            current_command = cmd;
        break;
        
        case SET_STANDARD_FORMAT:
            
            
            MET_Can_Protocol_returnCommandExecuting();
            current_command = cmd;
            break;
            
         
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}
 
        
        
        

unsigned short protocolGetFormatLeft(int index){
    if(index >= MAX_FORMAT_INDEX) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter(MAX_FORMAT_INDEX + index ,0) + (unsigned short) MET_Can_Protocol_GetParameter(MAX_FORMAT_INDEX + index ,1) * 256;    
}
unsigned short protocolGetFormatRight(int index){
    if(index >= MAX_FORMAT_INDEX) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter(MAX_FORMAT_INDEX + index ,2) + (unsigned short) MET_Can_Protocol_GetParameter(MAX_FORMAT_INDEX + index ,3) * 256;    
}       
        
unsigned short protocolGetFormatFront(int index){
    if(index >= MAX_FORMAT_INDEX) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter(index ,0) + (unsigned short) MET_Can_Protocol_GetParameter(index ,1) * 256;    
}
unsigned short protocolGetFormatBack(int index){
    if(index >= MAX_FORMAT_INDEX) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter(index ,2) + (unsigned short) MET_Can_Protocol_GetParameter(index ,3) * 256;    
}       
unsigned short protocolGetFormatTrap(int index){
    if(index >= MAX_FORMAT_INDEX) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + index/2 , 2* (index%2)) + (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + index/2 ,2*(index%2)+1) * 256;    
}    

unsigned short protocolGetFilter(int index){
    if(index >= 5) return false;
    return (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + (MAX_FORMAT_INDEX/2) + index/2 , 2* (index%2)) + (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + (MAX_FORMAT_INDEX/2) + index/2 ,2*(index%2)+1) * 256;    
}    

unsigned short protocolGetMirror(void){
    
    return (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + (MAX_FORMAT_INDEX/2) + 3  , 0) + (unsigned short) MET_Can_Protocol_GetParameter((2*MAX_FORMAT_INDEX) + (MAX_FORMAT_INDEX/2) + 3  , 1) * 256;    
}    
