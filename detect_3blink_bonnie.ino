// Detects sequence of three blinks of room lighting
// Intended as a robust user signal

int int_led = 13;
int red_led = 12;
int yellow_led = 11;
int green_led = 10;
int button = 7;
int photocell_in = A1;

void setup() 
{
  pinMode(int_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(photocell_in, INPUT);
}

void loop() 
{
  int button_state = LOW;
  int i = 0;
  int range_delay_us = 0;
  int blink_delay = 1000;
  int range_cm = 100;
  int photocell_level;
  int light_state = HIGH;
  int prev_light_state = HIGH;
  int blink_count = 0;
  int dark_cycle_count = 0;
  int light_cycle_count = 0;
  
  // Slow red led flash while waiting
  // for user to  press button to start test
  digitalWrite(green_led, HIGH);
  button_state = LOW;
  while(button_state == LOW)
  {
    digitalWrite(red_led, HIGH);
    delay(400);
    digitalWrite(red_led, LOW);
    delay(400);
    button_state = digitalRead(button);
   }
  // Blink green led to indicate loop exit
  digitalWrite(green_led, 0);
  delay(500);
  digitalWrite(green_led, HIGH);

 
  // 10 Hz flicker while waiting for 3-blink signal
  // (or button press)
  light_state = HIGH; // start with assumption of light
  prev_light_state = HIGH;
  dark_cycle_count = 0;
  light_cycle_count = 0;
  blink_count = 0;
  button_state = LOW;
  while(button_state == LOW && blink_count < 3)
  {
    // Read voltage from photocell sensor
    photocell_level = analogRead(photocell_in);  // 0 - 1023 = 0 - 5V
    // Light is 3.0V or above, dark is 2.0 or below. Keep previous value otherwise
    prev_light_state = light_state;
    if(photocell_level < 400) light_state = LOW;
    if(photocell_level > 600) light_state = HIGH;
    if(light_state == LOW)  // lights are out
    {
      digitalWrite(yellow_led, HIGH);
      light_cycle_count = 0;
      dark_cycle_count++;
    }
    else // lights are on
    {
      digitalWrite(yellow_led, LOW);
      // if a dark interval .3s - 2.0 s is detected
      if (prev_light_state == LOW && dark_cycle_count >= 3 && dark_cycle_count <= 20)  blink_count++; 
      dark_cycle_count = 0;  // reset count of dark ticks
      light_cycle_count++;
      if(light_cycle_count > 50) blink_count = 0; // if no dark for 5s, reset blink count
    }
    
    digitalWrite(red_led, HIGH);
    delay(50);
    digitalWrite(red_led, LOW);
    delay(50);
    button_state = digitalRead(button);
  }
  digitalWrite(green_led, 0);
  delay(500);
  digitalWrite(green_led, HIGH);
  
}
