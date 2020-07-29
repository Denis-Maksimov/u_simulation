#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "lcd1602_simulation.h"

//=======================================
static struct termios stored;
 
void init() 
{
    lcd_draw();
    struct termios settings;
 
    tcgetattr(0, &stored);
 
    settings = stored;
 
    settings.c_lflag &= (~ICANON);
    settings.c_lflag &= (~ECHO);
    settings.c_cc[VTIME] = 0;
    settings.c_cc[VMIN] = 1;
 
    tcsetattr(0, TCSANOW, &settings);
    
}
void reset(void) 
{
    tcsetattr(0, TCSANOW, &stored);
    gotoxy(0,5);
    resetcolor();

}

void u_sleep(unsigned int seconds)
{
    fflush(stdout);
    sleep(seconds);
}
//====================================




void lcd_draw(void) 
{
	home();
    clrscr();
	set_display_atrib(B_BLUE);
    set_display_atrib(F_WHITE);

	puts(	
       //         111111111122
       //123456789012345678901
        "┌─────lcd1602────┐\n" //0
        "|                |\n" //1
        "|                |\n" //2
		"└────────────────┘"); //3
	// resetcolor();
}



void lcd_clear()
{
    set_cursor(0,0);
    printf("                ");
    set_cursor(0,1);
    printf("                ");
}












