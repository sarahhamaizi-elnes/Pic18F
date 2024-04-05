 #include "horloge.h"
#include "LCD.h"

char S1 = 48  ;
char S2 = 48  ;
char M1 = 48 ;
char M2 = 48  ;
char H1 = 48  ;
char H2 = 48 ;
char day = 1 ;
int controWatch =0;

void horloge_compute()
{
       S1++ ;
       if(S1==58){S1=48;S2++;}
       if(S2==54){S1=S2=48;M1++;}
       if(M1==58){M1=48;M2++;}
       if(M2==54){M1=M2=48;H1++;}
       if(H1==58){H1=48;H2++;}
       if(H2==50&&H1==52){H1=H2=48;day++;}
       
}

void horloge_Display()  {
       if(day==1){Lcd_out(1, 10,"SUN");}
       if(day==2){Lcd_out(1, 10,"MON");}
       if(day==3){Lcd_out(1, 10,"TUE");}
       if(day==4){Lcd_out(1, 10,"WED");}
       if(day==5){Lcd_out(1, 10,"THR");}
       if(day==6){Lcd_out(1, 10,"FRI");}
       if(day==7){Lcd_out(1, 10,"SAT");}
       if(day == 8){day =1;}
       

    // Affiche les valeurs sur l'écran LCD
        Lcd_chr(1, 8, S1);
        Lcd_Out(1,3,":");
        Lcd_chr(1, 7, S2);
        Lcd_chr(1, 5, M1);
        Lcd_Out(1,6 ,":");
        Lcd_chr(1, 4, M2);
        Lcd_chr(1, 2, H1);
        Lcd_chr(1, 1, H2);

}