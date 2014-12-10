/* 
This sketch allows the Arduino to read 16 bytes of data from Vixen and turn on
its pins accordingly, which in turn control a solid state relay hooked up to Xmas lights.
*/

// Define pins on Arduino that will control the relay.
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

// Define variable to hold # of channels
#define CHANNEL_COUNT 16

// Define array to hold all channels
int channels[] = 
{
  CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07,
  CHANNEL_08, CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14, CHANNEL_15, CHANNEL_16
};

// Define array to hold incoming data stream from Vixen
int incomingByte[16];

// Define baud rate. This figure must match that of your profile configuration in Vixen!
#define BAUD_RATE 57600

void setup()
{
  // Begin serial communication
  Serial.begin(BAUD_RATE);

  // Set up each channel as an output
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    pinMode(channels[i], OUTPUT);  
  }
}

void loop()
{
  // Read data from Vixen, store in array
  if (Serial.available() >= CHANNEL_COUNT)
  {
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }                                  
    // Write each byte of data from array to a pin on Arduino
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      digitalWrite(channels[i], incomingByte[i]);
    }
  }
}
