#ifndef __DISPLAY_OVERVIEW_H_
#define __DISPLAY_OVERVIEW_H_
#include <M5Core2.h>

struct panelStruct {
  uint left;
  uint top;
  uint width;
  uint height;
  String label;
  uint headerFont;
  uint dataFont;
  uint backgroundColor565;
  uint headerColor565;
  uint dataColor565;
};

class DisplayOverview {
  public:
    DisplayOverview();
    void Loop(bool RefreshDisplay);

  private:
    void drawPanel(panelStruct panel);
    void drawData(panelStruct panel, int data, String units);
    void drawHorizFlow(uint x1, uint x2, uint y, int left2right, int max);
    void drawVertFlow(uint y1, uint y2, uint x, int top2bottom, int max);
    int calculateDelay(int value, int max);

  private:
    int ACInputWatt;
    String InverterStatus;
    int ACOutputWatt;
    int BatteryPercent;
    int PVInputWatt;
    int DCOutputWatt;
    int BatteryAmps;
    
    uint FlowLoop;
};

#endif