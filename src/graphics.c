#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"


#define CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define ACTIVATE_RESET (PORTGCLR = 0x200)
#define DO_NOT_RESET (PORTGSET = 0x200)

#define ACTIVATE_VDD (PORTFCLR = 0x40)
#define ACTIVATE_VBAT (PORTFCLR = 0x20)

#define TURN_OFF_VDD (PORTFSET = 0x40)
#define TURN_OFF_VBAT (PORTFSET = 0x20)

void delayMs(int milliseconds){
    int i;
    long j;
    long ms = 80000;
    for (i = 0; i < milliseconds; ++i){
        for (j = 0; j < ms; ++j){}
    }
}

void inititalize_host(){
    /* Initialize SPI port 2. 
    */ 
    SPI2CON = 0; 
    SPI2BRG = 15; 
    //8Mhz, with 80Mhz PB clock 
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    SPI2CONSET = 0x8060;
    
    /* Make pins RF4, RF5, and RF6 be outputs. 
    */ 
    //PORTFSET = 0x70;
    CHANGE_TO_COMMAND_MODE;     // Command mode 
    ACTIVATE_VDD;               // Turn on VDD power
    ACTIVATE_VBAT;              // Turn on VBAT power

    /* Make the RG9 pin be an output. On the Basic I/O Shield, this pin 
    ** is tied to reset. 
    */ 
    ACTIVATE_RESET;
    DO_NOT_RESET;
}

uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
    return SPI2BUF;
}


void inititalize_display() 
{ 
    inititalize_host();
    /* We're going to be sending commands, so clear the Data/Cmd bit 
    */ 
    CHANGE_TO_COMMAND_MODE; 
    delayMs(1); 
    /* Start by turning VDD on and wait a while for the power to come up.  
    */
    ACTIVATE_VDD;
    delayMs(10); 
    /* Display off command 
    */ 
    spi_send_recv(0xAE); 
    /* Bring Reset low and then high 
    */ 
    ACTIVATE_RESET; 
    delayMs(1); 
    DO_NOT_RESET; 
    /* Send the Set Charge Pump and Set Pre-Charge Period commands 
    */ 
    spi_send_recv(0x8D); 
    spi_send_recv(0x14); 
    spi_send_recv(0xD9); 
    spi_send_recv(0xF1); 
    /* Turn on VCC and wait 100ms 
    */ 
    ACTIVATE_VBAT;
    delayMs(100); 
    /* Send the commands to invert the display. This puts the display origin 
    ** in the upper left corner. 
    */ 
    spi_send_recv(0xA1); 
    //remap columns 
    spi_send_recv(0xC8); 
    //remap the rows 
    /* Send the commands to select sequential COM configuration. This makes the 
    ** display memory non-interleaved. 
    */ 
    spi_send_recv(0xDA); 
    //set COM configuration command 
    spi_send_recv(0x20); 
    //sequential COM, left/right remap enabled 
    /* Send Display On command 
    */ 
    spi_send_recv(0xAF); 
} 