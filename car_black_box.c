/*
 * File:   car_black_box.c
 * Author: Dell
 *
 * Created on 14 December, 2023, 9:47 AM
 */


#include "car_black_box.h"

void display_default_screen()
{
    clcd_print("  Timer   E  SP",LINE1(0));
    //call timer display
    //call signature display
    display_signature();
    //call speed display based on potentiometer
    display_speed();
}

void display_signature()
{
    unsigned int gear_key;
    gear_key = read_matrix_keypad(EDGE);
    static int gear = 7;
    if(gear_key == 1)
    {
        gear = 0;
    }
    else if(gear_key == 2)
    {
        if(gear < 6)
        {
            gear++;
        }
        else if (gear == 7)
        {
            gear =1;
        }
    }
    else if(gear_key == 3)
    {
        if(gear > 1)
        {
            gear--;
        }
    }
    clcd_print(signature[gear],LINE2(10));
}

void display_speed()
{
    unsigned int speed =(read_adc(4))/10.23;
    clcd_putch(((speed/10) + 48),LINE2(14));
    clcd_putch(((speed%10) + 48),LINE2(15));
}




void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit;
	CLCD_PORT = byte;

	/* Should be atleast 200ns */
	CLCD_EN = HI;
	CLCD_EN = LO;

	PORT_DIR = INPUT;
	CLCD_RW = HI;
	CLCD_RS = INSTRUCTION_COMMAND;

	do
	{
		CLCD_EN = HI;
		CLCD_EN = LO;
	} while (CLCD_BUSY);

	CLCD_RW = LO;
	PORT_DIR = OUTPUT;
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;

	CLCD_RW = LO;

	
     /* Startup Time for the CLCD controller */
    __delay_ms(40);
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(1); 
    
    CURSOR_HOME;
    __delay_us(100);
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	while (*data != '\0')
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}



unsigned char read_matrix_keypad(char detection)
{
    unsigned char key;
    if(detection ==0)
    {
        return scan_key();
    }
    else if(detection == 1)
    {
        key = scan_key();
        static unsigned char once =1;
        if(key != 0xFF && once)
        {
            once =0;
            return key;
        }
        else if(key == 0xFF)
            once =1;
    }
    return 0xFF;
}


unsigned char scan_key()
{
    RB5=0;
    RB6=1;
    RB7=1;
    if(RB1 == 0) return 1;
    else if(RB2== 0) return 4;
    else if(RB3== 0) return 7;
    else if(RB4== 0) return 10;
    RB5=1;
    RB6=0;
    RB7=1;
    if(RB1 == 0) return 2;
    else if(RB2== 0) return 5;
    else if(RB3== 0) return 8;
    else if(RB4== 0) return 11;
    RB5=1;
    RB6=1;
    RB7=0;
    RB7=0;
    if(RB1 == 0) return 3;
    else if(RB2== 0) return 6;
    else if(RB3== 0) return 9;
    else if(RB4== 0) return 12;
    return 0xFF;
}

void init_matrix_keypad()
{
    TRISB = 0x1E;
    RBPU=0;
}


void init_adc()
{
    ADON =1;
    VCFG1=0;
    VCFG0=0;
    //we need to select channel 4 as analog so we are setting the last four bits as 1010
    PCFG3 =1;
    PCFG2=0;
    PCFG1=1;
    PCFG0=0;
    //beacause we need to have acqusition time as 8tad so the value is 100
    ACQT2=1;
    ACQT1=0;
    ACQT0=0;
}

unsigned short read_adc(unsigned char channel)
{
    //select channel from adcon0
    ADCON0 = (ADCON0 & 0xC3)|(channel <<2);
    //select justification as right
    ADFM =1;
    //set the GO bit for status
    GO =1;
    while(GO);
    return ADRESL|((ADRESH & 0x03)<<8);
}


void login_screen()
{
    unsigned int success =0;
    while(success != 1)
    unsigned char pass_key;
    unsigned char enter =0;
    //unsigned char success =0;
    unsigned char password[4] = "1010";
    unsigned char user_in[5];
    user_in[4]='/0';
    unsigned int check =0;
    unsigned int count =0;
    unsigned char cursor_flag=0;
    unsigned char chance=0;
    while(count < 5)
    {
        if(success == 0)
        {
            clcd_print(" ENTER PASSWORD ",LINE1(0));
        }
        pass_key = read_matrix_keypad(EDGE);
        if(enter ==0)
        {
            if(wait++ == 800)
            {
                wait =0;
                if(cursor_flag ==0)
                {
                    clcd_putch("_",LINE2(4+count));
                }
                else
                {
                    clcd_putch(" ",LINE2(4+count));
                }
                cursor_flag =~cursor_flag;
            }
        }
        if(pass_key ==11 || pass_key ==12)
        {
            clcd_putch("*",LINE2(4+count));
            user_in[count] = key+37;
            if(password[count] != user_in[count])
            {
                check=1;
            }
            count++;
        }
        if(check ==0 && count == 4)
        {
            enter =1;
            success =1;
            //set menu flag
        }
        else if(check ==1 && count == 4)
        {
            if(chance < 3)
            {
                clcd_print(" WRONG_PASSWORD ",LINE1(0));
                clcd_print("  Attempts left ",LINE2(0));
                clcd_putch(53-chance,LINE2(0));
                enter =0;
                success =0;
                chance++;
                for(unsigned long int i=100000;i--;);
                CLEAR_DISP_SCREEN;
            }
            else
            {
                clcd_print(" WAIT_FOR       ");
                clcd_print("                ");
                //timer delay for one minute
            }
        }
    }
    
}