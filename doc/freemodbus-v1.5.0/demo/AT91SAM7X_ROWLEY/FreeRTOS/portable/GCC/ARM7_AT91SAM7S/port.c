/* 
 * MODBUS Library: SAM7X Port
 * Copyright (c) 2007 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * $Id: port.c,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */
/*
 * FreeRTOS.org V4.4.0 - Copyright (C) 2003-2007 Richard Barry.
 *
 * This file is part of the FreeRTOS.org distribution.
 *
 * FreeRTOS.org is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * FreeRTOS.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FreeRTOS.org; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * A special exception to the GPL can be applied should you wish to distribute
 * a combined work that includes FreeRTOS.org, without being obliged to provide
 * the source code for any proprietary components.  See the licensing section 
 * of http://www.FreeRTOS.org for full details of how and when the exception
 * can be applied.
 * 
 ****************************************************************************
 * See http://www.FreeRTOS.org for documentation, latest information, license 
 * and contact details.  Please ensure to read the configuration and relevant 
 * port sections of the online documentation.
 *
 * Also see http://www.SafeRTOS.com for an IEC 61508 compliant version along
 * with commercial development and support options.
 ***************************************************************************
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>

/* ----------------------- FreeRTOS includes --------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

/* ----------------------- Platform includes --------------------------------*/
#include "AT91SAM7X256.h"
#include "lib_AT91SAM7X256.h"

/* ----------------------- Defines ------------------------------------------*/
#define portINITIAL_SPSR                  ( ( portSTACK_TYPE ) 0x1f )
#define portTHUMB_MODE_BIT                ( ( portSTACK_TYPE ) 0x20 )
#define portINSTRUCTION_SIZE              ( ( portSTACK_TYPE ) 4 )
#define portNO_CRITICAL_SECTION_NESTING   ( ( portSTACK_TYPE ) 0 )

/*
#define portPRESCALE_VALUE                0x00
#define portINTERRUPT_ON_MATCH            ( ( unsigned portLONG ) 0x01 )
#define portRESET_COUNT_ON_MATCH          ( ( unsigned portLONG ) 0x02 )

#define portPIT_CLOCK_DIVISOR             ( ( unsigned portLONG ) 16 )
#define portPIT_COUNTER_VALUE             ( ( ( configCPU_CLOCK_HZ / portPIT_CLOCK_DIVISOR ) / 1000UL ) * portTICK_RATE_MS )

#define portINT_LEVEL_SENSITIVE           0
#define portPIT_ENABLE                    ( ( unsigned portSHORT ) 0x1 << 24 )
#define portPIT_INT_ENABLE                ( ( unsigned portSHORT ) 0x1 << 25 )
*/

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Function prototypes ------------------------------*/
extern void     vPortISRStartFirstTask( void );
extern void     vNonPreemptiveTick ( void );
extern void     vPreemptiveTick( void );

/* ----------------------- Static functions ---------------------------------*/
static void     prvSetupTimerInterrupt( void );

/* ----------------------- Start implementation -----------------------------*/

portSTACK_TYPE *
pxPortInitialiseStack( portSTACK_TYPE * pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
    portSTACK_TYPE *pxOriginalTOS;

    pxOriginalTOS = pxTopOfStack;
    /* Setup the initial stack of the task.  The stack is set exactly as 
     * expected by the portRESTORE_CONTEXT() macro. 
     */

    /* First on the stack is the return address - which in this case is the
     * start of the task.  The offset is added to make the return address appear
     * as it would within an IRQ ISR. 
     */
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode + portINSTRUCTION_SIZE;
    pxTopOfStack--;

    *pxTopOfStack = ( portSTACK_TYPE ) 0xaaaaaaaa;      /* R14 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxOriginalTOS;   /* Stack used when task starts goes in R13. */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x12121212;      /* R12 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x11111111;      /* R11 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x10101010;      /* R10 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x09090909;      /* R9 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x08080808;      /* R8 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x07070707;      /* R7 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x06060606;      /* R6 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x05050505;      /* R5 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x04040404;      /* R4 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x03030303;      /* R3 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x02020202;      /* R2 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x01010101;      /* R1 */
    pxTopOfStack--;

    /* When the task starts is will expect to find the function parameter in
     * R0.
     */
    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters;    /* R0 */
    pxTopOfStack--;

    /* The last thing onto the stack is the status register, which is set for
     * system mode, with interrupts enabled. 
     */
    *pxTopOfStack = ( portSTACK_TYPE ) portINITIAL_SPSR;

#ifdef THUMB_INTERWORK
    {
        /* We want the task to start in thumb mode. */
        *pxTopOfStack |= portTHUMB_MODE_BIT;
    }
#endif

    pxTopOfStack--;

    /* Some optimisation levels use the stack differently to others.  This 
     * means the interrupt flags cannot always be stored on the stack and will
     * instead be stored in a variable, which is then saved as part of the
     * tasks context.
     */
    *pxTopOfStack = portNO_CRITICAL_SECTION_NESTING;

    return pxTopOfStack;
}

portBASE_TYPE
xPortStartScheduler( void )
{
    /* Start the timer that generates the tick ISR.  Interrupts are disabled
     * here already. 
     */
    prvSetupTimerInterrupt(  );

    /* Start the first task. */
    vPortISRStartFirstTask(  );

    /* Should not get here! */
    return 0;
}

void
vPortEndScheduler( void )
{
}

/*
 * Setup the timer 0 to generate the tick interrupts at the required frequency.
 */
static void
prvSetupTimerInterrupt( void )
{
    /* Setup the AIC for PIT interrupts.  The interrupt routine chosen depends
     * on whether the preemptive or cooperative scheduler is being used. 
     */
#if configUSE_PREEMPTION == 0
    AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_SYS, AT91C_AIC_PRIOR_HIGHEST, 
                           AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                           ( void ( * )( void ) )vNonPreemptiveTick );
#else
    AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_SYS, AT91C_AIC_PRIOR_HIGHEST,
                           AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                           ( void ( * )( void ) )vPreemptiveTick );
    
#endif

    /* Configure the PIT period. */
    AT91F_PITInit( AT91C_BASE_PITC, portTICK_RATE_MS * 1000U, configCPU_CLOCK_HZ / 1000000U );
    AT91F_PITEnableInt( AT91C_BASE_PITC );
    AT91F_AIC_EnableIt( AT91C_BASE_AIC, AT91C_ID_SYS );
}
