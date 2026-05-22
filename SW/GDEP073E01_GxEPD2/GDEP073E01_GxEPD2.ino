/* Display test for Good Display GDEP073E01
 * example from GxEPD2 library is used
 * 
 * Board with display:   LaskaKit ESPink-Frame-7x   https://www.laskakit.cz/laskakit-espink-frame-7x/
 * 
 * Libraries:
 * EPD library: https://github.com/ZinggJM/GxEPD2_4G
 *
 * Email:podpora@laskakit.cz
 * Web:laskakit.cz
 */

/* Display test for Good Display GDEP073E01
 * Board: LaskaKit ESPink-Frame-7x
 * Display: GxEPD2_730c_GDEP073E01, 800x480, 7-color
 */

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_7C.h>
#include <Adafruit_GFX.h>
#include "bitmaps/Bitmaps7c800x480.h" // 7.3" 7-color

// SPI pins:
// MOSI/SDI  11
// CLK/SCK   12
// CS        10

#define DC     48
#define RST    45
#define BUSY   38
#define POWER  47

GxEPD2_7C<GxEPD2_730c_GDEP073E01, GxEPD2_730c_GDEP073E01::HEIGHT / 4> display(
  GxEPD2_730c_GDEP073E01(SS, DC, RST, BUSY)
); // 800x480 7-color

void setup()
{
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("setup");

  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
  Serial.println("Display power ON");
  delay(1000);

  display.init(115200);

  drawDemoBitmap();
  waitDisplayReady();

  Serial.println("Extra delay 3s");
  delay(3000);

  draw7colors();
  delay(4000);

  display.powerOff();

  Serial.println("setup done");
}

void loop()
{
}

void drawDemoBitmap()
{
  Serial.println("drawDemoBitmap");
  display.epd2.drawDemoBitmap(
    Bitmap7c800x480,
    0, 0,
    0,
    800, 480,
    0,
    false,
    true
  );
}

void draw7colors()
{
  Serial.println("draw7colors");

  display.setRotation(0);

  uint16_t h = display.height() / 6;

  display.setFullWindow();

  display.firstPage();
  do
  {
    display.fillRect(0, 0 * h, display.width(), h, GxEPD_BLACK);
    display.fillRect(0, 1 * h, display.width(), h, GxEPD_WHITE);
    display.fillRect(0, 2 * h, display.width(), h, GxEPD_GREEN);
    display.fillRect(0, 3 * h, display.width(), h, GxEPD_BLUE);
    display.fillRect(0, 4 * h, display.width(), h, GxEPD_RED);
    display.fillRect(0, 5 * h, display.width(), h, GxEPD_YELLOW);
  }
  while (display.nextPage());
}
void waitDisplayReady()

{

  Serial.println("Waiting for BUSY...");

  unsigned long start = millis();

  while (digitalRead(BUSY) == LOW)   // pokud by nefungovalo, změnit LOW na HIGH

  {

    delay(100);

    if (millis() - start > 120000)

    {

      Serial.println("BUSY wait timeout");

      return;

    }

  }

  Serial.println("BUSY OK");

}