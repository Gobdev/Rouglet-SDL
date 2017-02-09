#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "stdlib.h"
#include "../images/alphabet.h"
#include "../images/symbols.h"
#include "../images/item_sprites.h"
#include "../images/graphic_elements.h"



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

//Screen dimension constants 
const int SCREEN_WIDTH = 640; 
const int SCREEN_HEIGHT = 480;

const int PIXEL_SIZE = 4;

//oldGraphics
char oledBuffer[OLED_MAX_BYTES] = {0};
char debugBuffer[OLED_MAX_BYTES] = {0};
int debug = 0;
int debug_pages_len[4] = {0};

SDL_Window *window;
SDL_Renderer *renderer;

void enable_debug(){
    debug = 1;
}

void disable_debug(){
    debug = 0;
}

int get_oled_height(){
    return OLED_COL_LENGTH * OLED_PAGES;
}

int get_oled_width(){
    return OLED_ROW_LENGTH;
}




void initialize_display() 
{ 
    
    //Initialize SDL 
        //Create windows
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
        if( window == NULL )
        {
            //printf( "Window could not be created!");
        }
        else 
        {
            SDL_SetWindowTitle(window, "Rouglet 1.505");
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
} 



void GobSetPixel(int x, int y)
{
    int i, j;
    for(i = 0; i<PIXEL_SIZE; i++) {
        for(j = 0; j<PIXEL_SIZE; j++) {
            SDL_RenderDrawPoint(renderer, x*PIXEL_SIZE+i, y*PIXEL_SIZE+j);
        }
    }

}



void OledUpdate(char* buffer) { 
    int ibit;
    int ipag; 
    int irow;
    int icol; 
    char* pb; 
    pb = buffer; 
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (ipag = 0; ipag < OLED_PAGES; ipag++) { 
        for(irow = 0; irow< OLED_ROW_LENGTH; irow++)
        {
        for(ibit = 0; ibit<8; ibit++)
        {
            if((*pb >> ibit) & 1) GobSetPixel(irow,ipag*8+ibit);
        }
        pb++;
        }
    } 
	SDL_RenderPresent(renderer);
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
        draw_array[i] = items[start_position + i];
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

void print_hex(int x, int y, int value){
    char str[15] = {0};
    unsigned int newValue = value; 
    int_to_hex_string((unsigned int) value, str);
    print_text(x, y, "0x");
    print_text(x + 8, y, str);
}

void print_char(int x, int y, char value){
    print_int(x, y, ((int) value & ~(~0 << 8)));
}

void print_char_hex(int x, int y, char value){
    print_hex(x, y, ((int) value & ~(~0 << 8)));
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
    char clearBits, clear_left_to_do;

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
    shift = mod(y, OLED_COL_LENGTH);

    first_page = (y - OLED_COL_LENGTH * (y < 0)) / OLED_COL_LENGTH * OLED_ROW_LENGTH; // Sets the first page of the oled to be drawn to.
    last_page = first_page + ((y_size + shift) / OLED_COL_LENGTH) * OLED_ROW_LENGTH;  // Sets the last page to be drawn to.
    current_page = first_page;
    
    /* 
        To not clear any pixels that are not used by this image, the clear bits are shifted by the same amount as the picture bits.
    */
    if (y_size < OLED_COL_LENGTH)
        clearBits = 0xFF >> (OLED_COL_LENGTH - y_size);
    else
        clearBits = 0xFF; 
    clearBits = ~(clearBits << shift);
    for (j = 0; j < x_size; j++){
        if (x + j >= OLED_ROW_LENGTH)
            break;
        oledBuffer[first_page + x + j] &= clearBits;
    }

    if (y_size < OLED_COL_LENGTH)
        clear_left_to_do = shift + y_size - OLED_COL_LENGTH;
    else
        clear_left_to_do = y_size - shift;
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
                    oledBuffer[current_page + x + j] |= pic[j + picPage] << shift;  // Set the bits to the values in the picture.    
                }
            }
        }
        current_page += OLED_ROW_LENGTH;    //Switch to the next page.
        if (current_page <= last_page && current_page >= 0 && current_page < OLED_MAX_BYTES){
            if (clear_left_to_do > 0){
                for (j = 0; j < x_size; j++){
                    oledBuffer[current_page + x + j] &= 0xFF << clear_left_to_do;
                }
                clear_left_to_do -= OLED_COL_LENGTH;
            }
            /*
                Draw the lower part of this page of the image. This time the image is shifted to the right, since we want 
                to draw its lower half on the upper part of the screen.
            */
            for (j = 0; j < x_size; j++){
                if (x + j >= 0 && x + j < OLED_ROW_LENGTH){
                    if (x + j >= OLED_ROW_LENGTH)
                        break;
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

void pop_up_text(char* text1, char* text2){
    paint_pic(18, 5, pop_up_background);
    print_text(24, 9, text1);
    print_text(24, 16, text2);
    update_screen();
    buttonPress(1000);
    //clearScreen();
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
