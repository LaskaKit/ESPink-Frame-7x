/* I2C test for LaskaKit ESPink-Frame-7x
 * 
 * Board:   LaskaKit ESPink-Frame-7x   https://www.laskakit.cz/laskakit-espink-frame-7x/
 *
 * Libraries:
 * EPD library: https://github.com/ZinggJM/GxEPD2
 * 
 * Email:podpora@laskakit.cz
 * Web:laskakit.cz
 */

#include <Arduino.h>

//#include <GxEPD2_4C.h>
#include <GxEPD2_4G_4G.h>
#include <GxEPD2_4G_BW.h>
//#include <GxEPD2_7C.h>

#include <Fonts/FreeMonoBold12pt7b.h>
#include <Wire.h>
#include <driver/rtc_io.h>

#define TIME_TO_SLEEP 5           // Time ESP32 will go to sleep (in seconds)
#define uS_TO_S_FACTOR 1000000ULL // Conversion factor for micro seconds to seconds
RTC_DATA_ATTR int bootCount = 0;  // Variable for keeping number of wakeups

//MOSI/SDI    11
//CLK/SCK     12
//SS/CS       10
#define DC    48 
#define RST   45  
#define BUSY  38 
#define POWER 47
#define SDA   42
#define SCL   2

#define TEXT_PADDING 30

GxEPD2_4G_4G<GxEPD2_750_GDEY075T7, GxEPD2_750_GDEY075T7::HEIGHT / 2> display(GxEPD2_750_GDEY075T7(/*CS=5*/ SS, /*DC=*/ DC, /*RST=*/ RST, /*BUSY=*/ BUSY)); // GDEY075T7  800x480, UC8179 (GD7965), (FPC-C001 20.08.20)
//GxEPD2_4C<GxEPD2_750c_GDEM075F52, GxEPD2_750c_GDEM075F52::HEIGHT / 2> display(GxEPD2_750c_GDEM075F52(SS, DC, RST, BUSY)); // GDEM075F52 800x480, JD79665AA (FPC-8619)
//GxEPD2_7C < GxEPD2_730c_GDEP073E01, GxEPD2_730c_GDEP073E01::HEIGHT / 4 > display(GxEPD2_730c_GDEP073E01(SS, DC, RST, BUSY)); // GDEP073E01 800x480 7-color, (E350911HF 94V-0 F-6 ROHS 24141)

void start_sleep()
{
  gpio_hold_en((gpio_num_t)POWER); // Hold Epaper on during sleep
  gpio_deep_sleep_hold_en();                   // Hold Epaper on during sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void print_middle_line(char *text, uint16_t padding)
{
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor((display.width() - tbw) / 2, padding);
  display.print(text);
}

void I2C_test()
{
  byte error, address;
  int nDevices = 0;
  char text[40];
  display.fillScreen(GxEPD_WHITE);
  sprintf(text, "Scanning for I2C devices...");
  print_middle_line(text, TEXT_PADDING);
  for (address = 0x01; address < 0x7f; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      nDevices++;
      sprintf(text, "I2C device found at address 0x%d", address);
      print_middle_line(text, TEXT_PADDING * nDevices + TEXT_PADDING);
    }
    else if (error != 2)
    {
      if (nDevices)
      {
        sprintf(text, "Error %d at address 0x%d", error, address);
        print_middle_line(text, TEXT_PADDING * nDevices + 2 * TEXT_PADDING);
      }
      else
      {
        sprintf(text, "Error %d at address 0x%d", error, address);
        print_middle_line(text, TEXT_PADDING * 2);
      }
    }
  }
  if (nDevices == 0)
  {
    sprintf(text, "No I2C devices found");
    print_middle_line(text, TEXT_PADDING * 2);
    sprintf(text, "Next scanning in 5 seconds...");
    print_middle_line(text, TEXT_PADDING * 3);
    sprintf(text, "Went to sleep %d times", bootCount);
    print_middle_line(text, TEXT_PADDING * 4);
    sprintf(text, "Going to sleep");
    print_middle_line(text, TEXT_PADDING * 5);
  }
  else
  {
    sprintf(text, "Next scanning in 5 seconds...");
    print_middle_line(text, TEXT_PADDING * nDevices + 2 * TEXT_PADDING);
    sprintf(text, "Went to sleep %d times", bootCount);
    print_middle_line(text, TEXT_PADDING * 3);
    sprintf(text, "Going to sleep");
    print_middle_line(text, TEXT_PADDING * nDevices + 4 * TEXT_PADDING);
  }
  display.display(true);
}

void setup()
{
  Serial.begin(115200);
  // turn on power to display
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);   // turn the Display on (HIGH is the voltage level)
  Serial.println("Display power ON");
  delay(500);   
  
  if (bootCount)                         // If first boot, redraw
  {
    display.init(0, false);
  }
  else
  {
    display.init(0, true);
  }
  display.setRotation(0);
  display.fillScreen(GxEPD_WHITE);
  display.setPartialWindow(0, 0, display.width(), display.height()); // Set display window for fast update
  Wire.begin (SDA, SCL);

  I2C_test();
  bootCount++; // Increment boot number every reboot
  start_sleep();
}

void loop()
{
}