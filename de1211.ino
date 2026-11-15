//Cấu hình thư viện và biến toàn cục
#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int distance,target,line,speed;
int ObjRed, ObjGreen, ObjBlue;
String mau;
//================================================

// Dò line và đi theo line===================

// Hàm này check if/else trên cảm biến ánh sáng, từ đó điều chỉnh hướng để robot đi theo Line 
void runFollowLine() {
  LbIRLine.read();
  line = LbIRLine.value();
  LbIRLine.displayOnRGB(0x00ff00);
  switch (line) {
    case 4:
    case 14:
    case 12:
    case 8:
      LbMotion.runLR(speed / 4, speed);
      break;

    case 2:
    case 7:
    case 3:
    case 1:
      LbMotion.runLR(speed, speed / 4);
      break;

    case 9:
    case 13:
    case 11:
    case 10:
    case 5:
      break;

    default:
      LbMotion.runLR(speed, speed);
   }

}
//===========================================

// hàm này chạy theo vạch đen, mỗi lần nhích lên tí, lố thì dừng
void followLineCarefully() {
  for (int count = 0; count < 10; count++) {
    runFollowLine();
    if (LbIRLine.isBlackDetected()) {
      break;
    }
    LbMotion.waitDistanceMm(1);
  }

}
//==============================================

// Cho quãng đường cần đi, đi đúng quãng đấy, lố thì dừng
void followLine() {
  target = LbMotion.getDistanceMm() + distance;
  followLineCarefully();
  while (LbIRLine.isBlackDetected()) {
    followLineCarefully();
    if ((LbMotion.getDistanceMm() > target)) {
      if ((LbIRLine.value() == 6)) {
        break;
      }
    }
    LbMotion.waitDistanceMm(1);
  }

}
//===============================================

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

// các hàm di chuyển cơ bản, có quy ước để rẽ tốt nhất

void chay(int kc) {
  LbMotion.runLRrpm(30, 30);
  LbMotion.waitDistanceMm(kc * 10);
  LbMotion.stopAndWait();
}

void lui(int kc) {
  LbMotion.runLRrpm(-30, -30);
  LbMotion.waitDistanceMm(kc * 10);
  LbMotion.stopAndWait();
}

void phai(int goc) {
  LbMotion.runLRrpm(25, -25);
  LbMotion.waitRotationDeg(goc);
  LbMotion.stopAndWait();
}

void trai(int goc) {
  LbMotion.runLRrpm(-25, 25);
  LbMotion.waitRotationDeg(goc);
  LbMotion.stopAndWait();
}
//==============================================

void HandleTask(){
  HandleF3();
}
void HandleF3(){
  chay(10);
  phai(90);
  distance = 375;
  followLine();
  LbMotion.stopAndWait();
  phai(40);
  chay(10);
  mau = Obj_detectColor();
  if(mau == "Green" || mau == "Blue" || mau == "Purple"){
    HandleF3Color();
  }
  else{
    HandleF3CRL();
  }
}
void HandleF3Color(){
  chay(5);
  lui(15);
  phai(40);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  phai(90);
  distance = 140;
  followLine();
  LbMotion.stopAndWait();
  trai(30);
  chay(3);
  LbGripper.moveTo(60);
  lui(4);
  trai(150);
  followLine();
  LbMotion.stopAndWait();
  trai(40);
  chay(10);
  LbGripper.open();
  lui(11);
  HandleC4fromF3();
}
void HandleF3CRL(){
  LbGripper.moveTo(60);
  lui(10);
  phai(140);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  trai(40);
  chay(20);
  LbGripper.open();
  lui(20);
  HandleC4fromCRL();
}
void HandleC4fromF3(){
  trai(40);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  phai(40);
  chay(10);
  mau = Obj_detectColor();
  if(mau == "Green" || mau == "Blue" || mau == "Purple"){
    HandleC4Color();
  }
  else{
    HandleC4CRL();
  }
}
void HandleC4fromCRL(){
  trai(140);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  trai(40);
  chay(10);
  mau = Obj_detectColor();
  if(mau == "Green" || mau == "Blue" || mau == "Purple"){
    HandleC4Color();
  }
  else{
    HandleC4CRL();
  }
}
void HandleC4Color(){
  chay(5);
  lui(15);
  trai(40);
  HandleE1fromC4();
}
void HandleC4CRL(){
  LbGripper.moveTo(60);
  lui(10);
  trai(140);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  trai(40);
  chay(15);
  LbGripper.open();
  lui(16);
  HandleE1fromCRL();
}
void HandleE1fromC4(){
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  trai(140);
  chay(5);
  mau = Obj_detectColor();
  if(mau == "Green" || mau == "Blue" || mau == "Purple"){
    HandleE1fromC4Color();
  }
  else{
    HandleE1fromC4CRL();
  }
}
void HandleE1fromC4Color(){
  chay(5);
  lui(10);
  trai(40);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  phai(90);
  followLine();
  LbMotion.stopAndWait();
  trai(90);
  followLine();
  LbMotion.stopAndWait();
  LbGripper.moveTo(60);
  trai(180);
  followLine();
  LbMotion.stopAndWait();
  phai(40);
  chay(10);
  LbGripper.open();
  lui(30);
}
void HandleE1fromC4CRL(){
  LbGripper.moveTo(60);
  chay(50);
  LbGripper.open();
}
void HandleE1fromCRL(){
  trai(180);
  chay(30);
  mau = Obj_detectColor();
  if(mau == "Green" || mau == "Blue" || mau == "Purple"){
    HandleE1fromCRLColor();
  }
  else{
    HandleE1fromCRLCRL();
  }
}
void HandleE1fromCRLColor(){
  LbGripper.moveTo(60);
  lui(5);
  LbGripper.open();
  lui(5);
  phai(180);
  chay(20);
  trai(40);
  distance = 300;
  followLine();
  LbMotion.stopAndWait();
  LbGripper.moveTo(60);
  trai(180);
  followLine();
  LbMotion.stopAndWait();
  phai(40);
  chay(10);
  LbGripper.open();
  lui(40);
}
void HandleE1fromCRLCRL(){
  LbGripper.moveTo(60);
  lui(50);
  LbGripper.open();
}
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
  speed = 2000;
  HandleTask();
  LbMission.end();
}
//================================================