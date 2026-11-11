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

// Cấu hình Leanbot===============================
void setup() {
  Leanbot.begin();
  LbIRLine.doManualCalibration(TB1A);
  APDS.begin();
}
//================================================

//chay robot
void HandleH2(){
	chay(10);
	phai(90);
	distance = 75;
	followLine();
	LbMotion.stopAndWait();
	phai(40);
	chay(10);
	mau = Obj_detectColor();
	if(mau == "Green" || mau == "Blue" || mau == "Purple"){
		HandleH2Color();
	}
	else HandleH2CRL();
}
void HandleH2Color(){
	chay(5);
	lui(15);
	phai(40);
	distance = 310;
	followLine();
	LbMotion.stopAndWait();
	LbGripper.moveTo(60);
  lui(15);
	phai(180);
  distance = 150;
	followLine();
	LbMotion.stopAndWait();
	phai(140);
	chay(15);
	LbGripper.open();
	lui(15);
	trai(40);
	HandleE3();
}

void HandleH2CRL(){
	LbGripper.moveTo(60);
	lui(10);
	phai(90);
	chay(15);
  LbGripper.open();
	lui(15);
	trai(140);
	HandleE3();
}
void HandleE3(){
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(140);
	chay(10);
	mau = Obj_detectColor();
	if(mau == "Green" || mau == "Blue" || mau == "Purple"){
		HandleE3Color();
	}
	else HandleE3CRL();
}
void HandleE3Color(){
	chay(5);
	lui(15);
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
	LbGripper.moveTo(60);
	lui(5);
	trai(140);
	followLine();
	LbMotion.stopAndWait();
	trai(90);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(15);
	LbGripper.open();
	lui(15);
	trai(140);
	HandleF4fromE3();
}
void HandleE3CRL(){
	LbGripper.moveTo(60);
	lui(10);
	trai(40);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(40);
	chay(15);
	lui(15);
	trai(140);
	HandleF4fromCRL();
}
void HandleF4fromE3(){
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	trai(140);
	chay(10);
	mau = Obj_detectColor();
	if(mau == "Green" || mau == "Blue" || mau == "Purple"){
		HandleF4Color();
	}
	else HandleF4CRL();
}
void HandleF4fromCRL(){
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	phai(90);
	followLine();
	LbMotion.stopAndWait();
	trai(140);
	chay(10);
	if(mau == "Green" || mau == "Blue" || mau == "Purple"){
		HandleF4Color();
	}
	else HandleF4CRL();
}
void HandleF4Color(){
	chay(5);
	lui(15);
	trai(140);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	phai(40);
	chay(10);
}
void HandleF4CRL(){
	LbGripper.moveTo(60);
	lui(10);
	trai(140);
	distance = 300;
	followLine();
	LbMotion.stopAndWait();
	phai(40);
	chay(10);
	LbGripper.open();
}
//Chạy leanbot====================================
void loop() {
  LbMission.begin( TB1A + TB1B );
  speed = 2000;
  HandleH2();
  LbMission.end();
}
//================================================