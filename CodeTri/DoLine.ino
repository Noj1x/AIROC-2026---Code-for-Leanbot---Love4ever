// Cấu hình thư viện và biến toàn cục
#include <Leanbot.h>
int distance,target,line,speed,test;
//===========================================

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
      LbMotion.runLR(speed / 8, speed);
      break;

    case 2:
    case 7:
    case 3:
    case 1:
      LbMotion.runLR(speed, speed / 8);
      break;

    case 9:
    case 13:
    case 11:
    case 10:
    case 5:
      test = 123;
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

//===============================================


// cấu hình leanbot
void setup() {
  Leanbot.begin();
  LbIRLine.doManualCalibration(TB1A);
}
//===============================================

// main
void loop() {
  LbMission.begin( TB1A + TB1B );

  speed = 1500;      //speed luôn bằng 1500 để chuẩn xác
  distance = 400;    //distance chỉnh theo số đường cần đi (linh hoạt)
  followLine();
  LbMotion.stopAndWait();
  
  LbMission.end();
}
//===============================================