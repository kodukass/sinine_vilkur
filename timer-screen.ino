#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Timer.h>

Timer timer;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

 void setup(void) {
    u8g2.begin();
    Serial.begin(9600);    
}

int input = 15;

  void loop(void) {
    timer.start();
    if(timer.state() == RUNNING) Serial.println("timer running");

    for(int i=0; i <= input; i++){

      int time = (timer.read()/1000);
      char cstr[16];
      itoa(time, cstr, 10);
      Serial.println(cstr);
      u8g2.clearBuffer();					// clear the internal memory
      u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
      u8g2.drawStr(8,29,cstr);	// write something to the internal memory
      u8g2.sendBuffer();					// transfer internal memory to the display
      delay(1000);
    }

    timer.stop();
    if(timer.state() == STOPPED) Serial.println("timer stopped");

   delay(3000);
}

