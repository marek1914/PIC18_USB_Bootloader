
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "i2c.h"
#include "os.h"
#include "rtcc.h"
#include "bootloader.h"


char display_content[4][20];

static void _display_start(void);
static void _display_found(void);

uint8_t display_get_character(uint8_t line, uint8_t position)
{
    return display_content[line][position];
}

static void _display_clear(void)
{
    uint8_t row;
    uint8_t col;
    for(row=0;row<4;++row)
    {
        for(col=0;col<20;++col)
        {
            display_content[row][col] = ' ';
        }
    }
}

static uint8_t _display_itoa_u32(uint32_t value,  char *text)
{
    if(value>100000000)
    {
        itoa(text, (uint16_t)(value/10000), 10);
        itoa(text+5, (uint16_t)(value%10000), 10);
        *(text+9) = ' ';
        return 9;
    }
    else if(value>10000000)
    {
        itoa(text, (uint16_t)(value/10000), 10);
        itoa(text+4, (uint16_t)(value%10000), 10);
        *(text+8) = ' ';
        return 8;
    }
    else if(value>1000000)
    {
        itoa(text, (uint16_t)(value/10000), 10);
        itoa(text+3, (uint16_t)(value%10000), 10);
        *(text+7) = ' ';
        return 7;
    }
    else if(value>100000)
    {
        itoa(text, (uint16_t)(value/10000), 10);
        itoa(text+2, (uint16_t)(value%10000), 10);
        *(text+6) = ' ';
        return 6;
    }
    else if(value>10000)
    {
        itoa(text, (uint16_t)(value/10000), 10);
        itoa(text+1, (uint16_t)(value%10000), 10);
        *(text+5) = ' ';
        return 5;
    }
    else if(value>1000)
    {
        itoa(text, value, 10);
        *(text+4) = ' ';
        return 4;
    }
    else if(value>1000)
    {
        itoa(text, value, 10);
        *(text+3) = ' ';
        return 3;
    }
    else if(value>1000)
    {
        itoa(text, value, 10);
        *(text+2) = ' ';
        return 2;
    }
    else
    {
        itoa(text, value, 10);
        *(text+1) = ' ';
        return 1;
    }
}

static void _display_itoa(int16_t value, uint8_t decimals, char *text)
{
    uint8_t pos;
    uint8_t len;
    int8_t missing;
    char tmp[10];
    itoa(tmp, value, 10);
    len = strlen(tmp);
    
    if(value<0) //negative values
    {
        missing = decimals + 2 - len;
        if(missing>0) //zero-padding needed
        {
            for(pos=decimals;pos!=0xFF;--pos)
            {
                if(pos>=missing) //there is a character to copy
                {
                    tmp[pos+1] = tmp[pos+1-missing];
                }
                else //there is no character
                {
                    tmp[pos+1] = '0';
                }
            }
            len = decimals + 2;
        }  
    }
    else
    {
        missing = decimals + 1 - len;
        if(missing>0) //zero-padding needed
        {
            for(pos=decimals;pos!=0xFF;--pos)
            {
                if(pos>=missing) //there is a character to copy
                {
                    tmp[pos] = tmp[pos-missing];
                }
                else //there is no character
                {
                    tmp[pos] = '0';
                }
            }
            len = decimals + 1;
        }       
    }
 
    decimals = len - decimals - 1;
    
    for(pos=0;pos<len;++pos)
    {
        text[pos] = tmp[pos];
        if(pos==decimals)
        {
            //Insert decimal point
            ++pos;
            text[pos] = '.';
            break;
        }
    }
    for(;pos<len;++pos)
    {
        text[pos+1] = tmp[pos];
    }
}

void display_prepare(uint8_t mode)
{
    _display_clear();
    
    switch(mode&0xF0)
    {
        case DISPLAY_MODE_BOOTLOADER_START:
            _display_start();
            break;
            
        case DISPLAY_MODE_BOOTLOADER_FILE_FOUND:
            _display_found();
            break;
            
        default:
            os.display_mode = DISPLAY_MODE_BOOTLOADER_START;
            _display_start();
    }
}

static void _display_start(void)
{
    uint8_t cntr;
    const char line1[] = "Bootloader Mode:";
    const char line2[] = "Looking for file";
    const char line3[] = "FIRMWARE.HEX on USB";
    const char line4[] = "drive...";
    
    cntr = 0;
    while(line1[cntr])
        display_content[0][cntr] = line1[cntr++];
    cntr = 0;
    while(line2[cntr])
        display_content[1][cntr] = line2[cntr++];
    cntr = 0;
    while(line3[cntr])
        display_content[2][cntr] = line3[cntr++];
    cntr = 0;
    while(line4[cntr])
        display_content[3][cntr] = line4[cntr++];
}

static void _display_found(void)
{
    uint8_t cntr;
    uint8_t start;
    const char line1[] = "Bootloader Mode:";
    const char line2[] = "FIRMWARE.HEX found";
    const char line3[] = "Size: ";
    const char line3b[] = " bytes";
    const char line4[] = "Entries: ";
    
    cntr = 0;
    while(line1[cntr])
        display_content[0][cntr] = line1[cntr++];
    cntr = 0;
    while(line2[cntr])
        display_content[1][cntr] = line2[cntr++];
    cntr = 0;
    while(line3[cntr])
        display_content[2][cntr] = line3[cntr++];
    start = cntr;
    start += _display_itoa_u32(bootloader_get_file_size(), &display_content[2][cntr]);
    cntr = 0;
    while(line3b[cntr])
        display_content[2][start+cntr] = line3b[cntr++];
    cntr = 0;
    while(line3b[cntr])
        display_content[3][start+cntr] = line3b[cntr++];
    start = cntr;
    start += _display_itoa_u32(bootloader_get_entries(), &display_content[3][cntr]);
    
}

void display_update(void)
{
    i2c_display_cursor(0, 0);
    i2c_display_write_fixed(&display_content[0][0], 20);
    i2c_display_cursor(1, 0);
    i2c_display_write_fixed(&display_content[1][0], 20);
    i2c_display_cursor(2, 0);
    i2c_display_write_fixed(&display_content[2][0], 20);
    i2c_display_cursor(3, 0);
    i2c_display_write_fixed(&display_content[3][0], 20);
}