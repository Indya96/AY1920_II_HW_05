/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
* \author Gabriele Belotti
* \date , 2020
*/

// Include required header files
#include "I2C_Interface.h"
#include "InterruptRoutines.h"
#include "project.h"
#include "stdio.h"


/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18


/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

/**
*   \X, Y and Z axis new data available
*/

#define LIS3DH_STATUS_REG_DATA_AVAILABLE 0x04 

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

/**
*   \brief Hex value to set the accelerometer to output 3-axis
     acceleration data in normal mode at 100Hz 
*/
#define LIS3DH_CTRL_REG1_SET 0x57



/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23


/**
*   \brief Hex value to set active the BDU, enable the high resolution 
    and set the range to +-4G
*/

#define LIS3DH_CTRL_REG4_SET 0x98 
   

/**
*   \brief Address of the X-Axis output LSB register
*/
#define LIS3DH_OUT_X_L 0x28



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    

    ErrorCode error; 
    
    
   
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_CTRL_REG1_SET)
    {
        ctrl_reg1 = LIS3DH_CTRL_REG1_SET;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    
    /******************************************/
    /*     Read Control Register 4            */
    /******************************************/
    
    
    
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_REG4_SET; // must be changed to the appropriate value
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    int16 OutX;
    int16 OutY;
    int16 OutZ;
    
    
    float32 OutX_converted;
    float32 OutY_converted;
    float32 OutZ_converted;
    
    int32_t OutX_rounded;
    int32_t OutY_rounded;
    int32_t OutZ_rounded;

    
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[14]; 
    uint8_t AccelerationData[6];
    
    OutArray[0] = header;
    OutArray[13] = footer;
    
    
    timer_flag=0;
    
    isr_StartEx(custom_TIMER_ISR);
    
    
    
    while(!(LIS3DH_STATUS_REG & LIS3DH_STATUS_REG_DATA_AVAILABLE)){}
    Timer_WriteCounter(5);
    Timer_Start();
    
    
    for(;;)
    {
        if(timer_flag==1)  //every 10 ms
        {
            
            //wait until X, Y and Z axis new data are available
            
            while(!(LIS3DH_STATUS_REG & LIS3DH_STATUS_REG_DATA_AVAILABLE)){}
        
            timer_flag=0;
            
            //Multiread to read all the output registers
            
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_X_L,6,
                                            &AccelerationData[0]);
        
        
            if(error == NO_ERROR)
            {
                //cast to int16: I obtain the value in mG
                OutX = (int16)((AccelerationData[0] | (AccelerationData[1]<<8)))>>4;
                
                
                //the range is between -4G and +4G so the sensitivity is 2mG/digit
                //to obtain the value in mm/s^2 I multiply for the sensitivity and also for 9.81
                
                OutX_converted= ((float32)OutX)*2*9.81; //value in mm/s^2 
                
                OutX_rounded= (int32)(OutX_converted +0.5);  //value in mm/s^2 rounded
                                              
                
                OutArray[1] = (uint8_t)(OutX_rounded & 0xFF); //LSB
                OutArray[2] = (uint8_t)(OutX_rounded >> 8);   
                OutArray[3] = (uint8_t)(OutX_rounded >> 16);
                OutArray[4] = (uint8_t)(OutX_rounded >> 24);  //MSB

                
                OutY = (int16)((AccelerationData[2] | (AccelerationData[3]<<8)))>>4;
                
                OutY_converted= (float32) ((float32)OutY) *2*9.81; 
                
                OutY_rounded= (int32)(OutY_converted + 0.5);
                                              
                
                OutArray[5] = (uint8_t)(OutY_rounded & 0xFF); 
                OutArray[6] = (uint8_t)(OutY_rounded >> 8);   
                OutArray[7] = (uint8_t)(OutY_rounded >> 16);
                OutArray[8] = (uint8_t)(OutY_rounded >> 24);  
                
                OutZ = (int16)((AccelerationData[4] | (AccelerationData[5]<<8)))>>4;
                
                OutZ_converted= ((float32)OutZ)*2*9.81; 
                
                OutZ_rounded= (int32)(OutZ_converted + 0.5);
                                              
                
                OutArray[9] = (uint8_t)(OutZ_rounded & 0xFF); 
                OutArray[10] = (uint8_t)(OutZ_rounded >> 8);   
                OutArray[11] = (uint8_t)(OutZ_rounded >> 16);
                OutArray[12] = (uint8_t)(OutZ_rounded >> 24);  
                
                //I'm sending the values in mm/s^2 to keep the information of the first 3 decimals 
        
                UART_Debug_PutArray(OutArray, 14);
            }
        }
    }
}

/* [] END OF FILE */
