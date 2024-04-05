#ifndef DHT11fonctionne_h
#define DHT11fonctionne_h
#include "DHT11fonctionne.h"

// Définitions des fonctions


 // End LCD module connections
 // DHT11 sensor connection (here data pin is connected to pin RB0)

extern char CheckDHT11, error   ;
extern  char T_byte1, T_byte2, RH_byte1, RH_byte2, Ch ;
extern  char   j;
extern  unsigned Temp, RH, Sum, dataDht11 ;

extern char ReadData();
extern void DHT11_READ() ;
extern void DHT11_DISPLAY() ;

#endif