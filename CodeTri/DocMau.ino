//Cấu hình thư viện và biến toàn cục
#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int ObjRed, ObjGreen, ObjBlue;
String mau;
//================================================

// ĐỌC RGB =======================================

// Gọi cảm biến đọc màu
void _coreRead(int& r, int& g, int& b, CRGB ledColor){
  LbRGB.fillColor(ledColor);
  LbRGB.show();

  while (!APDS.colorAvailable());

  APDS.readColor(r, g, b);
}

// Trả về kết quả của cảm biến
void Obj_readRGB(){
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
//================================================

// NHẬN DIỆN MÀU =================================

// Check xem có phải Đỏ không ?
boolean BasicObj_isRed(){
  if (ObjRed <= ObjGreen) return false;
  if (ObjRed <= ObjBlue)  return false;
  if (ObjRed <= 3)        return false;

  return true;
}

// Check xem có phải Xanh Lá không ?
boolean BasicObj_isGreen(){
  if (ObjGreen <= ObjRed)  return false;
  if (ObjGreen <= ObjBlue) return false;
  if (ObjGreen <= 3)       return false;

  return true;
}

// Check xem có phải Tím không ?
boolean BasicObj_isPurple(){
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Purple: G < 2 × R.
  if ((long)ObjGreen >= 2L * ObjRed) return false;

  return true;
}

// Check xem có phải Xanh Lam không ?
boolean BasicObj_isBlue(){
  if (ObjBlue <= ObjRed)   return false;
  if (ObjBlue <= ObjGreen) return false;
  if (ObjBlue <= 3)        return false;

  if (ObjRed <= 0)         return false;
  if (ObjGreen <= 0)       return false;

  // Blue: G >= 2 × R.
  if ((long)ObjGreen < 2L * ObjRed) return false;

  return true;
}
//================================================

// ĐỌC MÀU========================================
String Obj_detectColor(){
  Obj_readRGB();
  
  if (BasicObj_isGreen()){
    return "Green";
  }
  else if (BasicObj_isPurple()){
    return "Purple";
  }
  else if (BasicObj_isBlue()){
    return "Blue";
  }
  else if(BasicObj_isRed()){
    return "Red";
  }
  else{
    return "Yellow";
  }
}
//================================================

// Cấu hình Leanbot===============================
void setup() {
  Leanbot.begin();
  LbIRLine.doManualCalibration(TB1A);
  APDS.begin();
}
//================================================

//Chạy leanbot====================================
void loop() {
  LbMission.begin( TB1A + TB1B );
  mau = Obj_detectColor();
  LbMission.end();
}
//================================================