// Input data pin definition array
const int DIN[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }; 
// Output data pin definition array
const int DOUT[8] = { 15, 14, 13, 12, 11, 10, 9, 8 };
// 7 Segment definitions for displaying the numbers 0 to 15 
const int SEGS[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

// Pin definition for the latch pull-up of a 74HCT573 data input chip, it does not have any impact if the chip is a 74HCT574 with driven clock
const int PULL_UP = 16;
// Common cathode pin of the single bit LED group
const int LED_GROUP = 17;
// Common cathode pin of the higher digit 7 segment display
const int HIGH_DIGIT = 18;
// Common cathode pin of the lower digit 7 segment display
const int LOW_DIGIT = 19;

// Query all data input bits and return the 8bit data value read from the input chip.
int get_data()
{
  int result=0;
  for (int i=0; i<8; i++)
  {
    int value = digitalRead(DIN[i]);
    if (value == HIGH)
      result += 1<<i;    
  }
  return result;
}

// Set the 8 bit data output for the 7 segment LED
void set_data(int value)
{
  for (int i=0; i<8; i++)
  {
    if ((value & 1) == 0)
      digitalWrite(DOUT[i], LOW);
    else
      digitalWrite(DOUT[i], HIGH);
    value >>= 1;
  }
}

// Set the 8 bit data output for the 8 bit LEDs
void set_data_rev(int value)
{
  for (int i=7; i>=0; i--)
  {
    if ((value & 1) == 0)
      digitalWrite(DOUT[i], LOW);
    else
      digitalWrite(DOUT[i], HIGH);
    value >>= 1;
  }
}

// Initialization function called before the loop
void setup() {
  // put your setup code here, to run once:
  pinMode(PULL_UP, INPUT_PULLUP);

  pinMode(LED_GROUP, OUTPUT);
  digitalWrite(LED_GROUP, HIGH);

  pinMode(HIGH_DIGIT, OUTPUT);
  digitalWrite(HIGH_DIGIT, HIGH);

  pinMode(LOW_DIGIT, OUTPUT);
  digitalWrite(LOW_DIGIT, HIGH);

  for (int i=0; i<8; i++)
  {
    pinMode(DIN[i], INPUT);

    pinMode(DOUT[i], OUTPUT);
    digitalWrite(DOUT[i], LOW);
  }
}

// Variable to capture the input data from the 74HCT574 or 74HCT573
int data = 0;

// Main loop function
void loop() {
  // put your main code here, to run repeatedly:
  data = get_data();

  set_data_rev(data);
  digitalWrite(LED_GROUP, LOW);
  delay(3);
  digitalWrite(LED_GROUP, HIGH);

  set_data(SEGS[data & 0xF]);
  digitalWrite(LOW_DIGIT, LOW);
  delay(1);
  digitalWrite(LOW_DIGIT, HIGH);

  set_data(SEGS[data >> 4]);
  digitalWrite(HIGH_DIGIT, LOW);
  delay(1);
  digitalWrite(HIGH_DIGIT, HIGH);

  delay(5);
}
