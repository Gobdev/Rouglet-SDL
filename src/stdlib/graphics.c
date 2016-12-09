#include <stdint.h>
#include <math.h>
#include <pic32mx.h>
#include "graphics.h"
#include "stdlib.h"
#include "../images/alphabet.h"
#include "../images/symbols.h"
#include "../images/item_sprites.h"

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

 //oldGraphics
char oledBuffer[OLED_MAX_BYTES] = {0};
char debugBuffer[OLED_MAX_BYTES] = {0};
const char whiteBox[7] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
int debug = 0;
int debug_pages_len[4] = {0};

void enable_debug(){
    debug = 1;
}

void inititalize_host(){
    /* Initialize SPI port 2. 
    */ 
    SPI2CON = 0; 
    SPI2BRG = 15; //8Mhz, with 80Mhz PB clock 
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
    delay_ms(1); 
    /* Start by turning VDD on and wait a while for the power to come up.  
    */
    ACTIVATE_VDD;
    delay_ms(10); 
    /* Display off command 
    */ 
    Spi2PutByte(0xAE); 
    /* Bring Reset low and then high 
    */ 
    ACTIVATE_RESET; 
    delay_ms(1); 
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
    delay_ms(100); 
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


const char* getCharacterPointer(char character){
    if (character > 0x1F && character < 0x40){
        return symbols + (character - 0x20) * 3 + 2;
    }
    if (character > 0x40 && character < 0x5B){
        return letters + (character - 0x41) * 3 + 2;
    }
    if (character > 0x60 && character < 0x7B){
        return letters + (character - 0x61) * 3 + 2;
    }
}

void paint_from_items(int x, int y, int item_id){
    int i;
    int start_position = item_id * 7;
    char draw_array[9] = {0};
    draw_array[0] = 7;
    draw_array[1] = 7;
    for(i = 2; i < 9 ; i++){
        draw_array[i] = items[start_position];
        start_position++;
    }
    paint_pic(x, y, draw_array);
}

void print_debug(int page, char* string){
    int i, j, length;
    length = strlen(string);
    const char* letter;
    debug_pages_len[page] = length;
    for (i = 0; i < length; i++){
        if (i * 4 + 3 > OLED_ROW_LENGTH)
            break;
        letter = getCharacterPointer(string[i]);
        for (j = 0; j < 3; j++){ 
            debugBuffer[page * OLED_ROW_LENGTH + i * 4 + j] = 0x0;
            debugBuffer[page * OLED_ROW_LENGTH + i * 4 + j] |= letter[j];
        }
        debugBuffer[page * OLED_ROW_LENGTH + i * 4 + 3] |= 0x0;   
    }
}

void print_int(int x, int y, int value){
    char str[15] = {0};
    int_to_string(value, str);
    print_text(x, y, str);
}

void print_char(int x, int y, char value){
    char str[15] = {0};
    char_to_string(value, str);
    print_text(x, y, str);   
}

void print_hex(int x, int y, int value){
    char str[15] = {0};
    unsigned int newValue = value; 
    int_to_hex_string((unsigned int) value, str);
    print_text(x, y, "0x");
    print_text(x + 8, y, str);
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

void paint_pic(int x, int y, const unsigned char* pic){
    /*
        Read the size of the image from the first two bytes of the picture array.
    */
    int x_size = pic[0];  
    int y_size = pic[1];
    pic += 2;
    int i, j, shift, first_page, last_page, current_page, picPage;
    char clearLeft, clearRight;

    /*
        If not a single pixel of the image falls on the screen, the rest of the code is skipped and the function returns here.
    */
    if (x < -x_size || x > OLED_ROW_LENGTH + x_size || y < -y_size || y > OLED_COL_LENGTH * OLED_PAGES + y_size){
        return;
    }

    /*
        Set the value that the images get shifted by when they aren't drawn at the top pixel of a page.
        E.g. an image drawn on x = 0, y = 5 will be drawn at x = 0, y = 0 but shifted 5 pixels downward instead.
    */
    shift = y % OLED_COL_LENGTH;
    if (shift < 0)
        shift += OLED_COL_LENGTH;

    first_page = (y - OLED_COL_LENGTH * (y < 0)) / OLED_COL_LENGTH * OLED_ROW_LENGTH; // Sets the first page of the oled to be drawn to.
    last_page = first_page + ((y_size + shift) / OLED_COL_LENGTH) * OLED_ROW_LENGTH;  // Sets the last page to be drawn to.
    current_page = first_page;
    
    /* 
        To no clear any pixels that are not used by this image, the clear bits are shifted by the same amount as the picture bits.
    */
    clearLeft = ~(0xFF << shift);                   
    clearRight = ~(0xFF >> (OLED_COL_LENGTH - shift));
    
    /*
        Iterates for as many pages as the picture needs to be drawn.
        E.g a 2x2 image will need two pages if it is drawn at x = 0, y = 7, while a 7x7 will need one page if drawn at x = 0, y = 0.
    */
    for (picPage = 0; picPage < (y_size / OLED_COL_LENGTH + 1) * x_size; picPage += x_size){
        if (current_page >= 0 && current_page < OLED_MAX_BYTES){
            /*
                Draw the upper part of this page of the image. Shift it to the left to draw it at the wanted pixel.
            */
            for (j = 0; j < x_size; j++){
                if (x + j >= 0 && x + j < OLED_ROW_LENGTH){
                    oledBuffer[current_page + x + j] &= clearLeft;                  // Clear the bits to be used.
                    oledBuffer[current_page + x + j] |= pic[j + picPage] << shift;  // Set the bits to the values in the picture.    
                }
            }
        }
        current_page += OLED_ROW_LENGTH;    //Switch to the next page.
        if (current_page <= last_page && current_page >= 0 && current_page < OLED_MAX_BYTES){
            /*
                Draw the lower part of this page of the image. This time the image is shifted to the right, since we want 
                to draw its lower half on the upper part of the screen.
            */
            for (j = 0; j < x_size; j++){
                if (x + j >= 0 && x + j < OLED_ROW_LENGTH){
                    oledBuffer[current_page + x + j] &= clearRight;                                     // Clear the bits to be used
                    oledBuffer[current_page + x + j] |= pic[j + picPage] >> (OLED_COL_LENGTH - shift);  // Set the bits to the value in the picture
                }
            }
        }
        /*
            Now both of the parts of the image should have been drawn.
        */
    }
}

void print_text(int x, int y, char* str){
    int length = strlen(str);
    int i, j;
    const char *letterPic;
    char currentLetter[5] = {3, 5, 0, 0, 0};
    for (i = 0; i < length; i++){
        letterPic = getCharacterPointer(str[i]);
        for (j = 0; j < 3; j++){
            currentLetter[2 + j] = letterPic[j];
        }
        paint_pic(x + i * 4, y, currentLetter);
    }
}


void putDebugInBuffer(){
    int page, i, j;
    for (page = 0; page < OLED_PAGES; page++){
        for (i = 0; i < debug_pages_len[page]; i++){
            for (j = 0; j < 4; j++){
                oledBuffer[page * OLED_ROW_LENGTH + i * 4 + j] = 0x0;
                oledBuffer[page * OLED_ROW_LENGTH + i * 4 + j] = debugBuffer[page * OLED_ROW_LENGTH + i * 4 + j];
            }
        }
    }
}

void update_screen(){
    if (debug)
        putDebugInBuffer();
    OledUpdate(oledBuffer);
}
