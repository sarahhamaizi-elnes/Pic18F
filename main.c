 #include "DHT11fonctionne.h"
#include "LCD.h"
#include "horloge.h"
// mode
sbit RemoteLocal at RA4_bit;
// DHT11 Pins declaration
sbit DHT11_PIN at RC3_bit;
sbit DHT11_PIN_Direction at TRISC3_bit;

// FLame sensor interruption pin shoul be high_priority
sbit flame at RB0_bit;
sbit flame_Dir at TRISB0_bit;

// output of ..........
sbit alarme0 at RA1_bit;
sbit alarme0_Dir at TRISA1_bit;

// output of ..........
sbit alarme1 at RA0_bit;
sbit alarme1_Dir at TRISA0_bit;

// output of ..........

sbit LED at RA2_bit;
sbit LED_Dir at TRISA2_bit;

char state = '0' ;                         // differnt states 0: idle, 1: local
char stateTamp;                         // differnt states 0: idle, 1: local

char overflow;              // number of overflow TMR0
char feuDetecte     = 0 ;
char gazDetecte     = 0 ;
char motionDetecte  = 0 ;
char Localboutton   = 0 ;
char stateboutton   = 0 ;
char RB4Interrupt  = 0 ;
char TamponRB4Interrupt  ;
char TMR30Finish  =0;

char TMR30First =0;

void interrupt(){
// check if INT1 flag caused the interrupt / RB1 INTERRUPTION
   if(INTCON3.INT2IF){

       INTCON3.INT2IF        = 0;       // clear flag of RB1 interrupt
    }

// check if INT1 flag caused the interrupt / RB1 INTERRUPTION
   else if(INTCON3.INT1IF){
       gazDetecte            = 1 ;
       INTCON3.INT1IF        = 0;       // clear flag of RB1 interrupt
    }
// check if INT0 flag caused the interrupt / RB0 INTERRUPTION
   else if(INTCON.INT0IF){
      feuDetecte             = 1 ;
      INTCON.INT0IF           = 0;      // clear flag of RBO interrupt
    }
    
     if(PIR1.RC1IF ){
       state = UART1_Read();


       PIR1.RC1IF        = 0;       // clear flag of RB1 interrupt
    }

}

void interrupt_low(){

    if(INTCON.TMR0IF){     // check if TMR0 caused the interrupt
       overflow = overflow-1;     // decrement overflow Number
       if (overflow == 0){
          TMR30Finish         = 1;
          T0CON.TMR0ON       = 0;
       }
       INTCON.TMR0IF        = 0;       // clear flag of RB1 interrupt
    }

  // check if RB4 caused the interrupt
   else if (INTCON.RBIF  ){
        while(PORTB.RB4 == 1);
        state            = '1' ;
        stateTamp            = '1' ;
        
        TamponRB4Interrupt = PORTB;            // clear flag of RB4 interrupt
        INTCON.RBIF = 0;
    }
    else if(PIR1.TMR1IF){ // check if INT0 flag caused the interrupt
          state   = '4';
          TMR1L = 0;         // reset initial value of the TMR1
          TMR1H = 0x80;
          //TMR1H = 0x00;

          PIR1.TMR1IF      = 0;
       }

}

void main() {

         TRISA.B4    = 1;  // configure RA4 as input to used it as switch of remote and local
         TRISB.B0    = 1;
         TRISB.B1    = 1;
         TRISB.B2    = 1;
         TRISB.B4    = 1;
         //TRISD.B0    = 0;
         //TRISE       = 0 ;
         TRISC       = 2;
         
         TRISC.B6    = 1;
         TRISC.B7    = 1;

         ANSELB      = 0x00;              // set PORTB as digital I/O
         ANSELA      = 0x00;              // set PORTB as digital I/O
         ANSELD      = 0x00;              // set PORTB as digital I/
         ANSELC      = 0x00;              // set PORTB as digital I
         ANSELE      = 0x00;              // set PORTB as digital I
         
         UART1_Init(9600);
         delay_ms(500);
         
         PIE1.RC1IE  = 1;
         IPR1.RCIP = 1;

//TMR1 configuration

        T1CON.T1SOSCEN      = 1;    // to configure secondary oscilltor of 32.786khz
        OSCCON2.SOSCGO      = 1;
        delay_ms(50);

        TMR1L              = 0;         // initial value of TMR1
        TMR1H              = 0x80;

        T1CON.TMR1CS0       = 0;        // external oscillator
        T1CON.TMR1CS1       = 1;

        T1CON.T1SYNC        = 0;

        T1CON.T1CKPS1       = 0 ;       // prescaler = 1
        T1CON.T1CKPS0       = 0 ;

        T1CON.TMR1ON        = 1 ;       // launch TMR1

        PIE1.TMR1IE         = 1;        // enable TMR1 interrupt
        PIR1.TMR1IF         = 0 ;

        IPR1.TMR1IP         = 0 ;      // TMR1 Interrupt priority (0: low, 1: hight)


// configuration de TIMER0 :: temporisation
         T0CON = 0x06 ;
         
         TMR0H = 0x00 ;
         TMR0L = 0x00;

         INTCON.TMR0IE = 1 ;      //TMR0 interrupt
         INTCON.TMR0IF = 0 ;
         INTCON2.TMR0IP = 0  ;    //  priority for TMR0 interrupt (0: low, 1: high)

 //ENABLE RB4 interrupt ON CHANGE
       INTCON.RBIE    = 1;
       INTCON.RBIF   = 0;
       IOCB.IOCB4    = 0 ;         //ENABLE PORT ON CHANGE  RB4
       INTCON2.RBIP  = 0 ;       // high priority for RB4-7

//ENABLE RB1 interrupt
      INTCON3.INT1IE = 1;       // enable external interrupt on RB0/INT0
      INTCON3.INT1IF = 0;       // clear the INT0 flag
      INTCON2.INTEDG1 = 1;      // interrupt on falling edge
      INTCON3.INT1IP = 1;        // high priority for RB1

//ENABLE RB2 interrupt
      INTCON3.INT2IE = 1;               // enable external interrupt
      INTCON3.INT2IF = 0;               // clear the flag
      INTCON2.INTEDG2 = 1;              // interrupt on falling edge
      INTCON3.INT2IP = 1 ;              // high priority for RB2

//ENABLE RB0 interrupt
      INTCON.INT0IE = 1;                // enable external interrupt on RB0/INT0
      INTCON.INT0IF = 0;                // clear the INT0 flag
      INTCON2.INTEDG0 = 0;              // interrupt on falling edge of RB0/INT0*/

// Pull up resistor of PORTB
      INTCON2.RBPU = 0;                 // pull up resistor (0 : enable 1: disable )
      WPUB.WPUB0 = 1;                    // pull up resistor for RB0 (0: disable, 1: enable);
      WPUB.WPUB1 = 1;                    // pull up resistor for RB1 (0: disable, 1: enable);
      WPUB.WPUB2 = 1;                    // pull up resistor for RB2 (0: disable, 1: enable);
      WPUB.WPUB3 = 1;                    // pull up resistor for RB3 (0: disable, 1: enable);
      WPUB.WPUB4 = 0;                    // pull up resistor for RB4 (0: disable, 1: enable);
      WPUB.WPUB5 = 1;                    // pull up resistor for RB5 (0: disable, 1: enable);
      WPUB.WPUB6 = 1;                    // pull up resistor for RB6 (0: disable, 1: enable);
      WPUB.WPUB7 = 1;                    // pull up resistor for RB7 (0: disable, 1: enable);

// mange prority of interrupts
      RCON.IPEN         = 1;            // enable high pririty
      INTCON.GIEH       = 1;              // enable global interrupts
      INTCON.GIEL       = 1;                 // enable

 // initiate LCD
      LCD_Init ();
      Lcd_Cmd(_LCD_CURSOR_OFF);        // cursor off
      Lcd_Cmd(_LCD_CLEAR);             // clear LCD*/
      delay_ms(100);
      Lcd_Out(2,1 , "Saraah");

  while(1){       // open loop
   //PORTE= state;


     switch(state)
       {
            case '0' :  // Idle : check the switch of remote/Local  connected to RA4
                 if ( RemoteLocal == 0){ // local
                   IOCB.IOCB4    = 1 ;                 // enable RB4 interrupt
                   INTCON.RBIE    = 1;

                   Lcd_Out(2,1 , "mode:  Local");
                    INTCON.TMR0IE = 1 ;                 //TMR0 interrupt
                   //T0CON.TMR0ON    = 1;
                 }
                 else {                    // remote
                  IOCB.IOCB4          = 0 ;          // Disable RB4 interrupt

                   Lcd_Out(2, 1, "mode: Remote");
                   INTCON.TMR0IE       = 0 ;          //TMR0 interrupt
                  //T0CON.TMR0ON    = 0;
                   overflow        = 7;
                 }
                 stateTamp = '0';

            break;

            case '1' :    //this mode is activated when RB4 interrupt is detected
                TMR0H           = 0x00 ;
                TMR0L           = 0x00;
                overflow        = 7;
                T0CON.TMR0ON    = 1;
                stateTamp = '1';
 
                Lcd_Cmd(_LCD_CLEAR);
                if (state != '4')  state           = '2';
            break;

            case '2' :    //display the DHT11measure in the LCD: this mode is activated automatically when mode 1 is finish
                delay_ms(500);
                if (stateTamp =='4')   Lcd_Cmd(_LCD_CLEAR);
                DHT11_DISPLAY();
                if (TMR30Finish == 1){
                    state = '3';
                    TMR30Finish = 0;  
                }
                if  (RemoteLocal ==1)  {
                     state = '0';
                     Lcd_Cmd(_LCD_CLEAR);
                }
                stateTamp = '2';

           break;
             
           case '3' :   //clear LCD before return to state 0
                  Lcd_Cmd(_LCD_CLEAR);
                  stateTamp = '3';
                  state = '0';

            break;
            
            case '4' :   //clock    TMR1 inerrupt every 1 s
                  DHT11_READ();
                  horloge_compute();
                  if  (stateTamp != '2'){
                    horloge_Display();
                  }
                  state =stateTamp;
                  stateTamp = '4';
            break;

            case '5' :   //mode remote
                 DHT11_READ();
                 UART1_Write(T_byte1) ;
                UART1_Write(RH_byte1) ;
                 state = '0'  ;

            break;

            case '6' :   //mode remote
                 Lcd_Cmd(_LCD_CLEAR);
                 state = '0';
            break;

             default:
                      state = state;
             break;


      }
  }
}