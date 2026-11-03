//  Generated Leanbot Arduino code
//    by Leanbot Blockly Editor 2026-02-14

#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int ObjRed,ObjGreen,ObjBlue;

// ==================================================
// ĐỌC RGB — GIỮ CÁCH ĐO CỦA BẠN
// ==================================================
void _coreRead(int& r, int& g, int& b, CRGB ledColor)
{
  LbRGB.fillColor(ledColor);
  LbRGB.show();

  while (!APDS.colorAvailable());

  APDS.readColor(r, g, b);
}

void Obj_readRGB()
{
  int rRed, gGreen, bBlue;
  int rr, gg, bb;

  int originalBrightness = LbRGB.getBrightness();
  LbRGB.setBrightness(255);

  _coreRead(rRed, gg, bb, CRGB::Red);
  _coreRead(rr, gGreen, bb, CRGB::Lime);
  _coreRead(rr, gg, bBlue, CRGB::Blue);
  _coreRead(rr, gg, bb, CRGB::Black);

  LbRGB.setBrightness(originalBrightness);

  ObjRed   = rRed   - rr;
  ObjGreen = gGreen - gg;
  ObjBlue  = bBlue  - bb;
}

// ==================================================
// NHẬN DIỆN MÀU
// ==================================================
boolean BasicObj_isRed()
{
  if (ObjRed <= ObjGreen) return false;
  if (ObjRed <= ObjBlue)  return false;
  if (ObjRed <= 3)        return false;

  return true;
}

boolean BasicObj_isGreen()
{
  if (ObjGreen <= ObjRed)  return false;
  if (ObjGreen <= ObjBlue) return false;
  if (ObjGreen <= 3)       return false;

  return true;
}

boolean BasicObj_isPurple()
{
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Purple: G < 2 × R.
  if ((long)ObjGreen >= 2L * ObjRed) return false;

  return true;
}

boolean BasicObj_isBlue()
{
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Blue: G >= 2 × R.
  if ((long)ObjGreen < 2L * ObjRed) return false;

  return true;
}

// ==================================================
// ĐỌC MÀU
// ==================================================
void Obj_detectColor()
{
  Obj_readRGB();

  
  if (BasicObj_isGreen())
  {
    Green();
  }
  else if (BasicObj_isPurple())
  {
    Purple();
  }
  else if (BasicObj_isBlue())
  {
    Blue();
  }
  else
  {
    RedYellow();
  }
}

void Purple() {
  lui(63);
  Re_phai(28);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_trai(53);
  Di_qua_X_nga(2);
  Di_den_nga();
  LbGripper.moveTo(75);
  Re_trai(152);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_phai(53);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_phai(105);
  chay(100);
  LbGripper.open();
  lui(100);
}

// Describe this function...
void RedYellow() {
  LbGripper.moveTo(75);
  lui(63);
  Re_phai(27);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_trai(27);
  chay(100);
  LbGripper.open();
}

// Describe this function...
void Green() {
  lui(63);
  Re_trai(29);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_phai(53);
  Di_qua_X_nga(1);
  Di_den_nga();
  Re_trai(21);
  chay(27);
  LbGripper.moveTo(75);
  lui(27);
  Re_trai(118);
  Di_qua_X_nga(1);
  Di_den_nga();
  Re_trai(53);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_trai(105);
  chay(100);
  LbGripper.open();
  lui(100);
}

// Describe this function...
void Di_den_khi_co_re() {
  LbIRLine.read();
  while (!((LbIRLine.value() == 15))) {
    Di_theo_line();
    LbIRLine.read();
  }

}

// Describe this function...
void CRLtoH1() {
  Di_den_nga();
  chay(10);
  Re_phai(27);
  Di_qua_X_nga(2);
  Di_den_nga();
  Re_phai(105);
  chay(63);
  Obj_detectColor();
}

auto v = 2000;
// Describe this function...
void Di_theo_line() {
  LbIRLine.read();
  auto bien = LbIRLine.value();
  LbIRLine.displayOnRGB(0x33ff33);
  if ((bien == 4)) {
    LbMotion.runLR(v / 2, v);
  } else if ((bien == 2)) {
    LbMotion.runLR(v, v / 2);
  } else if ((bien == 12)) {
    LbMotion.runLR(0, v);
  } else if ((bien == 3)) {
    LbMotion.runLR(v, 0);
  } else if ((bien == 8)) {
    LbMotion.runLR(0 - v / 2, v / 2);
  } else if ((bien == 1)) {
    LbMotion.runLR(v / 2, 0 - v / 2);
  } else {
    LbMotion.runLR(v, v);
  }
}

// Describe this function...
void Di_den_nga() {
  Di_quang_X(5);
  Di_den_khi_co_re();
  LbMotion.stopAndWait();
}

// Describe this function...
void Di_qua_X_nga(int nga) {
  for (int count = 0; count < nga; count++) {
    Di_quang_X(30);
    Di_den_khi_co_re();
  }

}

// Describe this function...
void Blue() {
  lui(63);
  Re_phai(30);
  Di_qua_X_nga(4);
  Di_den_nga();
  LbGripper.moveTo(75);
  Re_phai(152);
  Di_qua_X_nga(4);
  Di_den_nga();
  Re_phai(105);
  chay(100);
  LbGripper.open();
}

// Describe this function...
void Di_quang_X(int kc) {
  auto kcx = kc + LbMotion.getDistanceMm();
  while ((LbMotion.getDistanceMm() < kcx)) {
    Di_theo_line();
  }

}

// Describe this function...
void Re_phai(int do2) {
  LbMotion.runLR(v, 0 - v);
  LbMotion.waitRotationDeg(do2);
  LbMotion.stopAndWait();
}

// Describe this function...
void chay(int kc) {
  LbMotion.runLR(v, v);
  LbMotion.waitDistanceMm(kc);
  LbMotion.stopAndWait();
}

// Describe this function...
void Re_trai(int do2) {
  LbMotion.runLR(0 - v, v);
  LbMotion.waitRotationDeg(do2);
  LbMotion.stopAndWait();
}

// Describe this function...
void lui(int kc) {
  LbMotion.runLR(0 - v, 0 - v);
  LbMotion.waitDistanceMm(kc);
  LbMotion.stopAndWait();
}


void setup() {
  Leanbot.begin();
  APDS.begin();
  LbIRLine.doManualCalibration(TB1A);
}

void loop() {
  LbMission.begin(TB1A + TB1B);
  CRLtoH1();
  LbMission.end();
}