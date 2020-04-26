/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */


#include "InterruptRoutines.h"
#include "project.h"

CY_ISR(custom_TIMER_ISR)
{
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    
    // set the flag to one 
    timer_flag = 1;
    
}