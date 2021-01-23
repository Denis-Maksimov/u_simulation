#include "main.h"



///API

int lvl=0;

//---------------- Главное меню -----------------------
/**
 * +settings
 * ... min temp {-20..30}
 * ... max temp { $min_temp..30}
 * ... time {%HH %MM}
 * ... mode {manual, auto}
 * ... control(*manual) {on,off}
 * +monitor
 * ... {temp state(on/off)}
 * ... {time}
 * 
 */




#include <string.h>
#include <time.h>
static pthread_mutex_t* pmtx;

enum btn_state{
    btn_up=65,
    btn_down=66,
    btn_ok=10
};  

int u_state=0;

int btn_get_state(){
    int rv;
    pthread_mutex_lock(pmtx);
        rv=u_state;
        u_state=0;
    pthread_mutex_unlock(pmtx);
    return rv;
}

void btn_set_state(int new){    
    pthread_mutex_lock(pmtx);
        u_state=new;
    pthread_mutex_unlock(pmtx);
}





void* gpio_cb(void* arg){
    // pthread_mutex_t* mtx=(pthread_mutex_t*)arg;
    int ch=0;
    do{
        btn_set_state(ch);
        u_sleep_ms(50);
    }while ((ch = getc(stdin)) != 'q');
    btn_set_state(ch);
    u_sleep(1);
    return 0;

}

//===============================================
void print_time();
void print_state();
struct 
{
    int max_temp;
    int min_temp;
    char mode;

}setts={0,0,0};

void print_mode(int x, int y){
    set_cursor(x,y);
    printf("      ");
    set_cursor(x,y);
    if(setts.mode&2){
        printf("auto");
        return;
    }
        printf("manual");
    
}
void set_mode(){
    lcd_clear();
    int btn=0;
    set_cursor(0,0);printf("*set.mode");
    while ((btn=btn_get_state())!='q'){
    if( btn==btn_up)
        setts.mode|=2;
    if( btn==btn_down)
        setts.mode&=~2;
    if( btn==' ')
        return;
    
    print_mode(0,1);
    u_sleep_ms(200);

    }
    
}

void set_state(){
    lcd_clear();
    int btn=0;
    set_cursor(0,0);printf("*control");
    while ((btn=btn_get_state())!='q'){
    if( btn==btn_up)
        setts.mode|=1;
    if( btn==btn_down)
        setts.mode&=~1;
    if( btn==' ')
        return;
    
    print_state();
    u_sleep_ms(200);

    }
    
}

void print_max_temp(){
    set_cursor(1,0);
    printf("max.temp");
    set_cursor(0,1);
    printf("    ");
    set_cursor(0,1);
    printf("%d",setts.max_temp);
}

void set_max_temp(){
    lcd_clear();
    int btn=0;
    set_cursor(0,0);printf("*");
    while ((btn=btn_get_state())!='q'){
    if( btn==btn_up)
        (setts.max_temp>=30)?(setts.max_temp=30):(setts.max_temp++);
    if( btn==btn_down)
        (setts.max_temp<=setts.min_temp)?(setts.max_temp=setts.min_temp):(setts.max_temp--);
    if( btn==' ')
        return;
    
    print_max_temp();
    u_sleep_ms(200);

    }
}
//---------------
void print_min_temp(){
    set_cursor(1,0);
    printf("min.temp");
    set_cursor(0,1);
    printf("    ");
    set_cursor(0,1);
    printf("%d",setts.min_temp);
}

void set_min_temp(){
    lcd_clear();
    int btn=0;
    set_cursor(0,0);printf("*");
    while ((btn=btn_get_state())!='q'){
    if( btn==btn_up)
        (setts.min_temp>=setts.max_temp)?(setts.min_temp=setts.max_temp):(setts.min_temp++);
    if( btn==btn_down)
        (setts.min_temp<=-20)?(setts.min_temp=-20):(setts.min_temp--);
    if( btn==' ')
        return;
    
    print_min_temp();
    u_sleep_ms(200);

    }
}

void settings_menu(){
    lcd_clear();
    int btn=0;
    int ptr=0;

    while ((btn=btn_get_state())!='q')
    {
        if( btn==btn_up)
            (ptr>=5)?(ptr=5):(ptr++);
        if( btn==btn_down)
            (ptr<=0)?(ptr=0):(ptr--);
        lcd_clear();
        set_cursor(0,0);
        switch (ptr)
        {
        case 0:
            print_min_temp();
            if( btn==' ')
                set_min_temp();
            break;
        case 1:
            print_max_temp();
            if( btn==' ')
                set_max_temp();
            break;
        case 2:
            printf(" set.time");
            print_time();
            if( btn==' ');
                //TODO
            break;
        case 3:
            printf(" set.mode");
            print_mode(0, 1);
            if( btn==' ')
                set_mode();
            break;
        case 4:
            printf(" control");
            print_state();
            if( btn==' ')
                set_state();
            break;
        case 5:
            printf(" exit");
            if( btn==' ')
                return;
            break;
        default:
            ptr=0;

        }

        set_cursor(0,3);
        fflush(stdout);
        btn=btn_get_state();
        u_sleep_ms(200);
        
    }
    printf("exit");


}




//===============================================
void print_temp(){
    set_cursor(0,0);
    printf("       ");
    set_cursor(0,0);
    printf("+25,5");
}
void print_state(){
    set_cursor(0,1);
    printf("   ");
    set_cursor(0,1);
    if(setts.mode&1){
        printf("on");
    }else{
        printf("off");
    }
}
void print_time()
{
    set_cursor(8,1);
    time_t ttime=time(0);
    struct tm* lt = localtime (&ttime);
    printf ("%02d:%02d.%02d", lt->tm_hour,lt->tm_min,lt->tm_sec );
}

void main_menu(){
    lcd_clear();
    int btn=0;
    // int ptr=0;

    while ((btn=btn_get_state())!='q')
    {
        // if( btn==btn_up)
        //     ptr++;
        // if( btn==btn_down)
        //     ptr--;
        if( btn==' ')
            settings_menu();
 

        print_temp();
        print_mode(8, 0);
        print_state();
        print_time();
        set_cursor(0,3);
        // printf("ptr %d",ptr);
        fflush(stdout);
        btn=btn_get_state();
        u_sleep_ms(200);
        
    }

}
//===============================================

int main(int argc, char const *argv[])
{
    init();
    pthread_t gpio;
    pthread_mutex_t mtx;
    pmtx=&mtx;
    pthread_mutex_init(&mtx,0);
    pthread_create(&gpio,0,gpio_cb,&mtx);
    
    main_menu();
    pthread_join(gpio,0);
    reset();
	return 0;
}



