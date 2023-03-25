#include <M5Core2.h>
#include "overview.h"

bool RefreshDisplay = true;
uint DisplayPage = 1;
uint DisplayMaxPage = 2;
uint LCDBrightness = 2900;
uint FontNumber = 1;
uint TextSize = 2;

int ACInputWatt = 14;
String InverterStatus = "Inverting";
int ACOutputWatt = 38;
int BatteryPercent = 100;
int PVInputWatt = 586;
int DCOutputWatt = 18;

void displayInit(void) {
  //M5.Axp.SetLcdVoltage(LCDBrightness); 
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(TextSize);
  RefreshDisplay = true;
}

void displayOverview(void) {
  char buffer[100];

  if (RefreshDisplay) {
    M5.Lcd.drawBitmap(0, 0, 320, 240, overview);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(231,76,60));
    M5.Lcd.drawString("AC Input", 52,51, FontNumber);
    M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(46,204,113));
    M5.Lcd.drawString("AC Output", 267,51, FontNumber);
    M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(244,179,80));
    M5.Lcd.drawString("PV Solar", 37,161, FontNumber);
    M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(26,188,156));
    M5.Lcd.drawString("DC Output", 273,213, FontNumber);
    RefreshDisplay = false;
  }

  M5.Lcd.setTextSize(1);
  // Inverter Status
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(71,137,208));
  sprintf(buffer, "%s", InverterStatus);
  M5.Lcd.drawString(buffer, 160,50, FontNumber);

  M5.Lcd.setTextSize(2);
  // Paint AC Input
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(192,57,43));
  sprintf(buffer, "%dW", ACInputWatt);
  M5.Lcd.drawString(buffer, 52,68, FontNumber);
  // Paint AC Output
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(39,174,96));
  sprintf(buffer, "%dW", ACOutputWatt);
  M5.Lcd.drawString(buffer, 267,68, FontNumber);
  // Paint PV Input
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(243,156,18));
  sprintf(buffer, "%dW", PVInputWatt);
  M5.Lcd.drawString(buffer, 37,180, FontNumber);
  // Paint DC Output
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(22,161,133));
  sprintf(buffer, "%dW", DCOutputWatt);
  M5.Lcd.drawString(buffer, 273,230, FontNumber);

  
  M5.Lcd.setTextSize(3);
  // Paint Battery Percent
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(71,137,208));
  sprintf(buffer, "%d%%", BatteryPercent);
  M5.Lcd.drawString(buffer, 160,184, FontNumber);

}

void displayDetail(void) {
  M5.Lcd.drawString(" Detail ", 50,90, FontNumber);
}

void displayLoop(void) {
  switch(DisplayPage) {
    case 1:
      displayOverview();
      break;
    case 2:
      displayDetail();
      break;
    default:
      displayOverview();
      break;
  }
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
    TextSize++;
    if (TextSize > 7) TextSize = 1;
    M5.Lcd.setTextSize(TextSize);
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
