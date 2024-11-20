#define LEDpin1 10                                                                     
#define LEDpin2 9
#define time 500
void setup()
{
pinMode(LEDpin1, OUTPUT);
pinMode(LEDpin2, OUTPUT);
}
void loop()
{
digitalWrite(LEDpin1, HIGH);
delay(time);

digitalWrite(LEDpin2, HIGH);
digitalWrite(LEDpin1, LOW);
delay(time);
digitalWrite(LEDpin2, LOW);
//delay(time);
}