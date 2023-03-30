#include <M5Core2.h>
#include "display-overview.h"

bool RefreshDisplay = true;
uint DisplayPage = 1;
uint DisplayMaxPage = 2;
uint LCDBrightness = 2900;
uint FontNumber = 2;
uint TextSize = 1;

DisplayOverview displayOverview;

void displayInit(void) {
  //M5.Axp.SetLcdVoltage(LCDBrightness); 
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  RefreshDisplay = true;
}

void displayDetail(void) {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(TextSize);

  M5.Lcd.drawNumber(FontNumber, 10,10);
  M5.Lcd.drawString(" Detail ", 50,90, FontNumber);
}

void displayLoop(void) {
  switch(DisplayPage) {
    default:
    case 1:
      displayOverview.Loop(RefreshDisplay);
      break;
    case 2:
      displayDetail();
      break;
  }
  RefreshDisplay = false;
}

void handleInput(void) {
  if (M5.BtnA.wasPressed()) {
    DisplayPage++;
    if (DisplayPage > DisplayMaxPage) DisplayPage = 1;
    RefreshDisplay = true;
  }
  if (M5.BtnC.wasPressed()) {
    DisplayPage--;
    if (DisplayPage < 1) DisplayPage = DisplayMaxPage;
    RefreshDisplay = true;
  }
  if (M5.BtnB.wasPressed()) {
    /*
    TextSize++;
    if (TextSize > 7) TextSize = 1;
    M5.Lcd.setTextSize(TextSize);
    */
    FontNumber++;
  }
}

/////////////////////////////////////////////////////////////////////////
void setup() {
  M5.begin(true,false,true,true);  // LCD, SD, Serial, I2C, MBusMode
  //M5.IMU.Init();

  displayInit();
}

/////////////////////////////////////////////////////////////////////////
void loop() {
  M5.update();
  handleInput();
  displayLoop();
}
