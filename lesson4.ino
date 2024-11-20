#include <Servo.h>
#define SERVO_PIN   11	//servo to D11
#define LPT 2 				 
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
#define SPEED  120   
#define TURN_SPEED  200
#define BACK_SPEED1  220
#define BACK_SPEED2  90 
#define MOVE_TIME  50   


const int distancelimit = 8; //distance limit for obstacles in front           
int distance;
int numcycles = 0;
Servo head;

void go_Advance()  //motor rotate clockwise -->robot go ahead
{
  digitalWrite(IN4,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN2,HIGH );
  digitalWrite(IN1,LOW);
  set_Motorspeed(SPEED,SPEED);
}
void go_Back() //motor rotate counterclockwise -->robot go back
{
  digitalWrite(IN4,LOW);
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN2,LOW);
  digitalWrite(IN1,HIGH);
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

void setup() {
  /******L298N******/
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);

  stop_Stop();//stop move
  /*init HC-SR04*/
  pinMode(Trig_PIN, OUTPUT); 
  pinMode(Echo_PIN,INPUT); 
  /*init buzzer*/
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, HIGH);  
 
  digitalWrite(Trig_PIN,LOW);
  /*init servo*/
  head.attach(SERVO_PIN); 
  head.write(90);
  delay(2000);
  Serial.begin(9600);

}




void loop(){
  int dist=watch();
   Serial.println(dist);
if (dist<distancelimit) {
 digitalWrite(BUZZ_PIN, LOW);
  go_Advance();
 
  }
  else 
  { digitalWrite(BUZZ_PIN, HIGH);
    if ( dist> distancelimit )
     go_Back();
    else  stop_Stop() ;
  }
}
