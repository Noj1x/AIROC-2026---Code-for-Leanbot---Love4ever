// cấu hình thư viện và biến toàn cục
#include <Leanbot.h>
//=============================================

// các hàm di chuyển cơ bản, có quy ước để rẽ tốt nhất

void thang(int kc) {
  LbMotion.runLRrpm(50, 50);
  LbMotion.waitDistanceMm(kc * 10);
  LbMotion.stopAndWait();
}

void lui(int kc) {
  LbMotion.runLRrpm(-50, -50);
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

// cấu hình Leanbot
void setup() {
  Leanbot.begin();
}
//==============================================

//main
void loop() {
  LbMission.begin( TB1A + TB1B );
  
  LbMission.end();
}
//==============================================
