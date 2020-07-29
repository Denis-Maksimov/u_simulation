#ifndef LCD_API_H
#define LCD_API_H

typedef struct
{
    char* name;
    int val;
    int min; 
    int max;
    char** submenus; //
}MenuItem;
 
 
extern int lsd_sellect(int* i,int* ok_pressed,MenuItem* item);



#endif // !LCD_API_H