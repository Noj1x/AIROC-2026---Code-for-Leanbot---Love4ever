//Cấu hình thư viện và biến toàn cục
#include <Leanbot.h>
#include <Arduino_APDS9960.h>
int ObjRed, ObjGreen, ObjBlue;
int distance,target,line,speed;
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

// chay robot
void HandleE3(){
	chay(13);
	phai(40);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(140);
	chay(7);
	mau = Obj_detectColor();
	if(mau == "Green" || mau == "Purple" || mau == "Blue"){
		HandleE3Color();
	}
	else{
		HandleE3CRL();
	}
}
void HandleE3Color(){
	chay(12);
	lui(20);
	trai(140);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	phai(90);
	distance = 150;
	followLine();
	LbMotion.stopAndWait();
	trai(35);
	chay(5);
	LbGripper.moveTo(50);
	lui(5);
	trai(140);
	distance = 135;
	followLine();
	LbMotion.stopAndWait();
	trai(90);
	distance = 285;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(15);
	LbGripper.open();
	lui(15);
	phai(140);
	F3fromE3();
}

void F3fromE3(){
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	phai(90);
	distance = 150;
	followLine();
	LbMotion.stopAndWait();
}

void HandleE3CRL(){
	LbGripper.moveTo(50);
	lui(7);
	trai(40);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(30);
	LbGripper.open();
	lui(30);
	F3fromCRL();
}

void F3fromCRL(){
	trai(140);
	distance = 600;
	followLine();
	LbMotion.stopAndWait();
	phai(90);
	distance = 150;
	followLine();
	LbMotion.stopAndWait();
	phai(40);
	chay(7);
	mau = Obj_detectColor();
	if(mau == "Green" || mau == "Purple" || mau == "Blue"){
		F3push();
	}
	else{
		F3CRL();
	}
}

void F3push(){
	LbGripper.moveTo(50);
	phai(20);
	LbGripper.open();
	trai(20);
	lui(7);
	phai(140);
	distance = 150;
	followLine();
	LbMotion.stopAndWait();
	trai(90);
	distance = 600;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(20);
}

void F3CRL(){
	LbGripper.moveTo(50);
	lui(7);
	phai(140);
	distance = 150;
	followLine();
	LbMotion.stopAndWait();
	trai(90);
	distance = 600;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(20);
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
  HandleE3();
  LbMission.end();
}
//================================================