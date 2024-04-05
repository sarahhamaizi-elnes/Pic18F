#include "DHT11fonctionne.h"
#include "LCD.h"
 char CheckDHT11 = 0  ;
 char RH_byte1 = 0 ;
 char RH_byte2 = 0 ;
 char T_byte1 = 0 ;
 char T_byte2 = 0 ;
 unsigned Temp = 0 ;
 unsigned RH = 0 ;
 unsigned dataDht11 = 0 ;
 char i = 0 ;
 char j = 0 ;
 char NoResponse = 0 ;
 char error ;
 unsigned Sum ;

 //////////////////////////////
 char ReadData(){
      for(j = 0; j < 8; j++){
            while(!PORTE.B0); //Wait until PORTD.F0 goes HIGH
            delay_us(30);
            if(PORTE.B0 == 0)
            dataDht11&= ~(1<<(7 - j)); //Clear bit (7-b)
            else {dataDht11|= (1 << (7 - j)); //Set bit (7-b)
            while(PORTE.B0);} //Wait until PORTD.F0 goes LOW
      }
      return dataDht11;
 }
 //////////////////////////////

void DHT11_READ() {
   //Lcd_Cmd(_LCD_CURSOR_OFF); // cursor off

   //Lcd_Cmd(_LCD_CLEAR); // clear LCD

   TRISE.B0 = 0; //Configure RD0 as output
   PORTE.B0 = 0; //RD0 sends 0 to the sensor
   delay_ms(18);
   PORTE.B0 = 1; //RD0 sends 1 to the sensor
   delay_us(30);
   TRISE.B0 = 1; //Configure RD0 as input

   //////////////////////////////

   CheckDHT11 = 0;
   delay_us(40);
   
   if (PORTE.B0 == 0){
     delay_us(80);
     if (PORTE.B0 == 1)
        CheckDHT11 = 1;
     delay_us(40);
   }

   if(CheckDHT11 == 1){
           RH_byte1 = ReadData();
           RH_byte2 = ReadData();
           T_byte1 = ReadData();
           T_byte2 = ReadData();
           Sum = ReadData();
           if(Sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2) & 0XFF)){
               error = 0;
           }
           else error = 1;
   }

}

void DHT11_DISPLAY() {
     //if (CheckDHT11 == 1){
          if (error =1){
               if (NoResponse = 1){
                 Lcd_Cmd(_LCD_CLEAR);
                 NoResponse = 0;
               }
               Temp  = T_byte1;
               RH    = RH_byte1;
               Lcd_Out(1, 6, "Temp: C");
               Lcd_Out(2, 2, "Humidity: %");
               LCD_Chr(1, 12, 48 + ((Temp / 10) % 10));
               LCD_Chr(1, 13, 48 + (Temp % 10));
               LCD_Chr(2, 12, 48 + ((RH / 10) % 10));
               LCD_Chr(2, 13, 48 + (RH % 10));
           }
           else{
              Lcd_Cmd(_LCD_CURSOR_OFF); // cursor off
               Lcd_Cmd(_LCD_CLEAR); // clear LCD
               Lcd_Out(1, 1, "Check sum error"); }
    //}
     //else {
         //Lcd_Out(1, 3, "No response");
        // Lcd_Out(2, 1, "from the sensor");
        // NoResponse = 1;

   //}
   //delay_ms(1000);

 }