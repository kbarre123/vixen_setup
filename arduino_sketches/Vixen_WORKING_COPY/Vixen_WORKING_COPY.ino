/* 
This sketch allows the Arduino to read 16 bytes of data from Vixen and turn on
its pins accordingly.
*/

// Define pins on Arduino that will control the LEDs.
#define CHANNEL_01 2
#define CHANNEL_02 3
#define CHANNEL_03 4
#define CHANNEL_04 5
#define CHANNEL_05 6
#define CHANNEL_06 7
#define CHANNEL_07 8
#define CHANNEL_08 9
#define CHANNEL_09 10
#define CHANNEL_10 11
#define CHANNEL_11 12
#define CHANNEL_12 13
#define CHANNEL_13 A5
#define CHANNEL_14 A4
#define CHANNEL_15 A3
#define CHANNEL_16 A2

// Define array of channels.
int channels[] = 
  {
    CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07, CHANNEL_08, 
    CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14, CHANNEL_15, CHANNEL_16
  };

// Define number of channels.
#define CHANNEL_COUNT 16

// Define buffer to store data from Vixen until it gets written to the Arduino.
int incomingByte[16];

// Define the baud rate for communication with Vixen. This must match that of the Vixen profile!!  
#define BAUD_RATE 57600

void setup()
{
  Serial.begin(BAUD_RATE);

  // Set up each channel as an output.
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    pinMode(channels[i], OUTPUT);
  }

  powerOnSelfTest();
}

void loop()
{
  turnLightsOff();
  readFromVixen();
}

//***************** DEFINE FUNCTIONS ************************

// Turn lights off
void turnLightsOff()
{
  // Switch from 0 to 255 once I hook up the relays; they're polarity is opposite than LEDs'.
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    analogWrite(channels[i], 0);
  }
}

// Power on self test.
void powerOnSelfTest()
{
  turnLightsOff();
  
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    analogWrite(channels[i], 255);
    delay(500);
    analogWrite(channels[i], 0);
    delay(500);
  }

  turnLightsOff();
}

// Read data from Vixen.
void readFromVixen()
{
  if (Serial.available() >= CHANNEL_COUNT)
  {  // Store incoming bytes to buffer.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }
    // Write buffer to channels.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      analogWrite(channels[i], incomingByte[i]);
    }    
  }
}
