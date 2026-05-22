/* Display test for LaskaKit ESPink-Frame-7x
 * 
 * Board with display:   LaskaKit ESPink-Frame-7x   https://www.laskakit.cz/laskakit-espink-frame-7x/
 *
 * Email:podpora@laskakit.cz
 * Web:laskakit.cz
 */

// ! For the board settings see Display_EPD_W21_spi.h


#include <SPI.h>
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "image.h"
// 定义颜色
#define BLACK  0x00
#define WHITE  0x11
#define GREEN  0x66
#define BLUE   0x55
#define RED    0x33
#define YELLOW 0x22

// 屏幕分辨率
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480
#define GRID_SIZE     8  // 8x8 棋盘
#define BLOCK_WIDTH   (SCREEN_WIDTH / GRID_SIZE)   // 每个格子宽度：100
#define BLOCK_HEIGHT  (SCREEN_HEIGHT / GRID_SIZE)  // 每个格子高度：60

// 棋盘数据数组
uint8_t chessboard[GRID_SIZE][GRID_SIZE];

// 初始化棋盘（黑白交替）
void initChessboard() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            chessboard[i][j] = ((i + j) % 2 == 0) ? WHITE : BLACK;  // 黑白交替
        }
    }
    Serial.println("Chessboard initialized");
}

// 显示整个棋盘（全屏刷新，1 字节 2 像素）
void displayChessboard() {
    uint8_t* rowBuffer = (uint8_t*)malloc(SCREEN_WIDTH / 2);
    if (rowBuffer == NULL) {
        Serial.println("Failed to allocate rowBuffer");
        return;
    }

    EPD_W21_WriteCMD(DTM);

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        int gridY = y / BLOCK_HEIGHT;
        for (int x = 0; x < SCREEN_WIDTH; x += 2) {
            int gridX = x / BLOCK_WIDTH;
            uint8_t color1 = chessboard[gridY][gridX];
            uint8_t color2 = (x + 1 < SCREEN_WIDTH) ? chessboard[gridY][(x + 1) / BLOCK_WIDTH] : color1;
            rowBuffer[x / 2] = (color1 << 4) | (color2 & 0x0F);
        }
        for (int i = 0; i < SCREEN_WIDTH / 2; i++) {
            EPD_W21_WriteDATA(rowBuffer[i]);
        }
        if (y % 100 == 0) {
            yield();
        }
    }

    free(rowBuffer);

    // 后续流程（与 EPD_Display_White 一致）
    EPD_W21_WriteCMD(PON);
    lcd_chkstatus();
    EPD_W21_WriteCMD(BTST2);
    EPD_W21_WriteDATA(0x6F);
    EPD_W21_WriteDATA(0x1F);
    EPD_W21_WriteDATA(0x17);
    EPD_W21_WriteDATA(0x49);
    EPD_W21_WriteCMD(DRF);
    EPD_W21_WriteDATA(0x00);
    lcd_chkstatus();
    EPD_W21_WriteCMD(POF);
    EPD_W21_WriteDATA(0x00);
    lcd_chkstatus();

    Serial.println("Chessboard displayed");
}

// 局部刷新函数（确保颜色正确写入）
void EPD_PartialWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color) {
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;

    EPD_W21_WriteCMD(CMD_PARTIAL_WINDOW);

    EPD_W21_WriteDATA((x >> 8) & 0x03);
    EPD_W21_WriteDATA(x & 0xFF);
    EPD_W21_WriteDATA((x_end >> 8) & 0x03);
    EPD_W21_WriteDATA(x_end & 0xFF);
    EPD_W21_WriteDATA((y >> 8) & 0x03);
    EPD_W21_WriteDATA(y & 0xFF);
    EPD_W21_WriteDATA((y_end >> 8) & 0x03);
    EPD_W21_WriteDATA(y_end & 0xFF);

    EPD_W21_WriteDATA(0x01);

    lcd_chkstatus();

    EPD_W21_WriteCMD(DTM);
    EPD_W21_WriteDATA(0x00); // 2-bit per pixel, DDX = 1

    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i += 2) { // 1 字节控制 2 个像素
            uint8_t data;
            if (i + 1 < width) {
                // 两个像素都填充相同颜色
                data = (color << 4) | (color & 0x0F);
            } else {
                // 只剩一个像素
                data = (color << 4);
            }
            EPD_W21_WriteDATA(data);
        }
        if (j % 10 == 0) {
            yield();
        }
    }

    lcd_chkstatus();

    EPD_W21_WriteCMD(PON);
    lcd_chkstatus();
    EPD_W21_WriteCMD(BTST2);
    EPD_W21_WriteDATA(0x6F);
    EPD_W21_WriteDATA(0x1F);
    EPD_W21_WriteDATA(0x17);
    EPD_W21_WriteDATA(0x49);
    EPD_W21_WriteCMD(DRF);
    EPD_W21_WriteDATA(0x00);
    lcd_chkstatus();
    EPD_W21_WriteCMD(POF);
    EPD_W21_WriteDATA(0x00);
    lcd_chkstatus();

    Serial.println("Partial window updated");
}

// 手动实现局部刷新（调用 EPD_PartialWindow）
void manualPartialUpdate(int gridX, int gridY, uint8_t color) {
    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
        chessboard[gridY][gridX] = color;
    }

    uint16_t x = gridX * BLOCK_WIDTH;
    uint16_t y = gridY * BLOCK_HEIGHT;

    EPD_PartialWindow(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, color);
    Serial.println("Manual partial update completed");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Setup started");

    // turn on power to display
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   // turn the Display Power on (HIGH is the voltage level)
    delay(100);   

    pinMode(BUSY, INPUT);  // BUSY
    pinMode(RST, OUTPUT); // RES
    pinMode(DC, OUTPUT); // DC
    pinMode(CS, OUTPUT); // CS
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
    SPI.begin (SCK, -1, MOSI, CS);  
    Serial.println("SPI initialized");
    EPD_init(); //Full screen refresh initialization.
    PIC_display(gImage_1);//To Display one image using full screen refresh.
    EPD_sleep();//Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(5000); //Delay for 5s.


    initChessboard();
}

void loop() {
    static int state = 0;

    if (state == 0) {
        Serial.println("Displaying initial chessboard");
        EPD_init();
        displayChessboard();
        EPD_sleep();
        state = 1;
        delay(5000);
    }
    else if (state == 1) {
        Serial.println("Updating (2, 2) to RED");
        EPD_init();
        manualPartialUpdate(2, 2, RED);
        EPD_sleep();
        state = 2;
        delay(5000);
    }
    else if (state == 2) {
        Serial.println("Updating (4, 4) to YELLOW");
        EPD_init();
        manualPartialUpdate(4, 4, YELLOW);
        EPD_sleep();
        state = 3;
        delay(5000);
    }
    else if (state == 3) {
        Serial.println("Clearing display");
        EPD_init();
        PIC_display_Clear();
        EPD_sleep();
        Serial.println("Display cleared");
        state = 4;
        delay(2000);
    }
    else {
        Serial.println("Program finished");
        while (1) {
            yield();
        }
    }
}