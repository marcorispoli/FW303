/*******************************************************************************
  PORT PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_port.h

  Summary:
    PORT PLIB Header File

  Description:
    This file provides an interface to control and interact with PORT-I/O
    Pin controller module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_PORT_H
#define PLIB_PORT_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/*** Macros for uC_TEST_LED pin ***/
#define uC_TEST_LED_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 2U))
#define uC_TEST_LED_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 2U))
#define uC_TEST_LED_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 2U))
#define uC_TEST_LED_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 2U))
#define uC_TEST_LED_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 2U))
#define uC_TEST_LED_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 2U)) & 0x01U)
#define uC_TEST_LED_PIN                  PORT_PIN_PA02

/*** Macros for uC_EXP_WIN pin ***/
#define uC_EXP_WIN_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 3U))
#define uC_EXP_WIN_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 3U))
#define uC_EXP_WIN_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 3U))
#define uC_EXP_WIN_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 3U))
#define uC_EXP_WIN_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 3U))
#define uC_EXP_WIN_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 3U)) & 0x01U)
#define uC_EXP_WIN_PIN                  PORT_PIN_PA03

/*** Macros for uC_OPTO_RIGHT pin ***/
#define uC_OPTO_RIGHT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 4U))
#define uC_OPTO_RIGHT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 4U))
#define uC_OPTO_RIGHT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 4U))
#define uC_OPTO_RIGHT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 4U))
#define uC_OPTO_RIGHT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 4U))
#define uC_OPTO_RIGHT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 4U)) & 0x01U)
#define uC_OPTO_RIGHT_PIN                  PORT_PIN_PB04

/*** Macros for uC_OPTO_TRAP pin ***/
#define uC_OPTO_TRAP_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 5U))
#define uC_OPTO_TRAP_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 5U))
#define uC_OPTO_TRAP_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 5U))
#define uC_OPTO_TRAP_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 5U))
#define uC_OPTO_TRAP_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 5U))
#define uC_OPTO_TRAP_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 5U)) & 0x01U)
#define uC_OPTO_TRAP_PIN                  PORT_PIN_PB05

/*** Macros for uC_LATCH_FRONT pin ***/
#define uC_LATCH_FRONT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 6U))
#define uC_LATCH_FRONT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 6U))
#define uC_LATCH_FRONT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 6U))
#define uC_LATCH_FRONT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 6U))
#define uC_LATCH_FRONT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 6U))
#define uC_LATCH_FRONT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 6U)) & 0x01U)
#define uC_LATCH_FRONT_PIN                  PORT_PIN_PB06

/*** Macros for uC_COMPONENT pin ***/
#define uC_COMPONENT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 7U)) & 0x01U)
#define uC_COMPONENT_PIN                  PORT_PIN_PB07

/*** Macros for T_STATOR pin ***/
#define T_STATOR_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 8U)) & 0x01U)
#define T_STATOR_PIN                  PORT_PIN_PB08

/*** Macros for T_BULB pin ***/
#define T_BULB_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 9U)) & 0x01U)
#define T_BULB_PIN                  PORT_PIN_PB09

/*** Macros for uC_IA pin ***/
#define uC_IA_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 4U))
#define uC_IA_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 4U))
#define uC_IA_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 4U))
#define uC_IA_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 4U))
#define uC_IA_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 4U))
#define uC_IA_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 4U)) & 0x01U)
#define uC_IA_PIN                  PORT_PIN_PA04

/*** Macros for uC_IB pin ***/
#define uC_IB_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 5U))
#define uC_IB_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 5U))
#define uC_IB_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 5U))
#define uC_IB_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 5U))
#define uC_IB_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 5U))
#define uC_IB_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 5U)) & 0x01U)
#define uC_IB_PIN                  PORT_PIN_PA05

/*** Macros for uC_MS1 pin ***/
#define uC_MS1_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 6U))
#define uC_MS1_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 6U))
#define uC_MS1_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 6U))
#define uC_MS1_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 6U))
#define uC_MS1_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 6U))
#define uC_MS1_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 6U)) & 0x01U)
#define uC_MS1_PIN                  PORT_PIN_PA06

/*** Macros for uC_MS2 pin ***/
#define uC_MS2_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 7U))
#define uC_MS2_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 7U))
#define uC_MS2_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 7U))
#define uC_MS2_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 7U))
#define uC_MS2_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 7U))
#define uC_MS2_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 7U)) & 0x01U)
#define uC_MS2_PIN                  PORT_PIN_PA07

/*** Macros for uC_DIR pin ***/
#define uC_DIR_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 8U))
#define uC_DIR_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 8U))
#define uC_DIR_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 8U))
#define uC_DIR_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 8U))
#define uC_DIR_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 8U))
#define uC_DIR_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 8U)) & 0x01U)
#define uC_DIR_PIN                  PORT_PIN_PA08

/*** Macros for uC_ENA pin ***/
#define uC_ENA_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 9U))
#define uC_ENA_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 9U))
#define uC_ENA_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 9U))
#define uC_ENA_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 9U))
#define uC_ENA_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 9U))
#define uC_ENA_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 9U)) & 0x01U)
#define uC_ENA_PIN                  PORT_PIN_PA09

/*** Macros for uC_RST pin ***/
#define uC_RST_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 10U))
#define uC_RST_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define uC_RST_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define uC_RST_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 10U))
#define uC_RST_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define uC_RST_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 10U)) & 0x01U)
#define uC_RST_PIN                  PORT_PIN_PA10

/*** Macros for uC_ENASTEP pin ***/
#define uC_ENASTEP_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 11U))
#define uC_ENASTEP_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 11U))
#define uC_ENASTEP_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 11U))
#define uC_ENASTEP_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 11U))
#define uC_ENASTEP_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 11U))
#define uC_ENASTEP_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 11U)) & 0x01U)
#define uC_ENASTEP_PIN                  PORT_PIN_PA11

/*** Macros for uC_LATCH_BACK pin ***/
#define uC_LATCH_BACK_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 10U))
#define uC_LATCH_BACK_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define uC_LATCH_BACK_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define uC_LATCH_BACK_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 10U))
#define uC_LATCH_BACK_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define uC_LATCH_BACK_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 10U)) & 0x01U)
#define uC_LATCH_BACK_PIN                  PORT_PIN_PB10

/*** Macros for uC_LED_ON pin ***/
#define uC_LED_ON_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 11U))
#define uC_LED_ON_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 11U))
#define uC_LED_ON_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 11U))
#define uC_LED_ON_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 11U))
#define uC_LED_ON_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 11U))
#define uC_LED_ON_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 11U)) & 0x01U)
#define uC_LED_ON_PIN                  PORT_PIN_PB11

/*** Macros for uC_STEP_TRAP pin ***/
#define uC_STEP_TRAP_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 12U))
#define uC_STEP_TRAP_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 12U))
#define uC_STEP_TRAP_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 12U))
#define uC_STEP_TRAP_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 12U))
#define uC_STEP_TRAP_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 12U))
#define uC_STEP_TRAP_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 12U)) & 0x01U)
#define uC_STEP_TRAP_PIN                  PORT_PIN_PB12

/*** Macros for uC_STEP_MIRROR pin ***/
#define uC_STEP_MIRROR_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 13U))
#define uC_STEP_MIRROR_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 13U))
#define uC_STEP_MIRROR_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 13U))
#define uC_STEP_MIRROR_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 13U))
#define uC_STEP_MIRROR_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 13U))
#define uC_STEP_MIRROR_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 13U)) & 0x01U)
#define uC_STEP_MIRROR_PIN                  PORT_PIN_PB13

/*** Macros for uC_STEP_FILTER pin ***/
#define uC_STEP_FILTER_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 14U))
#define uC_STEP_FILTER_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 14U))
#define uC_STEP_FILTER_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 14U))
#define uC_STEP_FILTER_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 14U))
#define uC_STEP_FILTER_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 14U))
#define uC_STEP_FILTER_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 14U)) & 0x01U)
#define uC_STEP_FILTER_PIN                  PORT_PIN_PB14

/*** Macros for uC_STEP_RIGHT pin ***/
#define uC_STEP_RIGHT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 15U))
#define uC_STEP_RIGHT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 15U))
#define uC_STEP_RIGHT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 15U))
#define uC_STEP_RIGHT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 15U))
#define uC_STEP_RIGHT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 15U))
#define uC_STEP_RIGHT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 15U)) & 0x01U)
#define uC_STEP_RIGHT_PIN                  PORT_PIN_PB15

/*** Macros for uC_STEP_LEFT pin ***/
#define uC_STEP_LEFT_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 12U))
#define uC_STEP_LEFT_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 12U))
#define uC_STEP_LEFT_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 12U))
#define uC_STEP_LEFT_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 12U))
#define uC_STEP_LEFT_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 12U))
#define uC_STEP_LEFT_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 12U)) & 0x01U)
#define uC_STEP_LEFT_PIN                  PORT_PIN_PA12

/*** Macros for uC_MOT_SLEEP pin ***/
#define uC_MOT_SLEEP_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 13U))
#define uC_MOT_SLEEP_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 13U))
#define uC_MOT_SLEEP_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 13U))
#define uC_MOT_SLEEP_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 13U))
#define uC_MOT_SLEEP_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 13U))
#define uC_MOT_SLEEP_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 13U)) & 0x01U)
#define uC_MOT_SLEEP_PIN                  PORT_PIN_PA13

/*** Macros for uC_LATCH_TRAP pin ***/
#define uC_LATCH_TRAP_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 14U))
#define uC_LATCH_TRAP_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 14U))
#define uC_LATCH_TRAP_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 14U))
#define uC_LATCH_TRAP_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 14U))
#define uC_LATCH_TRAP_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 14U))
#define uC_LATCH_TRAP_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 14U)) & 0x01U)
#define uC_LATCH_TRAP_PIN                  PORT_PIN_PA14

/*** Macros for uC_LATCH_MIRROR pin ***/
#define uC_LATCH_MIRROR_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 15U))
#define uC_LATCH_MIRROR_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 15U))
#define uC_LATCH_MIRROR_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 15U))
#define uC_LATCH_MIRROR_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 15U))
#define uC_LATCH_MIRROR_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 15U))
#define uC_LATCH_MIRROR_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 15U)) & 0x01U)
#define uC_LATCH_MIRROR_PIN                  PORT_PIN_PA15

/*** Macros for uC_LATCH_FILTER pin ***/
#define uC_LATCH_FILTER_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 16U))
#define uC_LATCH_FILTER_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 16U))
#define uC_LATCH_FILTER_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 16U))
#define uC_LATCH_FILTER_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 16U))
#define uC_LATCH_FILTER_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 16U))
#define uC_LATCH_FILTER_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 16U)) & 0x01U)
#define uC_LATCH_FILTER_PIN                  PORT_PIN_PA16

/*** Macros for uC_LATCH_RIGHT pin ***/
#define uC_LATCH_RIGHT_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 17U))
#define uC_LATCH_RIGHT_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 17U))
#define uC_LATCH_RIGHT_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 17U))
#define uC_LATCH_RIGHT_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 17U))
#define uC_LATCH_RIGHT_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 17U))
#define uC_LATCH_RIGHT_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 17U)) & 0x01U)
#define uC_LATCH_RIGHT_PIN                  PORT_PIN_PA17

/*** Macros for uC_LATCH_LEFT pin ***/
#define uC_LATCH_LEFT_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 18U))
#define uC_LATCH_LEFT_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 18U))
#define uC_LATCH_LEFT_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 18U))
#define uC_LATCH_LEFT_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 18U))
#define uC_LATCH_LEFT_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 18U))
#define uC_LATCH_LEFT_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 18U)) & 0x01U)
#define uC_LATCH_LEFT_PIN                  PORT_PIN_PA18

/*** Macros for uC_LATCH_CLR pin ***/
#define uC_LATCH_CLR_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 19U))
#define uC_LATCH_CLR_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 19U))
#define uC_LATCH_CLR_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 19U))
#define uC_LATCH_CLR_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 19U))
#define uC_LATCH_CLR_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 19U))
#define uC_LATCH_CLR_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 19U)) & 0x01U)
#define uC_LATCH_CLR_PIN                  PORT_PIN_PA19

/*** Macros for uC_OPTO_FRONT pin ***/
#define uC_OPTO_FRONT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 16U))
#define uC_OPTO_FRONT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 16U))
#define uC_OPTO_FRONT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 16U))
#define uC_OPTO_FRONT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 16U))
#define uC_OPTO_FRONT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 16U))
#define uC_OPTO_FRONT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 16U)) & 0x01U)
#define uC_OPTO_FRONT_PIN                  PORT_PIN_PB16

/*** Macros for uC_OPTO_BACK pin ***/
#define uC_OPTO_BACK_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 17U))
#define uC_OPTO_BACK_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 17U))
#define uC_OPTO_BACK_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 17U))
#define uC_OPTO_BACK_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 17U))
#define uC_OPTO_BACK_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 17U))
#define uC_OPTO_BACK_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 17U)) & 0x01U)
#define uC_OPTO_BACK_PIN                  PORT_PIN_PB17

/*** Macros for uC_SW1 pin ***/
#define uC_SW1_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 20U))
#define uC_SW1_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 20U))
#define uC_SW1_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 20U))
#define uC_SW1_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 20U))
#define uC_SW1_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 20U))
#define uC_SW1_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 20U)) & 0x01U)
#define uC_SW1_PIN                  PORT_PIN_PA20

/*** Macros for uC_XRAY_ON pin ***/
#define uC_XRAY_ON_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 21U))
#define uC_XRAY_ON_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 21U))
#define uC_XRAY_ON_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 21U))
#define uC_XRAY_ON_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 21U))
#define uC_XRAY_ON_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 21U))
#define uC_XRAY_ON_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 21U)) & 0x01U)
#define uC_XRAY_ON_PIN                  PORT_PIN_PA21

/*** Macros for CAN0_TX pin ***/
#define CAN0_TX_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 22U)) & 0x01U)
#define CAN0_TX_PIN                  PORT_PIN_PA22

/*** Macros for CAN0_RX pin ***/
#define CAN0_RX_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 23U)) & 0x01U)
#define CAN0_RX_PIN                  PORT_PIN_PA23

/*** Macros for uC_SW2 pin ***/
#define uC_SW2_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 24U))
#define uC_SW2_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 24U))
#define uC_SW2_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 24U))
#define uC_SW2_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 24U))
#define uC_SW2_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 24U))
#define uC_SW2_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 24U)) & 0x01U)
#define uC_SW2_PIN                  PORT_PIN_PA24

/*** Macros for uC_SW3 pin ***/
#define uC_SW3_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 25U))
#define uC_SW3_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 25U))
#define uC_SW3_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 25U))
#define uC_SW3_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 25U))
#define uC_SW3_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 25U))
#define uC_SW3_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 25U)) & 0x01U)
#define uC_SW3_PIN                  PORT_PIN_PA25

/*** Macros for uC_OPTO_MIRROR pin ***/
#define uC_OPTO_MIRROR_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 22U))
#define uC_OPTO_MIRROR_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 22U))
#define uC_OPTO_MIRROR_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 22U))
#define uC_OPTO_MIRROR_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 22U))
#define uC_OPTO_MIRROR_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 22U))
#define uC_OPTO_MIRROR_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 22U)) & 0x01U)
#define uC_OPTO_MIRROR_PIN                  PORT_PIN_PB22

/*** Macros for uC_TEST_PUSH pin ***/
#define uC_TEST_PUSH_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 23U))
#define uC_TEST_PUSH_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 23U))
#define uC_TEST_PUSH_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 23U))
#define uC_TEST_PUSH_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 23U))
#define uC_TEST_PUSH_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 23U))
#define uC_TEST_PUSH_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 23U)) & 0x01U)
#define uC_TEST_PUSH_PIN                  PORT_PIN_PB23

/*** Macros for FAN pin ***/
#define FAN_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 27U))
#define FAN_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 27U))
#define FAN_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 27U))
#define FAN_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 27U))
#define FAN_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 27U))
#define FAN_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 27U)) & 0x01U)
#define FAN_PIN                  PORT_PIN_PA27

/*** Macros for VITALITY_LED pin ***/
#define VITALITY_LED_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 31U))
#define VITALITY_LED_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 31U))
#define VITALITY_LED_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 31U))
#define VITALITY_LED_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 31U))
#define VITALITY_LED_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 31U))
#define VITALITY_LED_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 31U)) & 0x01U)
#define VITALITY_LED_PIN                  PORT_PIN_PB31

/*** Macros for uC_STEP_FRONT pin ***/
#define uC_STEP_FRONT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 0U))
#define uC_STEP_FRONT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 0U))
#define uC_STEP_FRONT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 0U))
#define uC_STEP_FRONT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 0U))
#define uC_STEP_FRONT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 0U))
#define uC_STEP_FRONT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 0U)) & 0x01U)
#define uC_STEP_FRONT_PIN                  PORT_PIN_PB00

/*** Macros for uC_OPTO_LEFT pin ***/
#define uC_OPTO_LEFT_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 1U))
#define uC_OPTO_LEFT_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 1U))
#define uC_OPTO_LEFT_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 1U))
#define uC_OPTO_LEFT_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 1U))
#define uC_OPTO_LEFT_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 1U))
#define uC_OPTO_LEFT_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 1U)) & 0x01U)
#define uC_OPTO_LEFT_PIN                  PORT_PIN_PB01

/*** Macros for uC_OPTO_FILTER pin ***/
#define uC_OPTO_FILTER_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 2U))
#define uC_OPTO_FILTER_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 2U))
#define uC_OPTO_FILTER_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 2U))
#define uC_OPTO_FILTER_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 2U))
#define uC_OPTO_FILTER_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 2U))
#define uC_OPTO_FILTER_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 2U)) & 0x01U)
#define uC_OPTO_FILTER_PIN                  PORT_PIN_PB02

/*** Macros for uC_STEP_BACK pin ***/
#define uC_STEP_BACK_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 3U))
#define uC_STEP_BACK_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 3U))
#define uC_STEP_BACK_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 3U))
#define uC_STEP_BACK_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 3U))
#define uC_STEP_BACK_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 3U))
#define uC_STEP_BACK_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 3U)) & 0x01U)
#define uC_STEP_BACK_PIN                  PORT_PIN_PB03

// *****************************************************************************
/* PORT Group

  Summary:
    Identifies the port groups available on the device.

  Description:
    These macros identifies all the ports groups that are available on this
    device.

  Remarks:
    The caller should not use the constant expressions assigned to any of
    the preprocessor macros as these may vary between devices.

    Port groups shown here are the ones available on the selected device. Not
    all ports groups are implemented. Refer to the device specific datasheet
    for more details. The MHC will generate these macros with the port
    groups that are available on the device.
*/

/* Group 0 */
#define PORT_GROUP_0 (PORT_BASE_ADDRESS + (0U * 0x80U))

/* Group 1 */
#define PORT_GROUP_1 (PORT_BASE_ADDRESS + (1U * 0x80U))


/* Helper macros to get port information from the pin */
#define GET_PORT_GROUP(pin)  ((PORT_GROUP)(PORT_BASE_ADDRESS + (0x80U * (((uint32_t)pin) >> 5U))))
#define GET_PIN_MASK(pin)   (((uint32_t)(0x1U)) << (((uint32_t)pin) & 0x1FU))

/* Named type for port group */ 
typedef uint32_t PORT_GROUP;


typedef enum
{
PERIPHERAL_FUNCTION_A = 0x0,
PERIPHERAL_FUNCTION_B = 0x1,
PERIPHERAL_FUNCTION_C = 0x2,
PERIPHERAL_FUNCTION_D = 0x3,
PERIPHERAL_FUNCTION_E = 0x4,
PERIPHERAL_FUNCTION_F = 0x5,
PERIPHERAL_FUNCTION_G = 0x6,
PERIPHERAL_FUNCTION_H = 0x7,
PERIPHERAL_FUNCTION_I = 0x8,
PERIPHERAL_FUNCTION_J = 0x9,
PERIPHERAL_FUNCTION_K = 0xA,
PERIPHERAL_FUNCTION_L = 0xB,
PERIPHERAL_FUNCTION_M = 0xC,
PERIPHERAL_FUNCTION_N = 0xD,

}PERIPHERAL_FUNCTION;

// *****************************************************************************
/* PORT Pins

  Summary:
    Identifies the available Ports pins.

  Description:
    This enumeration identifies all the ports pins that are available on this
    device.

  Remarks:
    The caller should not use the constant expressions assigned to any of
    the enumeration constants as these may vary between devices.

    Port pins shown here are the ones available on the selected device. Not
    all ports pins within a port group are implemented. Refer to the device
    specific datasheet for more details.
*/

typedef enum
{
    /* PA00 pin */
    PORT_PIN_PA00 = 0U,

    /* PA01 pin */
    PORT_PIN_PA01 = 1U,

    /* PA02 pin */
    PORT_PIN_PA02 = 2U,

    /* PA03 pin */
    PORT_PIN_PA03 = 3U,

    /* PA04 pin */
    PORT_PIN_PA04 = 4U,

    /* PA05 pin */
    PORT_PIN_PA05 = 5U,

    /* PA06 pin */
    PORT_PIN_PA06 = 6U,

    /* PA07 pin */
    PORT_PIN_PA07 = 7U,

    /* PA08 pin */
    PORT_PIN_PA08 = 8U,

    /* PA09 pin */
    PORT_PIN_PA09 = 9U,

    /* PA10 pin */
    PORT_PIN_PA10 = 10U,

    /* PA11 pin */
    PORT_PIN_PA11 = 11U,

    /* PA12 pin */
    PORT_PIN_PA12 = 12U,

    /* PA13 pin */
    PORT_PIN_PA13 = 13U,

    /* PA14 pin */
    PORT_PIN_PA14 = 14U,

    /* PA15 pin */
    PORT_PIN_PA15 = 15U,

    /* PA16 pin */
    PORT_PIN_PA16 = 16U,

    /* PA17 pin */
    PORT_PIN_PA17 = 17U,

    /* PA18 pin */
    PORT_PIN_PA18 = 18U,

    /* PA19 pin */
    PORT_PIN_PA19 = 19U,

    /* PA20 pin */
    PORT_PIN_PA20 = 20U,

    /* PA21 pin */
    PORT_PIN_PA21 = 21U,

    /* PA22 pin */
    PORT_PIN_PA22 = 22U,

    /* PA23 pin */
    PORT_PIN_PA23 = 23U,

    /* PA24 pin */
    PORT_PIN_PA24 = 24U,

    /* PA25 pin */
    PORT_PIN_PA25 = 25U,

    /* PA27 pin */
    PORT_PIN_PA27 = 27U,

    /* PA30 pin */
    PORT_PIN_PA30 = 30U,

    /* PA31 pin */
    PORT_PIN_PA31 = 31U,

    /* PB00 pin */
    PORT_PIN_PB00 = 32U,

    /* PB01 pin */
    PORT_PIN_PB01 = 33U,

    /* PB02 pin */
    PORT_PIN_PB02 = 34U,

    /* PB03 pin */
    PORT_PIN_PB03 = 35U,

    /* PB04 pin */
    PORT_PIN_PB04 = 36U,

    /* PB05 pin */
    PORT_PIN_PB05 = 37U,

    /* PB06 pin */
    PORT_PIN_PB06 = 38U,

    /* PB07 pin */
    PORT_PIN_PB07 = 39U,

    /* PB08 pin */
    PORT_PIN_PB08 = 40U,

    /* PB09 pin */
    PORT_PIN_PB09 = 41U,

    /* PB10 pin */
    PORT_PIN_PB10 = 42U,

    /* PB11 pin */
    PORT_PIN_PB11 = 43U,

    /* PB12 pin */
    PORT_PIN_PB12 = 44U,

    /* PB13 pin */
    PORT_PIN_PB13 = 45U,

    /* PB14 pin */
    PORT_PIN_PB14 = 46U,

    /* PB15 pin */
    PORT_PIN_PB15 = 47U,

    /* PB16 pin */
    PORT_PIN_PB16 = 48U,

    /* PB17 pin */
    PORT_PIN_PB17 = 49U,

    /* PB22 pin */
    PORT_PIN_PB22 = 54U,

    /* PB23 pin */
    PORT_PIN_PB23 = 55U,

    /* PB30 pin */
    PORT_PIN_PB30 = 62U,

    /* PB31 pin */
    PORT_PIN_PB31 = 63U,

    /* This element should not be used in any of the PORT APIs.
     * It will be used by other modules or application to denote that none of
     * the PORT Pin is used */
    PORT_PIN_NONE = 65535U,

} PORT_PIN;

// *****************************************************************************
// *****************************************************************************
// Section: Generated API based on pin configurations done in Pin Manager
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PORT_Initialize(void)

  Summary:
    Initializes the PORT Library.

  Description:
    This function initializes all ports and pins as configured in the
    MHC Pin Manager.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>

    PORT_Initialize();

    </code>

  Remarks:
    The function should be called once before calling any other PORTS PLIB
    functions.
*/

void PORT_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: PORT APIs which operates on multiple pins of a group
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t PORT_GroupRead(PORT_GROUP group)

  Summary:
    Read all the I/O pins in the specified port group.

  Description:
    The function reads the hardware pin state of all pins in the specified group
    and returns this as a 32 bit value. Each bit in the 32 bit value represent a
    pin. For example, bit 0 in group 0 will represent pin PA0. Bit 1 will
    represent PA1 and so on. The application should only consider the value of
    the port group pins which are implemented on the device.

  Precondition:
    The PORT_Initialize() function should have been called. Input buffer
    (INEN bit in the Pin Configuration register) should be enabled in MHC.

  Parameters:
    group - One of the IO groups from the enum PORT_GROUP.

  Returns:
    A 32-bit value representing the hardware state of of all the I/O pins in the
    selected port group.

  Example:
    <code>

    uint32_t value;
    value = PORT_Read(PORT_GROUP_C);

    </code>

  Remarks:
    None.
*/

uint32_t PORT_GroupRead(PORT_GROUP group);

// *****************************************************************************
/* Function:
    uint32_t PORT_GroupLatchRead(PORT_GROUP group)

  Summary:
    Read the data driven on all the I/O pins of the selected port group.

  Description:
    The function will return a 32-bit value representing the logic levels being
    driven on the output pins within the group. The function will not sample the
    actual hardware state of the output pin. Each bit in the 32-bit return value
    will represent one of the 32 port pins within the group. The application
    should only consider the value of the pins which are available on the
    device.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One of the IO groups from the enum PORT_GROUP.

  Returns:
    A 32-bit value representing the output state of of all the I/O pins in the
    selected port group.

  Example:
    <code>

    uint32_t value;
    value = PORT_GroupLatchRead(PORT_GROUP_C);

    </code>

  Remarks:
    None.
*/

uint32_t PORT_GroupLatchRead(PORT_GROUP group);

// *****************************************************************************
/* Function:
    void PORT_GroupWrite(PORT_GROUP group, uint32_t mask, uint32_t value);

  Summary:
    Write value on the masked pins of the selected port group.

  Description:
    This function writes the value contained in the value parameter to the
    port group. Port group pins which are configured for output will be updated.
    The mask parameter provides additional control on the bits in the group to
    be affected. Setting a bit to 1 in the mask will cause the corresponding
    bit in the port group to be updated. Clearing a bit in the mask will cause
    that corresponding bit in the group to stay unaffected. For example,
    setting a mask value 0xFFFFFFFF will cause all bits in the port group
    to be updated. Setting a value 0x3 will only cause port group bit 0 and
    bit 1 to be updated.

    For port pins which are not configured for output and have the pull feature
    enabled, this function will affect pull value (pull up or pull down). A bit
    value of 1 will enable the pull up. A bit value of 0 will enable the pull
    down.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One of the IO groups from the enum PORT_GROUP.

    mask  - A 32 bit value in which positions of 0s and 1s decide
             which IO pins of the selected port group will be written.
             1's - Will write to corresponding IO pins.
             0's - Will remain unchanged.

    value - Value which has to be written/driven on the I/O
             lines of the selected port for which mask bits are '1'.
             Values for the corresponding mask bit '0' will be ignored.
             Refer to the function description for effect on pins
             which are not configured for output.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupWrite(PORT_GROUP_C, 0x0F, 0xF563D453);

    </code>

  Remarks:
    None.
*/

void PORT_GroupWrite(PORT_GROUP group, uint32_t mask, uint32_t value);

// *****************************************************************************
/* Function:
    void PORT_GroupSet(PORT_GROUP group, uint32_t mask)

  Summary:
    Set the selected IO pins of a group.

  Description:
    This function sets (drives a logic high) on the selected output pins of a
    group. The mask parameter control the pins to be updated. A mask bit
    position with a value 1 will cause that corresponding port pin to be set. A
    mask bit position with a value 0 will cause the corresponding port pin to
    stay un-affected.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One of the IO ports from the enum PORT_GROUP.
    mask - A 32 bit value in which a bit represent a pin in the group. If the
    value of the bit is 1, the corresponding port pin will driven to logic 1. If
    the value of the bit is 0. the corresponding port pin will stay un-affected.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupSet(PORT_GROUP_C, 0x00A0);

    </code>

  Remarks:
    If the port pin within the the group is not configured for output and has
    the pull feature enabled, driving a logic 1 on this pin will cause the pull
    up to be enabled.
*/

void PORT_GroupSet(PORT_GROUP group, uint32_t mask);

// *****************************************************************************
/* Function:
    void PORT_GroupClear(PORT_GROUP group, uint32_t mask)

  Summary:
    Clears the selected IO pins of a group.

  Description:
    This function clears (drives a logic 0) on the selected output pins of a
    group. The mask parameter control the pins to be updated. A mask bit
    position with a value 1 will cause that corresponding port pin to be clear.
    A mask bit position with a value 0 will cause the corresponding port pin to
    stay un-affected.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One of the IO ports from the enum PORT_GROUP.
    mask - A 32 bit value in which a bit represent a pin in the group. If the
    value of the bit is 1, the corresponding port pin will driven to logic 0. If
    the value of the bit is 0. the corresponding port pin will stay un-affected.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupClear(PORT_GROUP_C, 0x00A0);

    </code>

  Remarks:
    If the port pin within the the group is not configured for output and has
    the pull feature enabled, driving a logic 0 on this pin will cause the pull
    down to be enabled.
*/

void PORT_GroupClear(PORT_GROUP group, uint32_t mask);

// *****************************************************************************
/* Function:
    void PORT_GroupToggle(PORT_GROUP group, uint32_t mask)

  Summary:
    Toggles the selected IO pins of a group.

  Description:
    This function toggles the selected output pins of a group. The mask
    parameter control the pins to be updated. A mask bit position with a value 1
    will cause that corresponding port pin to be toggled.  A mask bit position
    with a value 0 will cause the corresponding port pin to stay un-affected.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One of the IO ports from the enum PORT_GROUP.
    mask - A 32 bit value in which a bit represent a pin in the group. If the
    value of the bit is 1, the corresponding port pin will be toggled. If the
    value of the bit is 0. the corresponding port pin will stay un-affected.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupToggle(PORT_GROUP_C, 0x00A0);

    </code>

  Remarks:
    If the port pin within the the group is not configured for output and has
    the pull feature enabled, driving a logic 0 on this pin will cause the pull
    down to be enabled. Driving a logic 1 on this pin will cause the pull up to
    be enabled.
*/

void PORT_GroupToggle(PORT_GROUP group, uint32_t mask);

// *****************************************************************************
/* Function:
    void PORT_GroupInputEnable(PORT_GROUP group, uint32_t mask)

  Summary:
    Configures the selected IO pins of a group as input.

  Description:
    This function configures the selected IO pins of a group as input. The pins
    to be configured as input are selected by setting the corresponding bits in
    the mask parameter to 1.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One or more of the of the IO ports from the enum PORT_GROUP.
    mask - A 32 bit value in which a bit represents a pin in the group. If the
    value of the bit is 1, the corresponding port pin will be configured as
    input. If the value of the bit is 0. the corresponding port pin will stay
    un-affected.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupInputEnable(PORT_GROUP_C, 0x00A0);

    </code>

  Remarks:
   None.
*/

void PORT_GroupInputEnable(PORT_GROUP group, uint32_t mask);

// *****************************************************************************
/* Function:
    void PORT_GroupOutputEnable(PORT_GROUP group, uint32_t mask)

  Summary:
    Configures the selected IO pins of a group as output.

  Description:
    This function configures the selected IO pins of a group as output. The pins
    to be configured as output are selected by setting the corresponding bits in
    the mask parameter to 1.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    group - One or more of the of the IO ports from the enum PORT_GROUP.
    mask - A 32 bit value in which a bit represents a pin in the group. If the
    value of the bit is 1, the corresponding port pin will be configured as
    output. If the value of the bit is 0. the corresponding port pin will stay
    un-affected.

  Returns:
    None.

  Example:
    <code>

    PORT_GroupOutputEnable(PORT_GROUP_C, 0x00A0);

    </code>

  Remarks:
    None.
*/

void PORT_GroupOutputEnable(PORT_GROUP group, uint32_t mask);

// *****************************************************************************
/* Function:
    void PORT_PinPeripheralFunctionConfig(PORT_PIN pin, PERIPHERAL_FUNCTION function)

  Summary:
    Configures the peripheral function on the selected port pin

  Description:
    This function configures the selected peripheral function on the given port pin.

  Remarks:
    None
*/
void PORT_PinPeripheralFunctionConfig(PORT_PIN pin, PERIPHERAL_FUNCTION function);

// *****************************************************************************
/* Function:
    void PORT_PinGPIOConfig(PORT_PIN pin)

  Summary:
    Configures the selected pin as GPIO

  Description:
    This function configures the given pin as GPIO.

  Remarks:
    None
*/
void PORT_PinGPIOConfig(PORT_PIN pin);

// *****************************************************************************
// *****************************************************************************
// Section: PORT APIs which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PORT_PinWrite(PORT_PIN pin, bool value)

  Summary:
    Writes the specified value to the selected pin.

  Description:
    This function writes/drives the "value" on the selected I/O line/pin.

  Precondition:
    The PORT_Initialize() function should have been called once.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.
    value - value to be written on the selected pin.
            true  = set pin to high (1).
            false = clear pin to low (0).

  Returns:
    None.

  Example:
    <code>

    bool value = true;
    PORT_PinWrite(PORT_PIN_PB3, value);

    </code>

  Remarks:
    Calling this function with an input pin with the pull-up/pull-down feature
    enabled will affect the pull-up/pull-down configuration. If the value is
    false, the pull-down will be enabled. If the value is true, the pull-up will
    be enabled.
*/

static inline void PORT_PinWrite(PORT_PIN pin, bool value)
{
    PORT_GroupWrite(GET_PORT_GROUP(pin),
                    GET_PIN_MASK(pin),
                    (value ? GET_PIN_MASK(pin) : 0U));
}


// *****************************************************************************
/* Function:
    bool PORT_PinRead(PORT_PIN pin)

  Summary:
    Read the selected pin value.

  Description:
    This function reads the present state at the selected input pin.  The
    function can also be called to read the value of an output pin if input
    sampling on the output pin is enabled in MHC. If input synchronization on
    the pin is disabled in MHC, the function will cause a 2 PORT Clock cycles
    delay. Enabling the synchronization eliminates the delay but will increase
    power consumption.

  Precondition:
    The PORT_Initialize() function should have been called. Input buffer
    (INEN bit in the Pin Configuration register) should be enabled in MHC.

  Parameters:
    pin - the port pin whose state needs to be read.

  Returns:
    true - the state at the pin is a logic high.
    false - the state at the pin is a logic low.

  Example:
    <code>

    bool value;
    value = PORT_PinRead(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline bool PORT_PinRead(PORT_PIN pin)
{
    return ((PORT_GroupRead(GET_PORT_GROUP(pin)) & GET_PIN_MASK(pin)) != 0U);
}


// *****************************************************************************
/* Function:
    bool PORT_PinLatchRead(PORT_PIN pin)

  Summary:
    Read the value driven on the selected pin.

  Description:
    This function reads the data driven on the selected I/O line/pin. The
    function does not sample the state of the hardware pin. It only returns the
    value that is written to output register. Refer to the PORT_PinRead()
    function if the state of the output pin needs to be read.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    true - the present value in the output latch is a logic high.
    false - the present value in the output latch is a logic low.

  Example:
    <code>

    bool value;
    value = PORT_PinLatchRead(PORT_PIN_PB3);

    </code>

  Remarks:
    To read actual pin value, PIN_Read API should be used.
*/

static inline bool PORT_PinLatchRead(PORT_PIN pin)
{
    return ((PORT_GroupLatchRead(GET_PORT_GROUP(pin)) & GET_PIN_MASK(pin)) != 0U);
}


// *****************************************************************************
/* Function:
    void PORT_PinToggle(PORT_PIN pin)

  Summary:
    Toggles the selected pin.

  Description:
    This function toggles/inverts the present value on the selected I/O line/pin.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    None.

  Example:
    <code>

    PORT_PinToggle(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline void PORT_PinToggle(PORT_PIN pin)
{
    PORT_GroupToggle(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}


// *****************************************************************************
/* Function:
    void PORT_PinSet(PORT_PIN pin)

  Summary:
    Sets the selected pin.

  Description:
    This function drives a logic 1 on the selected I/O line/pin.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    None.

  Example:
    <code>

    PORT_PinSet(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline void PORT_PinSet(PORT_PIN pin)
{
    PORT_GroupSet(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}


// *****************************************************************************
/* Function:
    void PORT_PinClear(PORT_PIN pin)

  Summary:
    Clears the selected pin.

  Description:
    This function drives a logic 0 on the selected I/O line/pin.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    None.

  Example:
    <code>

    PORT_PinClear(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline void PORT_PinClear(PORT_PIN pin)
{
    PORT_GroupClear(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}


// *****************************************************************************
/* Function:
    void PORT_PinInputEnable(PORT_PIN pin)

  Summary:
    Configures the selected IO pin as input.

  Description:
    This function configures the selected IO pin as input. This function
    override the MHC input output pin settings.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    None.

  Example:
    <code>

    PORT_PinInputEnable(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline void PORT_PinInputEnable(PORT_PIN pin)
{
    PORT_GroupInputEnable(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}


// *****************************************************************************
/* Function:
    void PORT_PinOutputEnable(PORT_PIN pin)

  Summary:
    Enables selected IO pin as output.

  Description:
    This function enables selected IO pin as output. Calling this function will
    override the MHC input output pin configuration.

  Precondition:
    The PORT_Initialize() function should have been called.

  Parameters:
    pin - One of the IO pins from the enum PORT_PIN.

  Returns:
    None.

  Example:
    <code>

    PORT_PinOutputEnable(PORT_PIN_PB3);

    </code>

  Remarks:
    None.
*/

static inline void PORT_PinOutputEnable(PORT_PIN pin)
{
    PORT_GroupOutputEnable(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif
// DOM-IGNORE-END
#endif // PLIB_PORT_H
