/*
 * File:   main.c
 * Author: Dell
 *
 * Created on 14 December, 2023, 9:43 AM
 */


#include "car_black_box.h"
void init_configure()
{
    //initialize clcd
    init_clcd();
    //initialize matrix_keypad
    init_matrix_keypad();
    //initialize adc
    init_adc();
}


void main(void) {
    unsigned char key;
    init_configure();
    unsigned int flag=0;
    //unsigned int flag2=0;
    while(1)
    {
        key =read_matrix_keypad(EDGE);
        if(key == 10 ||key == 11 || key ==12)
            control_flag=1;
        
        if(control_flag == 0)
            display_default_screen();
        else if(control_flag == 1)
            login_screen();
        else if(control_flag == 2)
            display_menu_screen();
    }
    return;
}
