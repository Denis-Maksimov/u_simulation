#include "lcd1602_simulation.h"
#include "lcd_API.h"
 
int ch;

enum{
    btn_up=65,
    btn_down=66,
    btn_ok=10
};

///API

int lvl=0;

//---------------- Главное меню -----------------------
char* submenu_s[]={"temperature","hysteresis","monitor"};
MenuItem itemM={"Menu",3,0,2,submenu_s};



MenuItem item_temperature={"temp.",0,-20,20,0};
MenuItem item_hysteresis={"hysteresis",0,1,10,0};

char temp[12]={0};
char time[12]={0};

char* submenu_mon[]={temp,time};
MenuItem item_monitor={"monitor",0,0,1, submenu_mon};

MenuItem* item_2lvl[]={
    &item_temperature,
    &item_hysteresis,
    &item_monitor
    };


#include <string.h>
int main(int argc, char const *argv[])
{
    init();
    
    set_cursor(0,0);
    printf(" by Maksimov DN ");
    set_cursor(0,1);
    printf(" best regards!  ");

    strcpy(temp,"10.00");
    strcpy(time,"15:05");
    int i=0;
    int ok_pressed=0;
    
    u_sleep(2);
    do {
        temp[1]=='9'? temp[1]='0': temp[1]++;
        lcd_clear();

        if(ch==btn_up){i++;}
        if(ch==btn_down){i--;}
        if(ch==btn_ok){ok_pressed=1;}
        //------------------
        
        if(lvl){
            if(lsd_sellect(&i, &ok_pressed, item_2lvl[itemM.val])){
                lvl=0;
                itemM.val=itemM.max+1;
            }else{
                lvl=1;
            }
            
        }else{
            
            lsd_sellect(&i, &ok_pressed,&itemM);
            if(itemM.val>itemM.max){
                lvl=0;
            }else{
                lvl=1;
            }
        }
        

        if(ok_pressed) {
            ok_pressed = 0;
            set_cursor(0,3);//PC only..
            continue;
        }
        //------------------
        set_cursor(0,3);//PC only..
        
    }while ((ch = getc(stdin)) != 'q');

    reset();
    
	return 0;
}



