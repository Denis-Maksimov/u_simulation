#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <stdint.h>
#include <pthread.h>
#define ESC "\033"

//Format text
#define RESET 		0
#define BRIGHT 		1
#define DIM			2
#define UNDERSCORE	3
#define BLINK		4
#define REVERSE		5
#define HIDDEN		6

//Foreground Colours (text)

#define F_BLACK 	30
#define F_RED		31
#define F_GREEN		32
#define F_YELLOW	33
#define F_BLUE		34
#define F_MAGENTA 	35
#define F_CYAN		36
#define F_WHITE		37

//Background Colours
#define B_BLACK 	40
#define B_RED		41
#define B_GREEN		42
#define B_YELLOW	44
#define B_BLUE		44
#define B_MAGENTA 	45
#define B_CYAN		46
#define B_WHITE		47

//-----------------------------------------

#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //lear the screen, move to (1,1)
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define set_cursor(x,y)	gotoxy(x+2,y+2)
#define visible_cursor() printf(ESC "[?251");
//Set Display Attribute Mode	<ESC>[{attr1};...;{attrn}m
#define resetcolor() printf(ESC "[0m")
#define set_display_atrib(color) 	printf(ESC "[%dm",color)

//--------------------------------------------------------
uint32_t idr=0x00000000;


void lcd_draw(void) 
{
	home();
	set_display_atrib(B_BLUE);
    set_display_atrib(F_WHITE);

	puts(	
       //         111111111122
       //123456789012345678901
        "┌─────lcd1602────┐\n" //0
        "|                |\n" //1
        "|                |\n" //2
		"└────────────────┘"); //3
	resetcolor();
}
void mcu_draw(int x,int y) 
{
	gotoxy(x,y);
    // clrscr();
	// set_display_atrib(B_BLUE);
    // set_display_atrib(F_WHITE);

	puts(	
       //         111111111122
       //123456789012345678901
        "  ┌────┐  \n" //0
        " -|    |- \n" //1
        " -|    |- \n" //2
        " -|    |- \n" //3
        " -|    |- \n" //4
		"  └────┘  \n" //5
        );
	if(idr&0x1) {gotoxy(x,y+1);printf("*");}
    if(idr&0x2){ gotoxy(x,y+2);printf("*");}
    if(idr&0x4){ gotoxy(x,y+3);printf("*");}
    if(idr&0x8) {gotoxy(x,y+4);printf("*");}
    if(idr&0x10) {gotoxy(x+9,y+1);printf("*");}
    if(idr&0x20) {gotoxy(x+9,y+2);printf("*");}
    if(idr&0x40) {gotoxy(x+9,y+3);printf("*");}
    if(idr&0x80) {gotoxy(x+9,y+4);printf("*");}
            
     
    fflush(stdout);
}


static struct termios stored;
void init_w()
{
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
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
    home();
    clrscr();
    resetcolor();

}

//--------------------------------------------------------
void switch_input(int buf){
    switch (buf)
            {
            case 'z':
                idr^=0x01;break;
            case 'x':
                idr^=0x02;break;
            case 'c':
                idr^=0x04;break;
            case 'v':
                idr^=0x08; break;
            case 'a':
                idr^=0x10; break;
            case 's':
                idr^=0x20; break;
            case 'd':
                idr^=0x40; break;
            case 'f':
                idr^=0x80; break;
            default:
                break;
            }
}
//--------------------------------------------------------
int sellect_work()
{

    // int wath_duck=15;
    fd_set fd_in;
    struct timeval tv;

    while(1){  
        FD_ZERO( &fd_in );
        FD_SET( STDIN_FILENO, &fd_in );//добавляем в сет


        tv.tv_sec = 0;
        tv.tv_usec = 50000;
  
        
        int ret = select( STDIN_FILENO + 1, &fd_in, NULL, NULL, &tv );
        // проверяем успешность вызова
        if ( ret == -1 )
        {
            perror("SELECT_ERROR!!\n");
            return -1;  

        }else if( ret == 0 )
        {
            // таймаут, событий не произошло 
            // puts("tick");
            clrscr();
            lcd_draw();
            mcu_draw(1,6);
        
        }else{
            // обнаружили событие

            if (FD_ISSET(STDIN_FILENO, &fd_in) )
            {
               int buf=0;
               read(STDIN_FILENO,&buf,sizeof(int));
               switch_input(buf);
                if(buf=='q')return 0;

            //    printf("code=%d\n",buf);
            
            }
        }   


    }
}



void *routine(void* arg)
{
    
}

int main(int argc, const char *argv[])
{

    init_w();
    pthread_t th;
    pthread_create(&th,0,routine,0);
    sellect_work();
    reset();
    pthread_join(th,0);
    return 0;
}






