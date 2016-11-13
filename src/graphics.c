#include <stdint.h>
#include <pic32mx.h>
#include "images/character_sprites.c"

#define CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define ACTIVATE_RESET (PORTGCLR = 0x200)
#define DO_NOT_RESET (PORTGSET = 0x200)

#define ACTIVATE_VDD (PORTFCLR = 0x40)
#define TURN_OFF_VDD (PORTFSET = 0x40)

#define TURN_OFF_VBAT (PORTFSET = 0x20)
#define ACTIVATE_VBAT (PORTFCLR = 0x20)


#define OLED_MAX_BYTES 512 
//max number of bytes in display buffer 
#define OLED_COL_LENGTH 8 
//number of pixels in a column 
#define OLED_ROW_LENGTH 128
//number of pixels in a row
#define OLED_PAGES 4 
//number of display memory pages 

char oledBuffer[OLED_MAX_BYTES] = {0};
char debugBuffer[OLED_MAX_BYTES] = {0};
int debug = 0;

void enableDebug(){
    debug = 1;
}

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

char Spi2PutByte(char data) {
    /* Wait for transmitter to be ready 
    */ 
    while(!(SPI2STAT & 0x08));
    /* Write the next transmit byte. 
    */ 
    SPI2BUF = data;
    /* Wait for receive byte. 
    */ 
    while(!(SPI2STAT & 1));
    /* Return the received byte in the buffer. 
    */ 
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
    Spi2PutByte(0xAE); 
    /* Bring Reset low and then high 
    */ 
    ACTIVATE_RESET; 
    delayMs(1); 
    DO_NOT_RESET; 
    /* Send the Set Charge Pump and Set Pre-Charge Period commands 
    */ 
    Spi2PutByte(0x8D); 
    Spi2PutByte(0x14); 
    Spi2PutByte(0xD9); 
    Spi2PutByte(0xF1); 
    /* Turn on VCC and wait 100ms 
    */ 
    ACTIVATE_VBAT;
    delayMs(100); 
    /* Send the commands to invert the display. This puts the display origin 
    ** in the upper left corner. 
    */ 
    Spi2PutByte(0xA1); 
    //remap columns 
    Spi2PutByte(0xC8); 
    //remap the rows 
    /* Send the commands to select sequential COM configuration. This makes the 
    ** display memory non-interleaved. 
    */ 
    Spi2PutByte(0xDA); 
    //set COM configuration command 
    Spi2PutByte(0x20); 
    //sequential COM, left/right remap enabled 
    /* Send Display On command 
    */ 
    Spi2PutByte(0xAF); 
} 

void OledPutBuffer(int cb, char* rgbTx) { 
    int ib; 
    char bTmp; 
    /* Write/Read the data 
    */ 
    for (ib = 0; ib < cb; ib++) { 
        bTmp = Spi2PutByte(*rgbTx++);
    }   
} 

void OledUpdate(char* buffer) { 
    int ipag; 
    int icol; 
    char* pb; 
    pb = buffer; 
    for (ipag = 0; ipag < OLED_PAGES; ipag++) { 
        CHANGE_TO_COMMAND_MODE; 
        /* Set the page address 
        */ 
        Spi2PutByte(0x22); 
        //Set page command 
        Spi2PutByte(ipag); 
        //page number 
        /* Start at the left column 
        */ 
        Spi2PutByte(0x00); 
        //set low nybble of column 
        Spi2PutByte(0x10); 
        //set high nybble of column 
        CHANGE_TO_DATA_MODE; 
        /* Copy this memory page of display data. 
        */ 
        OledPutBuffer(OLED_ROW_LENGTH, pb);
        pb += OLED_ROW_LENGTH; 
    } 
} 

void clearScreen(){
    int i;
    for (i = 0; i < OLED_MAX_BYTES; i++){
        oledBuffer[i] = 0x0;
    }
}

void paintOnePixel(int x, int y){
    if (y < 0 || y > 32 || x < 0 || x > 128){
        return;
    } 
    oledBuffer[y / OLED_COL_LENGTH * OLED_ROW_LENGTH + x] |= 1 << (y % OLED_COL_LENGTH);
}

void paint7x7(int x, int y, const char* pic){
    int size = 7;
    int i, shift, page, clearBits;

    if (x < -size || x > OLED_ROW_LENGTH - size || y < -size || y > OLED_COL_LENGTH * OLED_PAGES - size)
        return;
    
    shift = y % OLED_COL_LENGTH;
    page = (y / OLED_COL_LENGTH) * OLED_ROW_LENGTH; // Select page
    if (page >= 0){
        for (i = 0; i < size; i++){
            if (x + i < 0)
                continue;
            clearBits = ~(0xFF << shift);
            oledBuffer[page + x + i] &= clearBits; // Clear the bits to be used.
            oledBuffer[page + x + i] |= pic[i] << shift;  // Set the bits to the values in the picture.
        }
    }
    if (shift >= OLED_COL_LENGTH - size && page >= -OLED_ROW_LENGTH && page < 3 * OLED_ROW_LENGTH){ // If the picture falls on two pages
        page += OLED_ROW_LENGTH;
        shift = OLED_COL_LENGTH - shift;
        for (i = 0; i < size; i++){
            if (x + i < 0)
                continue;
            clearBits = ~(0xFF >> shift);
            oledBuffer[page + x + i] &= clearBits; // Clear the bits to be used.
            oledBuffer[page + x + i] |= pic[i] >> shift;  // Set the bits to the values in the picture.       
        }
    }
}

void updateScreen(){
    OledUpdate(oledBuffer);
    if (debug)
        OledUpdate(debugBuffer);
}
