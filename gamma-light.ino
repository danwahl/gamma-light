#define PWM_PERIOD    25            // sample period in ms
#define LED_OUT_PIN   0
#define SW_IN_PIN     3
#define DIM_IN_PIN    A2

unsigned int sw_state, sw_prev;
unsigned long led_start;

void setup() {
  // led setup
  led_start = 0;
  pinMode(LED_OUT_PIN, OUTPUT);
  digitalWrite(LED_OUT_PIN, LOW);

  // switch setup
  sw_state = 0;
  sw_prev = HIGH;
  pinMode(SW_IN_PIN, INPUT_PULLUP);
}

int soft_pwm(unsigned long &start, int period, int duty) {
  // calculate time difference from start
  unsigned long diff = millis() - start;

  // check for end of period
  if(diff >= period) start = millis();
  // check diff against duty
  else if(diff >= duty) return HIGH;
  
  return LOW;
}

void loop() {
  // get dimmer setting
  int dim_out = PWM_PERIOD - map(analogRead(DIM_IN_PIN), 0, 1023, 0, PWM_PERIOD - 1);

  // check for switch state change
  int sw_in = digitalRead(SW_IN_PIN); 
  if(sw_in && !sw_prev) sw_state = !sw_state;
  sw_prev = sw_in;

  // if on, write dimmer setting to led
  if(sw_state) digitalWrite(LED_OUT_PIN, soft_pwm(led_start, PWM_PERIOD, dim_out));
  // otherwise turn led off
  else digitalWrite(LED_OUT_PIN, LOW);
  
  // delay before next loop
  delayMicroseconds(500);
}

