
#ifndef _MOT_BACK_H    
#define _MOT_BACK_H

#include "definitions.h"
#include "motlib.h"

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

        ext volatile MOTOR_STRUCT_t backMotor; //!< Motor main structure variable declaration

     /// @}   moduleStructures  


     /**
     * \defgroup publicModuleApi Public API Module definition
     * 
     * This section describes the API of the module
     * 
     *  @{
     */
        ext void motorBackInit(void); //!< Module initialization function
        ext bool activateBackCollimation(unsigned short target);//!< Activates the collimation

        
        
    /// @}   publicModuleApi 


/** @}*/ // backMotorModule

#endif // _MOT_BACK_H