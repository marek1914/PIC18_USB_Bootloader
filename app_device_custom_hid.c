/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_hid.h"
#include <string.h>
#include "system.h"

#include "os.h"
#include "ui.h"
#include "i2c.h"
#include "rtcc.h"
#include "display.h"


/** VARIABLES ******************************************************/
unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];

volatile USB_HANDLE USBOutHandle;    
volatile USB_HANDLE USBInHandle;

extern calibration_t calibrationParameters[7];

/** DEFINITIONS ****************************************************/
typedef enum
{
    //These are commands from the Microchip USB HID demo, leave them in place for compatibility
    COMMAND_TOGGLE_LED = 0x80,
    COMMAND_GET_BUTTON_STATUS = 0x81,
    COMMAND_READ_POTENTIOMETER = 0x37,
    //These commands are specific to this application
    COMMAND_GET_STATUS = 0x10,
    COMMAND_GET_DISPLAY_1 = 0x11,
    COMMAND_GET_DISPLAY_2 = 0x12,
    COMMAND_GET_CALIBRATION_1 = 0x13,
    COMMAND_GET_CALIBRATION_2 = 0x14
} CUSTOM_HID_DEMO_COMMANDS;

/** FUNCTIONS ******************************************************/
static void _fill_buffer_get_status(void);
static void _fill_buffer_get_display1(void);
static void _fill_buffer_get_display2(void);
static void _fill_buffer_get_calibration1(void);
static void _fill_buffer_get_calibration2(void);
static void _parse_command_short(uint8_t cmd);
static void _parse_command_long(uint8_t cmd, uint8_t data);
static void _parse_command_calibration(uint8_t cmd, uint8_t item, uint8_t dat1, uint8_t dat2, uint8_t dat3);


/*********************************************************************
* Function: void APP_DeviceCustomHIDInitialize(void);
*
* Overview: Initializes the Custom HID demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCustomHIDInitialize()
{
    //initialize the variable holding the handle for the last
    // transmission
    USBInHandle = 0;

    //enable the HID endpoint
    USBEnableEndpoint(CUSTOM_DEVICE_HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = (volatile USB_HANDLE)HIDRxPacket(CUSTOM_DEVICE_HID_EP,(uint8_t*)&ReceivedDataBuffer,64);
}

/*********************************************************************
* Function: void APP_DeviceCustomHIDTasks(void);
*
* Overview: Keeps the Custom HID demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCustomHIDInitialize() and APP_DeviceCustomHIDStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCustomHIDTasks()
{   
    uint8_t idx;
    
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
    
    //Check if we have received an OUT data packet from the host
    if(HIDRxHandleBusy(USBOutHandle) == false)
    {   
        //We just received a packet of data from the USB host.
        //Check the first uint8_t of the packet to see what command the host
        //application software wants us to fulfill.
        //Look at the data the host sent, to see what kind of application specific command it sent.
        switch(ReceivedDataBuffer[0])				
        {
            case COMMAND_GET_STATUS:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Call function to fill the buffer with general information
                    _fill_buffer_get_status();
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;
                
            case COMMAND_GET_DISPLAY_1:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Call function to fill the buffer with general information
                    _fill_buffer_get_display1();
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;
                
            case COMMAND_GET_DISPLAY_2:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Call function to fill the buffer with general information
                    _fill_buffer_get_display2();
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;
                
            case COMMAND_GET_CALIBRATION_1:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Call function to fill the buffer with general information
                    _fill_buffer_get_calibration1();
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;
                
            case COMMAND_GET_CALIBRATION_2:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Call function to fill the buffer with general information
                    _fill_buffer_get_calibration2();
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;
                
            case COMMAND_TOGGLE_LED:
                //We don't have an LED so we toggle the fan
                FANOUT_PIN ^= 1;
                break;
                
            case COMMAND_GET_BUTTON_STATUS:
                //Check to make sure the endpoint/buffer is free before we modify the contents
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    //Echo back to the host PC the command we are fulfilling in the first uint8_t
                    ToSendDataBuffer[0] = COMMAND_GET_BUTTON_STATUS;
                    if(PUSHBUTTON_BIT) 
                    {
                        //Pin is high so push button is not pressed
                        ToSendDataBuffer[1] = 0x01;
                    }
                    else									
                    {
                        //Pin is low so push button is pressed
                        ToSendDataBuffer[1] = 0x00;
                    }
                    //Prepare the USB module to send the data packet to the host
                    USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                }
                break;

            case COMMAND_READ_POTENTIOMETER:
                {
                    uint16_t return_value;
                    //Check to make sure the endpoint/buffer is free before we modify the contents
                    if(!HIDTxHandleBusy(USBInHandle))
                    {
                        //Echo back to the host PC the command we are fulfilling in the first uint8_t
                        ToSendDataBuffer[0] = COMMAND_READ_POTENTIOMETER;
                        //We don't have a pot so we echo back the on-board temperature (in 0.01 degrees centigrade) divided by 8
                        return_value = 0x1234;
                        ToSendDataBuffer[1] = (uint8_t) return_value; //LSB
                        ToSendDataBuffer[2] = return_value >> 8; //MSB
                        //Prepare the USB module to send the data packet to the host
                        USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                    }
                }
                break;
        }
        
        //Check if the host expects us to do anything else
        idx = 1;
        while(idx<62)
        {
            switch(ReceivedDataBuffer[idx] & 0xF0)
            {
                case 0x30:
                    _parse_command_short(ReceivedDataBuffer[idx]);
                    ++idx;
                    break;
                case 0x40:
                    _parse_command_long(ReceivedDataBuffer[idx], ReceivedDataBuffer[idx+1]);
                    idx += 2;
                    break;
                case 0x60:
                    _parse_command_calibration(ReceivedDataBuffer[idx], ReceivedDataBuffer[idx+1], ReceivedDataBuffer[idx+2], ReceivedDataBuffer[idx+3], ReceivedDataBuffer[idx+4]);
                    idx += 5;
                    break;
                default:
                    idx = 65; //exit loop
            }
        }

        //Re-arm the OUT endpoint, so we can receive the next OUT data packet 
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ReceivedDataBuffer, 64);
    }
}


//Fill buffer with general status information
static void _fill_buffer_get_status(void)
{
    //Echo back to the host PC the command we are fulfilling in the first uint8_t
    ToSendDataBuffer[0] = COMMAND_GET_STATUS;
    //Bytes 1-2: input voltage
    
    //Byte 16: Display mode
    ToSendDataBuffer[16] = os.display_mode;
    //Byte 17-22: Date and time, all in 2-digit DCB
    ToSendDataBuffer[17] = rtcc_get_year();
    ToSendDataBuffer[18] = rtcc_get_month();
    ToSendDataBuffer[19] = rtcc_get_day();
    ToSendDataBuffer[20] = rtcc_get_hours();
    ToSendDataBuffer[21] = rtcc_get_minutes();
    ToSendDataBuffer[22] = rtcc_get_seconds();
    //Charger details
   
    //Display status, display off, startup etc
    ToSendDataBuffer[41] = ui_get_status();
    ToSendDataBuffer[42] = os.timeSlot;
}

//Fill buffer with first half of display content
static void _fill_buffer_get_display1(void)
{
    uint8_t cntr;
    uint8_t line;
    uint8_t position;
    
   //Echo back to the host PC the command we are fulfilling in the first uint8_t
   ToSendDataBuffer[0] = COMMAND_GET_DISPLAY_1;
   
   //Get display data
   cntr = 1; 
   for(line=0; line<2; ++line)
   {
        for(position=0; position<20; ++position)
        {
            ToSendDataBuffer[cntr] = display_get_character(line, position);
            ++cntr;
        }
   }
}

//Fill buffer with second half of display content
static void _fill_buffer_get_display2(void)
{
    uint8_t cntr;
    uint8_t line;
    uint8_t position;
    
   //Echo back to the host PC the command we are fulfilling in the first uint8_t
   ToSendDataBuffer[0] = COMMAND_GET_DISPLAY_2;
   
   //Get display data
   cntr = 1; 
   for(line=2; line<4; ++line)
   {
        for(position=0; position<20; ++position)
        {
            ToSendDataBuffer[cntr] = display_get_character(line, position);
            ++cntr;
        }
   }
}


//Fill buffer with voltage and current measurement parameters
static void _fill_buffer_get_calibration1(void)
{
   //Echo back to the host PC the command we are fulfilling in the first uint8_t
   ToSendDataBuffer[0] = COMMAND_GET_CALIBRATION_1;
   //Copy calibration to buffer
   memcpy(&ToSendDataBuffer[1], &calibrationParameters[CALIBRATION_INDEX_INPUT_VOLTAGE], 12);
   memcpy(&ToSendDataBuffer[13], &calibrationParameters[CALIBRATION_INDEX_OUTPUT_VOLTAGE], 12);
   memcpy(&ToSendDataBuffer[25], &calibrationParameters[CALIBRATION_INDEX_INPUT_CURRENT], 12);
   memcpy(&ToSendDataBuffer[37], &calibrationParameters[CALIBRATION_INDEX_OUTPUT_CURRENT], 12);
}

//Fill buffer with temperature measurement parameters
static void _fill_buffer_get_calibration2(void)
{
   //Echo back to the host PC the command we are fulfilling in the first uint8_t
   ToSendDataBuffer[0] = COMMAND_GET_CALIBRATION_2;
   //Copy calibration to buffer
   memcpy(&ToSendDataBuffer[1], &calibrationParameters[CALIBRATION_INDEX_ONBOARD_TEMPERATURE], 12);
   memcpy(&ToSendDataBuffer[13], &calibrationParameters[CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_1], 12);
   memcpy(&ToSendDataBuffer[25], &calibrationParameters[CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_2], 12);
}

static void _parse_command_short(uint8_t cmd)
{
    switch(cmd)
    {
        case 0x3C:
            --os.encoderCount;
            break;
        case 0x3D:
            ++os.encoderCount;
            break;
        case 0x3E:
            ++os.buttonCount;
            break;
        case 0x3F:
            rtcc_write_eeprom();
            break;
    }
}

static void _parse_command_long(uint8_t cmd, uint8_t data)
{
    switch(cmd)
    {
        case 0x40:
            rtcc_set_year(data);
            break;
        case 0x41:
            rtcc_set_month(data);
            break;
        case 0x42:
            rtcc_set_day(data);
            break;
        case 0x43:
            rtcc_set_hours(data);
            break;
        case 0x44:
            rtcc_set_minutes(data);
            break;
        case 0x45:
            rtcc_set_seconds(data);
            break;
    }    
}

static void _parse_command_calibration(uint8_t cmd, uint8_t item, uint8_t dat1, uint8_t dat2, uint8_t dat3)
{
    int16_t parameter = dat1;
    parameter <<= 8;
    parameter |= dat2;
    //Store changes in RAM
    switch(item & 0x0F)
    {
        //Offset
        case 0x00:
            calibrationParameters[item>>4].Offset = parameter;
            break;
        //Slope
        case 0x01:
            calibrationParameters[item>>4].Multiplier = parameter;
            calibrationParameters[item>>4].Shift = dat3;
            break;
    }
    //Schedule changes to be written to EEPROM
    switch((calibrationIndex_t) item>>4)
    {
        case CALIBRATION_INDEX_INPUT_VOLTAGE:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_INPUT_VOLTAGE);
            break;
        case CALIBRATION_INDEX_OUTPUT_VOLTAGE:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_OUTPUT_VOLTAGE);
            break;
        case CALIBRATION_INDEX_INPUT_CURRENT:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_INPUT_CURRENT);
            break;
        case CALIBRATION_INDEX_OUTPUT_CURRENT:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_OUTPUT_CURRENT);
            break;
        case CALIBRATION_INDEX_ONBOARD_TEMPERATURE:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_ONBOARD_TEMPERATURE);
            break;
        case CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_1:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_EXTERNAL_TEMPERATURE_1);
            break;
        case CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_2:
            schedule_eeprom_write_task(EEPROM_WRITE_TASK_CALIBRATION_EXTERNAL_TEMPERATURE_2);
            break;
    }
}
