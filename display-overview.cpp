#include "display-overview.h"
#include "overview.h"

uint HEADER_HEIGHT = 20;
uint PANEL_RADIUS = 5;

uint FLOW_HEIGHT = 3;
uint BALL_HEIGHT = FLOW_HEIGHT + 1;
uint FLOW_OUTLINE_COLOR = M5.Lcd.color565(9,121,197);
uint FLOW_FILL_COLOR = M5.Lcd.color565(26,171,241);
uint FLOW_BALL_COLOR = M5.Lcd.color565(208,208,208);

int ACInputMaxWatt = (30*120);
int ACOutputMaxWatt = (3000);
int PVInputMaxWatt = (24*(15+20));
int DCOutputMaxWatt = (24*70);
int BatteryMaxAmps = (70);

panelStruct ACIN = { 1,20, 90,100, "AC Input", 2,4, M5.Lcd.color565(192,57,43), M5.Lcd.color565(231,76,60), TFT_WHITE};
panelStruct ACOUT = { 229,20, 90,100, "AC Output", 2,4, M5.Lcd.color565(39,174,96), M5.Lcd.color565(46,204,113), TFT_WHITE};
panelStruct PVIN = { 1,139, 90,100, "PV Input", 2,4, M5.Lcd.color565(213,165,8), M5.Lcd.color565(243,185,18), TFT_WHITE};
panelStruct DCOUT = { 229,139, 90,100, "DC Output", 2,4, M5.Lcd.color565(22,161,133), M5.Lcd.color565(26,188,156), TFT_WHITE};
panelStruct INVERTER = { 120,1, 80,120, "Inverter", 2,4, M5.Lcd.color565(15,151,232), M5.Lcd.color565(251,97,19), TFT_WHITE};
panelStruct BATTERY = { 110,159, 100,80, "Battery", 2,4, M5.Lcd.color565(251,97,19), M5.Lcd.color565(30,30,30), TFT_WHITE};

DisplayOverview::DisplayOverview() {
  ACInputWatt = 148;
  InverterStatus = "Inverting";
  ACOutputWatt = 1385;
  BatteryPercent = 100;
  PVInputWatt = 586;
  DCOutputWatt = 18;
  BatteryAmps = 37;

  FlowLoop = 0;
}

void DisplayOverview::Loop(bool RefreshDisplay) {
  char buffer[32];

  if (RefreshDisplay) {
    M5.Lcd.fillScreen(TFT_WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    drawPanel(ACIN);
    drawPanel(ACOUT);
    drawPanel(PVIN);
    drawPanel(DCOUT);
    drawPanel(INVERTER);
    drawPanel(BATTERY);
    M5.Lcd.fillRoundRect(BATTERY.left+10, BATTERY.top-7, 14, 10, 3, BATTERY.headerColor565);
    M5.Lcd.fillRoundRect(BATTERY.left+BATTERY.width-24, BATTERY.top-7, 14, 10, 2, BATTERY.headerColor565);
  }

  M5.Lcd.setTextSize(1);
  
  // Inverter Status
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(71,137,208));
  sprintf(buffer, "%s", InverterStatus);
  M5.Lcd.drawString(buffer, 160,52, 2);

  drawData(ACIN, ACInputWatt, "w");
  drawData(ACOUT, ACOutputWatt, "w");
  drawData(PVIN, PVInputWatt, "w");
  drawData(DCOUT, DCOutputWatt, "w");
  drawData(BATTERY, BatteryPercent, "%");

  drawHorizFlow(ACIN.left+ACIN.width, INVERTER.left, ACIN.top+60, ACInputWatt, ACInputMaxWatt);
  drawHorizFlow(INVERTER.left+INVERTER.width, ACOUT.left, ACOUT.top+60, ACOutputWatt, ACOutputMaxWatt);
  drawHorizFlow(PVIN.left+PVIN.width, BATTERY.left, PVIN.top+60, PVInputWatt, PVInputMaxWatt);
  drawHorizFlow(BATTERY.left+BATTERY.width, DCOUT.left, PVIN.top+60, DCOutputWatt, DCOutputMaxWatt);
  drawVertFlow(INVERTER.top+INVERTER.height, BATTERY.top-FLOW_HEIGHT, INVERTER.left+INVERTER.width/2, -1*BatteryAmps, BatteryMaxAmps);

  FlowLoop++;
}

void DisplayOverview::drawPanel(panelStruct panel) {
  M5.Lcd.fillRoundRect(panel.left, panel.top, panel.width, panel.height, PANEL_RADIUS, panel.backgroundColor565);
  M5.Lcd.fillRoundRect(panel.left, panel.top, panel.width, HEADER_HEIGHT, PANEL_RADIUS, panel.headerColor565);
  M5.Lcd.fillRect(panel.left, panel.top+HEADER_HEIGHT-PANEL_RADIUS, panel.width, PANEL_RADIUS, panel.headerColor565);
  M5.Lcd.setTextColor(panel.dataColor565, panel.headerColor565);
  M5.Lcd.drawString(panel.label, panel.left+panel.width/2,panel.top+HEADER_HEIGHT/2, panel.headerFont);
}

void DisplayOverview::drawData(panelStruct panel, int data, String units) {
  char buffer[16];
  M5.Lcd.setTextColor(panel.dataColor565, panel.backgroundColor565);
  sprintf(buffer, "%d%s", data, units);
  M5.Lcd.drawString(buffer, panel.left+panel.width/2,panel.top+panel.height/2+HEADER_HEIGHT/2+2, panel.dataFont);
}

void DisplayOverview::drawHorizFlow(uint x1, uint x2, uint y, int left2right, int max) {
  M5.Lcd.drawRoundRect(x1-2*FLOW_HEIGHT-1, y-FLOW_HEIGHT-1, (x2-x1)+4*FLOW_HEIGHT+2, 2*FLOW_HEIGHT+2, BALL_HEIGHT, FLOW_OUTLINE_COLOR);
  M5.Lcd.fillRoundRect(x1-2*FLOW_HEIGHT, y-FLOW_HEIGHT, (x2-x1)+4*FLOW_HEIGHT, 2*FLOW_HEIGHT, FLOW_HEIGHT, FLOW_FILL_COLOR);
  int delay = calculateDelay(left2right, max);
  if (delay > 100) return; // no flow ball
  uint width = (x2-x1)+2*FLOW_HEIGHT;
  uint offset = (FlowLoop/delay % width);
  uint x = (left2right > 0) ? x1-FLOW_HEIGHT-BALL_HEIGHT+offset : x2+FLOW_HEIGHT-BALL_HEIGHT-offset;
  M5.Lcd.fillRoundRect(x, y-BALL_HEIGHT, 2*BALL_HEIGHT, 2*BALL_HEIGHT, BALL_HEIGHT, FLOW_BALL_COLOR);
}

void DisplayOverview::drawVertFlow(uint y1, uint y2, uint x, int top2bottom, int max) {
  M5.Lcd.drawRoundRect(x-FLOW_HEIGHT-1, y1-2*FLOW_HEIGHT-1, 2*FLOW_HEIGHT+2, (y2-y1)+4*FLOW_HEIGHT+2, BALL_HEIGHT, FLOW_OUTLINE_COLOR);
  M5.Lcd.fillRoundRect(x-FLOW_HEIGHT, y1-2*FLOW_HEIGHT, 2*FLOW_HEIGHT, (y2-y1)+4*FLOW_HEIGHT, FLOW_HEIGHT, FLOW_FILL_COLOR);
  int delay = calculateDelay(top2bottom, max);
  if (delay > 100) return; // no flow ball
  uint height = (y2-y1)+2*FLOW_HEIGHT;
  uint offset = (FlowLoop/delay % height);
  uint y = (top2bottom > 0) ? y1-FLOW_HEIGHT-BALL_HEIGHT+offset : y2+FLOW_HEIGHT-BALL_HEIGHT-offset;
  M5.Lcd.fillRoundRect(x-BALL_HEIGHT, y, 2*BALL_HEIGHT, 2*BALL_HEIGHT, BALL_HEIGHT, FLOW_BALL_COLOR);
}

int DisplayOverview::calculateDelay(int value, int max) {
  int delay = abs(max / value);
  if (delay < 1) delay = 1;
  return 2+delay;  // max animation rate
}

