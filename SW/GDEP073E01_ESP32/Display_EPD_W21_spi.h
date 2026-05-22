#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "Arduino.h"

//IO settings
//SCK--GPIO23(SCLK)
//SDIN---GPIO18(MOSI)

#define  MOSI    11
#define SCK     12
#define CS    10
#define DC    48 
#define RST   45  
#define BUSY  38
#define POWER 47

#define isEPD_W21_BUSY digitalRead(BUSY)  //BUSY
#define EPD_W21_RST_0 digitalWrite(RST,LOW)  //RES
#define EPD_W21_RST_1 digitalWrite(RST,HIGH)
#define EPD_W21_DC_0  digitalWrite(DC,LOW) //DC
#define EPD_W21_DC_1  digitalWrite(DC,HIGH)
#define EPD_W21_CS_0 digitalWrite(CS,LOW) //CS
#define EPD_W21_CS_1 digitalWrite(CS,HIGH)


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);


#endif 