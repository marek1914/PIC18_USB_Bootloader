/* 
 * File:   system.h
 * Author: Luke
 *
 * Created on 5. September 2016, 21:17
 */

#ifndef OS_H
#define	OS_H

#include <stdint.h>

#define  PPSUnLock()    {EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 0;}
#define  PPSLock() 		{EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 1;}

/*
 * Configuration bits
 */

/*
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 2       // PLL Prescaler Selection bits (Divide by 2 (8 MHz oscillator input))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset  (Enabled)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
//#pragma config CPUDIV = OSC2_PLL2// CPU System Clock Postscaler (CPU system clock divide by 2)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = ON       // T1OSCEN Enforcement (Secondary Oscillator clock source may be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = ON     // Deep Sleep BOR (Enabled)
#pragma config DSWDTEN = ON     // Deep Sleep Watchdog Timer (Enabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = OFF    // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set and cleared as needed)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)
*/

/*
 * General definitions
 */

#define _XTAL_FREQ 8000000

#define PIN_INPUT           1
#define PIN_OUTPUT          0
#define PIN_DIGITAL         1
#define PIN_ANALOG          0

#define PPS_FUNCTION_CCP1_OUTPUT_A 14
#define PPS_FUNCTION_CCP2_OUTPUT_B 15

//PPS Input registers
//SPI2 Data Input: RPINR21
//SPI2 Clock Input: RPINR22
//SPI2 Slave Select Input:RPINR23

//PPS Output registers
//SPI2 Data Output: 10
//SPI2 Clock Output: 11
//SPI DMA Slave Select: 12

#define PPS_FUNCTION_SPI2_MISO_INPUT RPINR21
#define PPS_FUNCTION_SPI2_MOSI_OUTPUT 10
#define PPS_FUNCTION_SPI2_SCLK_OUTPUT 11
#define PPS_FUNCTION_SPI2_SCLK_INPUT RPINR22
#define PPS_FUNCTION_SPI2_SS_OUTPUT 12

#define DISP_EN_TRIS TRISDbits.TRISD0
#define DISP_EN_PORT LATDbits.LD0

#define VCC_HIGH_TRIS TRISCbits.TRISC2
#define VCC_HIGH_PORT LATCbits.LC2

#define BUCK_ENABLE_TRIS TRISBbits.TRISB1
#define BUCK_ENABLE_PIN LATBbits.LB1
#define BUCK_LOWFET_TRIS TRISBbits.TRISB2
#define BUCK_LOWFET_PIN LATBbits.LB2
#define BUCK_LOWFET_PPS RPOR6
#define BUCK_HIGHFET_TRIS TRISBbits.TRISB3
#define BUCK_HIGHFET_PIN LATBbits.LB3
#define BUCK_HIGHFET_PPS RPOR5

#define PWROUT_ENABLE_TRIS TRISCbits.TRISC7
#define PWROUT_ENABLE_PIN LATCbits.LC7
#define PWROUT_CH1_TRIS TRISEbits.TRISE2
#define PWROUT_CH1_PIN LATEbits.LE2
#define PWROUT_CH2_TRIS TRISEbits.TRISE1
#define PWROUT_CH2_PIN LATEbits.LE1
#define PWROUT_CH3_TRIS TRISEbits.TRISE0
#define PWROUT_CH3_PIN LATEbits.LE0
#define PWROUT_CH4_TRIS TRISAbits.TRISA5
#define PWROUT_CH4_PIN LATAbits.LA5

#define USBCHARGER_EN_TRIS TRISDbits.TRISD3
#define USBCHARGER_EN_PIN LATDbits.LD3

#define BOARD_REVISION_E

#ifdef BOARD_REVISION_F
#define SPI_MISO_TRIS TRISDbits.TRISD6
#define SPI_MISO_PORT PORTDbits.RD6
#define SPI_MISO_PPS 23
#define SPI_MOSI_TRIS TRISDbits.TRISD4
#define SPI_MOSI_PORT LATDbits.LD4
#define SPI_MOSI_PPS RPOR21
#define SPI_SCLK_TRIS TRISDbits.TRISD5
#define SPI_SCLK_PORT LATDbits.LD5
#define SPI_SCLK_PPS_OUT RPOR22
#define SPI_SCLK_PPS_IN 22
#define SPI_SS1_TRIS TRISDbits.TRISD7
#define SPI_SS1_PIN LATDbits.LD7
#define SPI_SS1_PPS RPOR24
#define SPI_SS2_TRIS TRISDbits.TRISD2
#define SPI_SS2_PIN LATDbits.LD2
#define SPI_SS2_PPS RPOR19

#define FANOUT_TRIS TRISDbits.TRISD1
#define FANOUT_PIN LATDbits.LD1
#endif /* BOARD_REVISION_F */

#ifdef BOARD_REVISION_E
#define SPI_MISO_TRIS TRISDbits.TRISD6
#define SPI_MISO_PORT PORTDbits.RD6
#define SPI_MISO_PPS 23
#define SPI_MOSI_TRIS TRISDbits.TRISD4
#define SPI_MOSI_PORT LATDbits.LD4
#define SPI_MOSI_PPS RPOR21
#define SPI_SCLK_TRIS TRISDbits.TRISD5
#define SPI_SCLK_PORT LATDbits.LD5
#define SPI_SCLK_PPS_OUT RPOR22
#define SPI_SCLK_PPS_IN 22
#define SPI_SS1_TRIS TRISDbits.TRISD7
#define SPI_SS1_PIN LATDbits.LD7
#define SPI_SS1_PPS RPOR24
#define SPI_SS2_TRIS TRISDbits.TRISD1
#define SPI_SS2_PIN LATDbits.LD1

#define FANOUT_TRIS TRISDbits.TRISD2
#define FANOUT_PIN LATDbits.LD2
#endif /* BOARD_REVISION_E */

#define VOLTAGE_REFERENCE_TRIS TRISAbits.TRISA3
#define VOLTAGE_REFERENCE_ANCON ANCON0bits.PCFG3
#define TEMPERATURE1_TRIS TRISBbits.TRISB0
#define TEMPERATURE1_ANCON ANCON1bits.PCFG12
#define TEMPERATURE1_CHANNEL 0b1100
#define TEMPERATURE2_TRIS TRISAbits.TRISA1
#define TEMPERATURE2_ANCON ANCON0bits.PCFG1
#define TEMPERATURE2_CHANNEL 0b0001
#define TEMPERATURE3_TRIS TRISAbits.TRISA2
#define TEMPERATURE3_ANCON ANCON0bits.PCFG2
#define TEMPERATURE3_CHANNEL 0b0010

#define NUMBER_OF_TIMESLOTS 8

#define PUSHBUTTON_BIT PORTAbits.RA0
#define PUSHBUTTON_PPS 0
#define ENCODER_A_BIT PORTBbits.RB7
#define ENCODER_A_PPS 9
#define ENCODER_B_BIT PORTBbits.RB6
#define ENCODER_B_PPS 10

#define USB_CHARGING_VOLTAGE_MINIMUM 12000
#define POWER_OUTPUTS_VOLTAGE_MINIMUM -1
#define BUCK_DUTYCYCLE_ASYNCHRONOUS_MINIMUM 40
#define BUCK_DUTYCYCLE_SYNCHRONOUS_MINIMUM 150
#define BUCK_DUTYCYCLE_MAXIMUM 242
#define BUCK_BATTERY_VOLTAGE_MAXIMUM 13500

#define EEPROM_RTCC_ADDRESS 0x0040


/*
 * Type definitions
 */

typedef enum 
{ 
    DISPLAY_MODE_BOOTLOADER_START = 0x00,
    DISPLAY_MODE_BOOTLOADER_FILE_FOUND = 0x10,
    DISPLAY_MODE_BOOTLOADER_FILE_VERIFYING = 0x11
            
} displayMode_t;

typedef enum 
{ 
    BOOTLOADER_MODE_START = 0x00,
    BOOTLOADER_MODE_FILE_FOUND = 0x10,
    BOOTLOADER_MODE_FILE_VERIFYING = 0x11
} bootloaderMode_t;

typedef enum 
{ 
    CPU_FREQUENCY_32kHz,
    CPU_FREQUENCY_8MHz,
    CPU_FREQUENCY_48MHz
} clockFrequency_t;

typedef enum 
{ 
    BUCK_OFF,
    BUCK_FREQUENCY_62500,
    BUCK_FREQUENCY_93750,
    BUCK_FREQUENCY_187500
} buckFrequency_t;

typedef enum 
{ 
    OUTPUT_1 = 0x01,
    OUTPUT_2 = 0x02,
    OUTPUT_3 = 0x04,
    OUTPUT_4 = 0x08,
    OUTPUT_ALL = 0x0F,
    OUTPUT_USB = 0x10,
    OUTPUT_FAN = 0x20,        
} outputs_t;

typedef enum
{
    BOARD_VOLTAGE_LOW,
    BOARD_VOLTAGE_HIGH 
} boardVoltage_t;

typedef enum
{
    CALIBRATION_INDEX_INPUT_VOLTAGE = 0,
    CALIBRATION_INDEX_OUTPUT_VOLTAGE = 1,
    CALIBRATION_INDEX_INPUT_CURRENT = 2,
    CALIBRATION_INDEX_OUTPUT_CURRENT = 3,
    CALIBRATION_INDEX_ONBOARD_TEMPERATURE = 4,
    CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_1 = 5,
    CALIBRATION_INDEX_EXTERNAL_TEMPERATURE_2 = 6,
    CALIBRATION_INDEX_COUNT = 7
} calibrationIndex_t;

typedef struct
{
    int16_t NeutralOffset;
    int16_t NeutralMultiplier;
    int8_t NeutralShift;
    int16_t Offset;
    int16_t Multiplier;
    int8_t Shift;
    int16_t AutoCalibration;
} calibration_t;

typedef struct
{
    volatile int8_t encoderCount;
    volatile int8_t buttonCount;
    volatile uint8_t timeSlot;
    volatile uint8_t done;
    //Bootloader
    bootloaderMode_t bootloader_mode;
    //Display
    displayMode_t display_mode;
} os_t;


/*
 * Global variables
 */

os_t os;


/*
 * Function prototypes
 */


void tmr_isr(void);
void system_init(void);
void system_delay_ms(uint8_t ms);
void system_encoder_enable(void);
void system_encoder_disable(void);

#endif	/* OS_H */

