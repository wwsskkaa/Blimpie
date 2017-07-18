
# Written by Bonnie


#include <Servo.h>
const int Red_LED = 9;
const int Green_LED = 11;
const int Yellow_LED = 10;

unsigned long PreviousTime;
double Output;
double Setpoint=20;
double PreviousError;
double kp=1.1;//kp tells how loarge gain response will be to current error
value
double kd=1.1;//derivative value gain constant
double Pterm;//proportional term provides output value which is
proportional to current error value.
double Dterm;//derivative term
/*The proportional value gain constant is referred to as Kp, the derivative
value gain constant as Kd.
The results of these multiplications P_term, and D_term.
*/
int safeD=100;
int Button_Pin = 1;
int Button_Value = 0;

int trigger = 2;
int echo = 3;

Servo myservo;//the propellor that actually goes downward
Servo myservo1;//the propellor that goes upward


const int AnalogPin = A0;
int Analog_Value = 0;
int Analog_Value_Changed = 0;

unsigned long duration;
unsigned long distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(Red_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Button_Pin, INPUT);
  myservo.attach(6);
  myservo.writeMicroseconds(1000);
  myservo1.attach(8);
  myservo1.writeMicroseconds(1000);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  Button_Value = LOW;
  while(Button_Value == LOW){
    unsigned long CurrentTime = millis();
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    duration = pulseIn(echo, HIGH);
    distance = duration/58.0;

     /*
     When it is more than safe distance, for example, 1 meter away from the
ground,
     then upper two propellors are going full speed to drive the blimp
downwards.
     */
    if(distance > safeD){
         myservo.writeMicroseconds(2000);//highest speed
      digitalWrite(Yellow_LED, HIGH);
      delay(1000);
      digitalWrite(Yellow_LED, LOW);
    }
      /*
     When it is more than the half of safe distance, for example, between
0.5 meter and 1 meter away from the ground,
     it's going into a slowering mode, which the upper two propellors are
going downward with a reduced speed as they get closer to the ground
     I assume the lowest speed to be 1100,and the highest speed to be
2000,and I want a linear relationship between speed and distance
     Speed=distance*a+b, then we get 2000=safeD*a+b and 1100=(safeD/2)*a+b,
so we get a=1800/safeD and b=200.
     Therefore, as the distance decreases, the speed decreases too.
     */
    else if(distance > safeD/2){
      myservo.writeMicroseconds(distance*1800/safeD+200);
      digitalWrite(Yellow_LED, HIGH);
      delay(10*distance);
      digitalWrite(Yellow_LED, LOW);
      delay(10*distance);
    }
      /*
     When it is less than half of the safe distance, for example, < 0.5
meter away from the ground,
     then need to be prepared for damping.And will call PD controller to
generate the force that would be applied by lower 2 propellors.
     */
    else {
   digitalWrite(Yellow_LED, LOW);
   double TIMECHANGE = (double)(CurrentTime - PreviousTime);//the change in
time
   double error = Setpoint - distance;//how much the actual distance
differs from the desired distance we want.
   double derivativeErr = (error - PreviousError) / TIMECHANGE;//Change in
error over change in time-it is the derivative of the error.
     Output = kp * error + kd * derivativeErr;//to retrive the output which
is closer and closer to setpoint every time.
   PreviousError = error;//record this error to be used next time
   PreviousTime = CurrentTime;//record the time to be used next time
      if(Output<Setpoint) //if the distance it returns is a smaller
distance than setpoint, then we want the bottom two propellors to start
moving,
                          //it is faster so it will add more forces upward,
it will push the blimp up a little bit
      {         myservo.writeMicroseconds(1010);
             myservo1.writeMicroseconds(1200);
             delay(2000);
}
      else if(Output>Setpoint)//if distance it returns is a larger distance
than setpoint, then we know it will still be going down,
                              //so downward servos are actually running
faster to putsh the blimp down a little bit more
      {         myservo.writeMicroseconds(1200);
         myservo1.writeMicroseconds(1100);
                      delay(2000);

}
      else  //if distance ==setpoint, then we know that it
      {         myservo.writeMicroseconds(1100);
         myservo1.writeMicroseconds(1100);
                      delay(1000000);//keep both servos on at the same time
on minimum speed, then we want it to hang on there forever until it got
shut off.
}
    }

    Button_Value = digitalRead(Button_Pin);
  }
  delay(1000);
  Button_Value = LOW;

  delay(1000);
  while(Button_Value == LOW){
    if(analogRead(AnalogPin) < 300){
    digitalWrite(Red_LED, HIGH);
    }
    if(analogRead(AnalogPin) > 400){
    digitalWrite(Red_LED, LOW);
    }
    else{

    }
    /*myservo.writeMicroseconds(1200);
    digitalWrite(Red_LED, HIGH);
    delay(20);
    digitalWrite(Red_LED, LOW);
    delay(20);*/
    Button_Value = digitalRead(Button_Pin);
  }
  Button_Value = LOW;

  delay(1000);
  while(Button_Value == LOW){
   Analog_Value = analogRead(AnalogPin);
   myservo.writeMicroseconds(1200+Analog_Value/2);
   digitalWrite(Green_LED, HIGH);
   delay(1000);
   digitalWrite(Green_LED, LOW);
   delay(1000);
   Button_Value = digitalRead(Button_Pin);
 }
}

void Tuning(double KP, double KD)
{
  /*there are a lot of approahces to tune kp and kd, and depend on
different ways, we have different tuning formulars.*/
   kp = KP;
   kd = KD;
}

void speed(double setpoint, double height)
{
/*we know the mass the robot, we know gravity, and we how much lift can a
propellors do.
The lift of 1 propellors going speed of 1712,and the lift is 9 grams.*/
}

