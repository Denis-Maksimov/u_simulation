 #include "lcd_API.h"
 #include "lcd1602_simulation.h"
 
 int lsd_sellect(int* i,int* ok_pressed,MenuItem* item)
 {
     (*i < item->min) ? (*i = item->min) : (*i);
     (*i > item->max) ? (*i = (item->max + 1)) : (*i);
     if (*ok_pressed)
     {
         
         set_cursor(0, 0);
         if (i[0] > item->max)
             printf("returning");
         else
         {
             item->val = i[0];
             printf("ok");
             if (item->submenus)
             {
                 printf(":");
                 printf(item->submenus[item->val]);
             }
         }
         u_sleep(1);
         i[0] = 0;
        //  lcd_clear();
        set_cursor(0, 1);
        printf("press up or down");
         return 1;
     }


     set_cursor(0, 0);
     printf(item->name);
     printf(":");
     if (item->submenus)
     {
        printf(item->submenus[item->val]);
     }
     else
     {
         printf("%d", item->val);
     }
     
     //------------------
     set_cursor(0, 1);
     if (*i > item->max)
     {
         printf("exit");
     }
     else
     {
         if (item->submenus)
             printf(item->submenus[*i]);
         else
             printf("i = %d\n", i[0]);
     }
     return 0;
 }
