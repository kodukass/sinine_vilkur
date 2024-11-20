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


int input = 5;

int timerOLED(int input){
  timer.start();
  if(timer.state() == RUNNING) Serial.println("timer running");

  for(int i=0; i <= input; i++){

    int sec = (input - timer.read()/1000);
    int min = (sec/60);
    int secDisplay = (sec-min*60);
    char cstrSec[5];
    char cstrMin[5];
    char cstrSecDisplay[5];
    char minSec[4];
    char nullTaht[1] = "0";
    char koolon[1] = ":";
    
    for(int i=0; i<6; i++){
      minSec[i] = 0;
    }
    
    itoa(sec, cstrSec, 10);
    itoa(min, cstrMin, 10);
    itoa(secDisplay, cstrSecDisplay, 10);

    
    if(min>=10){
      minSec[0]=cstrMin[0];
      minSec[1]=cstrMin[1];
    }
    if(min<10){
      minSec[0]=nullTaht[0];
      minSec[1]=cstrMin[0];
    }
    

    minSec[2]= koolon[0];

    
    if(secDisplay>=10){
      minSec[3]=cstrSecDisplay[0];
      minSec[4]=cstrSecDisplay[1];
    }
    if(secDisplay<10){
      minSec[3]=nullTaht[0];
      minSec[4]=cstrSecDisplay[0];
    }
    
    
    Serial.println(minSec);
    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(8,29,minSec);	// write something to the internal memory

    u8g2.sendBuffer();					// transfer internal memory to the display
    delay(1000);
  }

  timer.stop();
  if(timer.state() == STOPPED) Serial.println("timer stopped");

  return 0;
}

void loop(void) {
  timerOLED(input);
}

