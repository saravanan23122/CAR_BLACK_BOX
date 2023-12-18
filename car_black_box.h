/* 
 * File:   car_black_box.h
 * Author: Dell
 *
 * Created on 14 December, 2023, 9:57 AM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H
#include <xc.h>
#define LCD_H



#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7


#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0
#define _XTAL_FREQ                  20000000
#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME							clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE   0x33
#define EDGE 1
#define LEVEL 0

char *signature[8]={"C ","GN","G1","G2","G3","G4","GR","ON"};

void init_clcd(void);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_write(unsigned char bit_values, unsigned char control_bit);


void init_configure();

//1.Default screen display
void display_default_screen();
//  To display the gear signature
void display_signature();
// To display speed
void display_speed();

unsigned char read_matrix_keypad(char detection);

unsigned char scan_key();

void init_matrix_keypad();

void init_adc();

unsigned short read_adc(unsigned char channel);
//2.Event capture
//3.Login screen
//4.Menu

#endif	/* CAR_BLACK_BOX_H */

