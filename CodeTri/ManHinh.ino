#include <Leanbot.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>


#define OLED_RESET -1 
Adafruit_SH1106 display(OLED_RESET);

void manhinh(){
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  
  
  display.clearDisplay();
  
  // Cấu hình hiển thị chữ
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,24);
  
  // Ghi nội dung lên màn hình
  display.println("Love4ever");
  
  display.display();
}

void setup() {
  Leanbot.begin();
  manhinh();
}


void loop() {
  LbMission.begin( TB1A + TB1B );

  // Your code here
  
  LbMission.end();
}