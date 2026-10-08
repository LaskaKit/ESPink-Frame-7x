[🇨🇿 Česká verze](README_CZ.md)

![ESPink-Frame-7.x](img/laskakit-live-7-x-pro-e-paper-stavebnice-s-wifi-pro---zivy-obraz--1.jpg)

# LaskaKit ESPink-Frame-7.x — low-power ESP32-S3 board for large 7.5" and 7.3" ePaper displays

**ESPink-Frame-7.x** is a universal, ultra-low-power board built around the **ESP32-S3** for large 800 × 480 px ePaper displays. It drives the black & white and four-colour **7.5"** panels as well as the six-colour **7.3" Spectra 6**. It is the heart of the [LaskaKit Live-7.x-Pro](https://www.laskakit.cz/laskakit-live-7-x-pro-e-paper-stavebnice-s-wifi-pro---zivy-obraz-/) kit — a Wi-Fi photo frame / dashboard for the [Živý Obraz](https://zivyobraz.eu/) service — but it is just as useful for your own projects.

The board is designed for long battery life: the display, sensors, the μŠup connectors and the microSD card are all powered from a separate regulator that you switch off with a single GPIO before going to deep sleep. The battery is charged directly from USB-C, which is also used for programming via the CH9102F converter.

---

## Main features

- **ESP32-S3-WROOM-1U-N16R8** — 16 MB Flash, 8 MB PSRAM, Wi-Fi and Bluetooth LE, **U.FL connector for an external antenna** (the board sits behind a large display, so an external antenna gives much better range)
- **Two display connectors:**
  - 24-pin FPC for 7.5" panels (GDEY075T7, GDEM075F52)
  - 50-pin FPC for the 7.3" Spectra 6 panel (GDEP073E01)
- **Switchable peripheral power** — GPIO47 switches the `VSENSOR` rail (RT9080 regulator) that feeds the ePaper, μŠup connectors, the onboard SHT40, the microSD card and the expansion header
- **Li-Po battery charging** from USB-C (TP4054, approx. 0.56 A), JST-PH 2.0 battery connector, charge LED
- **Battery voltage measurement** via a voltage divider on GPIO9
- **Power switch** disconnecting the battery
- **Onboard SHT40** temperature and humidity sensor
- **μŠup connectors** — 1× I2C (compatible with Adafruit Stemma QT / SparkFun Qwiic) and 1× SPI
- **microSD card slot** with card-detect
- **3 user buttons** (UP / PUSH / DOWN) + RESET
- **1×15 expansion header** with unused GPIOs
- 4× M2.5 stand-offs for mounting

---

## Supported displays

| Display | Size | Resolution | Colours | Refresh | `DISPLAY` jumper |
|---|---|---|---|---|---|
| [Good Display GDEY075T7](https://www.laskakit.cz/good-display-gdey075t7-7-5--800x480-epaper-displej-grayscale/) | 7.5" | 800 × 480 | black & white, 4 grey levels | fast (~3.5 s), partial refresh | LEFT (0.44 Ω) |
| [Good Display GDEM075F52](https://www.laskakit.cz/good-display-gdem075f52-7-5--800x480-epaper-displej-4-barvy/) | 7.5" | 800 × 480 | black, white, red, yellow | slow (~22 s) | open (2.2 Ω, default) |
| [Good Display GDEP073E01](https://www.laskakit.cz/good-display-gdep073e01-7-3--800x480-epaper-barevny-displej/) | 7.3" | 800 × 480 | Spectra 6 — black, white, red, yellow, blue, green | slow (~19 s) | RIGHT (0.1 Ω) |

> **Important:** the `DISPLAY` solder jumper sets the current-sense resistor of the ePaper boost converter. Set it according to the display you use (see the table above and the schematic).

---

## Specifications

| Parameter | Value |
|---|---|
| MCU | ESP32-S3 (ESP32-S3-WROOM-1U-N16R8), 16 MB Flash, 8 MB PSRAM |
| Wireless | Wi-Fi 2.4 GHz, Bluetooth LE, external antenna via U.FL |
| Supply | USB-C 5 V or 1-cell Li-Po 3.7 V (JST-PH 2.0) |
| Charging | TP4054, approx. 0.56 A |
| USB-UART | CH9102F |
| Regulators | 2× RT9080-33 (always-on 3.3 V for the MCU, switchable `VSENSOR` 3.3 V for peripherals) |
| Onboard sensor | SHT40 (I2C, address 0x44) |
| Logic level | 3.3 V |
| Dimensions | approx. 185 × 120 × 8 mm (board with display) |
| Weight | 140 g |
| Hardware revision | v1.2 |

---

## Pinout

### ePaper (GxEPD2 standard)

| Signal | GPIO |
|---|---|
| MOSI / SDI | 11 |
| CLK / SCK | 12 |
| CS | 10 |
| DC | 48 |
| RST | 45 |
| BUSY | 38 |
| Peripheral power `VSENSOR` (HIGH = ON) | 47 |

### I2C μŠup + onboard SHT40

| Signal | GPIO |
|---|---|
| SDA | 42 |
| SCL | 2 |

### SPI μŠup

| Signal | GPIO |
|---|---|
| CS | 46 |
| MOSI | 3 |
| CLK | 14 |
| MISO | 21 |

### microSD card (shares SPI with the ePaper)

| Signal | GPIO |
|---|---|
| CS | 17 |
| MOSI | 11 |
| CLK | 12 |
| MISO | 13 |
| Card detect | 16 |

### Buttons (active LOW, external 10k pull-up)

| Button | GPIO |
|---|---|
| UP | 39 |
| PUSH | 40 |
| DOWN | 41 |

> GPIO39–41 are not RTC GPIOs, so the UP / PUSH / DOWN buttons **cannot wake the ESP32-S3 from deep sleep**. Wake the board with the RESET button or a timer.

### Battery voltage

| Signal | GPIO |
|---|---|
| Divider 1 MΩ / 1.3 MΩ | 9 |

```cpp
float vBat = analogReadMilliVolts(9) * 1.769 / 1000.0; // battery voltage in V
```

### Expansion header JP1 (1×15, 2.54 mm)

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | IO1 | 9 | IO15 |
| 2 | IO37 | 10 | IO7 |
| 3 | IO36 | 11 | IO6 |
| 4 | IO35 | 12 | IO5 |
| 5 | IO20 | 13 | IO4 |
| 6 | IO19 | 14 | GND |
| 7 | IO8 | 15 | VSENSOR (switched 3.3 V) |
| 8 | IO18 | | |

> IO35, IO36 and IO37 are used internally by the octal PSRAM of the N16R8 module — **do not use them**.

---

## Živý Obraz

The board is supported by [Živý Obraz](https://zivyobraz.eu/) — show your photos, a calendar (Google, Outlook, Apple), weather forecast, values from a weather station or home automation without writing any code.

1. Open the [web installer](https://zivyobraz.eu/?page=instalace).
2. Select the board **LaskaKit ESPink 2.x, 3.5 and 3.6** and your display type (GDEY075T7, GDEM075F52 or GDEP073E01).
3. Flash the firmware, connect to Wi-Fi and follow the [documentation](https://wiki.zivyobraz.eu/).

---

## Example code

Arduino examples are in the [SW](SW) folder. They use the [GxEPD2](https://github.com/ZinggJM/GxEPD2) library — select your display with the `#define DISP_...` at the top of the sketch.

| Example | Description |
|---|---|
| [GxEPD2_SHT40](SW/GxEPD2_SHT40) | Temperature, humidity and battery voltage on the display, then deep sleep |
| [GDEP073E01_GxEPD2](SW/GDEP073E01_GxEPD2) | Six-colour Spectra 6 display with GxEPD2 |
| [GDEP073E01_ESP32](SW/GDEP073E01_ESP32) | Six-colour Spectra 6 display with the Good Display driver (image from `image.h`) |
| [Buttons](SW/Buttons) | Reading the UP / PUSH / DOWN buttons |
| [I2C_test](SW/I2C_test) | I2C bus scan |
| [SD_Test](SW/SD_Test) | microSD card test |

Don't forget to set `GPIO47` HIGH before talking to the display, sensors or SD card, and LOW before going to deep sleep.

---

## Repository contents

- [HW](HW) — schematics (PDF)
- [3D](3D) — 3D model of the board (STEP, Fusion 360)
- [Production](Production) — production data (Gerber, BOM, CPL)
- [SW](SW) — example code
- [Version.md](Version.md) — hardware revision history

---

## Where to buy

- Board: [LaskaKit ESPink-Frame-7.x e-Paper Board](https://www.laskakit.cz/laskakit-espink-frame-7-x-e-paper-board/)
- Complete kit with display, battery, antenna and frame: [LaskaKit Live-7.x-Pro](https://www.laskakit.cz/laskakit-live-7-x-pro-e-paper-stavebnice-s-wifi-pro---zivy-obraz-/)
- Sensors with μŠup connector: [SHT40](https://www.laskakit.cz/laskakit-sht40-senzor-teploty-a-vlhkosti-vzduchu/), [BME280](https://www.laskakit.cz/arduino-senzor-tlaku--teploty-a-vlhkosti-bme280/), [SCD41](https://www.laskakit.cz/laskakit-scd41-senzor-co2--teploty-a-vlhkosti-vzduchu/)
