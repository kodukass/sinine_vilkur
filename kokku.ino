#include <Servo.h>
#define TIMERBTN   11
#define BUZZ_PIN	13	//buzzer to D13

#define IN1  7    //Right motor(K1/K2) direction Pin 7
#define IN2  8    //Right motor(K1/K2) direction Pin 8
#define IN3  9    //Left motor(K3/K4) direction Pin 9
#define IN4  10   //Left motor(K3/K4) direction Pin 10
#define ENA  5    //D5 connect to ENA PWM speed pin for Right motor(K1/K2)
#define ENB  6    //D6 connect to ENB PWM speed pin for Left motor(K3/K4)

#define Echo_PIN    2 	// Ultrasonic Echo pin connect to D2
#define Trig_PIN    3  	// Ultrasonic Trig pin connect to D3

#define TURN_TIME  50 
#define FAST_SPEED  200 
#define SPEED  100   
#define TURN_SPEED  200
#define BACK_SPEED1  220
#define BACK_SPEED2  90 
#define MOVE_TIME  50   

#define LEDpin1 12  
const int BUTTON_PIN = 4;       // the number of the pushbutton pin


const int mindistancelimit = 8; //distance limit for obstacles in front           
const int mediumdistancelimit = 30;
int distance;
int numcycles = 0;
Servo head;

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Timer.h>

Timer timer;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

// constants won't change. They're used here to set pin numbers:

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;                // the current reading from the input pin
int timerInt = -1;
int tuluke = 0;


void go_Advance()  //motor rotate clockwise -->robot go ahead
{
  digitalWrite(IN4,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN2,LOW );
  digitalWrite(IN1,HIGH);
  set_Motorspeed(SPEED,SPEED);
}
void go_Left()  //motor rotate clockwise -->robot go ahead
{
  digitalWrite(IN4,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN2,HIGH );
  digitalWrite(IN1,LOW);
  set_Motorspeed(FAST_SPEED,FAST_SPEED);
}
void go_Right()  //motor rotate clockwise -->robot go ahead
{
  digitalWrite(IN4,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN2,LOW );
  digitalWrite(IN1,HIGH);
  set_Motorspeed(FAST_SPEED,FAST_SPEED);
}
void go_Back() //motor rotate counterclockwise -->robot go back
{
  digitalWrite(IN4,HIGH);
  digitalWrite(IN3,LOW); 
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
  set_Motorspeed(SPEED,SPEED);
}
void stop_Stop() //motor brake -->robot stop
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,LOW); 
  set_Motorspeed(0,0);
}
void go_backleft() //motor rotate counterclockwise -->robot go back
{
  digitalWrite(IN4,HIGH);
  digitalWrite(IN3,LOW); 
  digitalWrite(IN2,LOW);
  digitalWrite(IN1,LOW);
  set_Motorspeed(SPEED,SPEED);
}
 
void go_backright() //motor rotate counterclockwise -->robot go back
{
  digitalWrite(IN4,LOW);
  digitalWrite(IN3,LOW); 
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
  set_Motorspeed(SPEED,SPEED);
}

/*set motor speed */
void set_Motorspeed(int lspeed,int rspeed) //change motor speed
{
  analogWrite(ENB,lspeed);//lspeed:0-255
  analogWrite(ENA,rspeed);//rspeed:0-255   
}

int watch(){
  long echo_distance;
  digitalWrite(Trig_PIN,LOW);
  delay(50);                                                                              
  digitalWrite(Trig_PIN,HIGH);
  delay(50);
  digitalWrite(Trig_PIN,LOW);
  echo_distance=pulseIn(Echo_PIN,HIGH);
  echo_distance=echo_distance*0.01657; //how far away is the object in cm
 
  return round(echo_distance);
}



void setup(void) {
  u8g2.begin();

  /******L298N******/
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);

    // initialize serial communication at 9600 bits per second:
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("setup");

  stop_Stop();//stop move
  /*init HC-SR04*/
  pinMode(Trig_PIN, OUTPUT); 
  pinMode(Echo_PIN,INPUT); 
  /*init buzzer*/
  pinMode(BUZZ_PIN, OUTPUT);
  
  digitalWrite(BUZZ_PIN, HIGH);  
 
  digitalWrite(Trig_PIN,LOW);
  /*init servo*/
  //head.attach(SERVO_PIN); 
  head.write(90);
  delay(2000);
  Serial.begin(9600);  

  pinMode(LEDpin1, OUTPUT);

}



int input = 1500;

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

void loop() {
  tuluke += 1;
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);
  //currentState2 = digitalRead(TIMERBTN);

  if(currentState == LOW){
    stop_Stop();
    digitalWrite(LEDpin1, LOW);
    Serial.println("button press");
    timerInt = -1;
    tuluke = 0;
    noTone(BUZZ_PIN);
  }
  Serial.println(timerInt);
  // put your main code here, to run repeatedly:
  if (timerInt == -1)
  {
    timerOLED(input);
    timerInt *= -1;
   
  }
  if (timerInt ==1)
  {
      int dist=watch();
    Serial.println(dist);
  if (dist>mediumdistancelimit) {
  //digitalWrite(BUZZ_PIN, LOW);
    go_Advance();
    Serial.println("advance");
  }
  
  double random1=random(-1,1);
  while (mindistancelimit<dist && dist < mediumdistancelimit)
  { 
    if (random1<0)
    {
      go_Right();
      Serial.println("right");
    }
    else
    {
      go_Left();
      Serial.println("left");
    }
    delay(random(100,200));
    dist=watch();
  }
  if (dist< mindistancelimit)
  { 
    go_Back();
    Serial.println("back");
    delay(500);
  }
  
    
  }

  if (tuluke % 5 == 0){
    digitalWrite(LEDpin1, HIGH);
    //tone(BUZZ_PIN, 500);
  }
  if (tuluke % 10 == 0){
    digitalWrite(LEDpin1, LOW);
    //tone(BUZZ_PIN, 1000);
  }
  
  }

