// which pins control which channels
#define CHANNEL01  2
#define CHANNEL02  3
#define CHANNEL03  4
#define CHANNEL04  5
#define CHANNEL05  6
#define CHANNEL06  7
#define CHANNEL07  8
#define CHANNEL08  9
#define CHANNEL09  10
#define CHANNEL10  11
#define CHANNEL11  12
#define CHANNEL12  13
#define CHANNEL13  A5
#define CHANNEL14  A4
#define CHANNEL15  A3
#define CHANNEL16  A2

// Which pins is the random/Vixen mode switch using
#define RANDOM_MODE_PININ A0
//#define RANDOM_MODE_PINOUT 53
#define RANDOM_MODE_SPEED 2500

int channels[] = {CHANNEL01,CHANNEL02,CHANNEL03,CHANNEL04,CHANNEL05,CHANNEL06,CHANNEL07,CHANNEL08,CHANNEL09,
CHANNEL10,CHANNEL11,CHANNEL12,CHANNEL13,CHANNEL14,CHANNEL15,CHANNEL16};

// how many channel will vixen be sending
#define CHANNEL_COUNT 16

// speed for the com port for talking with vixen
#define VIXEN_COM_SPEED 19200

// speed for talking with the serial monitor in the IDE
//#define PC_COM_SPEED 57600

// setup your choice of dimming values or just on/off values
// the relays don't seem to be able to dim the lights so it looks
// like I will have to build dimmer circuits for next year. The code
// doesn't change, just have to remove the relay board and replace
// it with a dimmer circuit for each relay.
//#define MODE_DIMMING 0
//#define MODE_FULL 1
//#define MODE MODE_FULL

boolean startingVixen = true;

void setup()
{
  //Serial.begin(PC_COM_SPEED);
  Serial.begin(VIXEN_COM_SPEED);
  
  // set the channel pins to output mode
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
    pinMode(channels[channelIndex],OUTPUT);
  }
  
  // set up the switch for Vixen or Random mode
  pinMode(RANDOM_MODE_PININ, INPUT);
  //digitalWrite(RANDOM_MODE_PININ,HIGH); // turn on the internal pull-up resistor
  //pinMode(RANDOM_MODE_PINOUT, OUTPUT);
  
  turnLightsOff();  
  powerOnSelfTest();
}

// !!!! note the PWM values that need to be sent to the relay board are reversed from the
// values comming in from Vixen.  Vixen 0-255 (off-on), Relays 255-0 (off-on)
void loop()
{
  if(digitalRead(RANDOM_MODE_PININ)==HIGH){ // blink at random mode
    startingVixen=true;
    doRandomLights();
  }else{ // play from Vixen mode
    if(startingVixen==true)
      turnLightsOff();
    readFromVixen();
  }
}

void powerOnSelfTest()
{
    //Serial.println("Power on self test running.");
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
      //Serial.print("Channel: ");
      //Serial.println(channelIndex+1,DEC);
      analogWrite(channels[channelIndex], 255); // turn on one channel at a time
      delay(500); // wait .5 seconds
      analogWrite(channels[channelIndex], 0);
    }
    turnLightsOff(); 
}

void turnLightsOff()
{
  //turn them all off
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
    analogWrite(channels[channelIndex], 0);
  }
}

void doRandomLights()
{
    randomSeed(analogRead(0));
    //Serial.println("Writting random values.");
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
      {
        int randNumber = random(0, 255);
        if(randNumber<=128)
          analogWrite(channels[channelIndex], 255);
        else
          analogWrite(channels[channelIndex], 0);
      }
    
    delay(2500);
}

void outputToLights(unsigned char* buffer)
{
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
    {
      analogWrite(channels[channelIndex], buffer[channelIndex]);
    }    
}

void readFromVixen()
{
  startingVixen = false;
  char *footer="VIXEN_END";
  unsigned char buffer[CHANNEL_COUNT];
  char buffer2[CHANNEL_COUNT];
  int index=0;
  unsigned long time = millis();

  waitForVixenHeader();
  while (true) {
    int inByte = Serial.read();
    if(inByte==-1){
      if(index==0 && millis()-time>1000) // we haven't read anything in a second
        return;
      continue;
    }
    time = millis();
    int lightByte = map(inByte, 0, 255, 255, 0);
    buffer[index] = inByte;
    buffer2[index] = inByte;
    buffer[index+1] = 0;
    buffer2[index+1] = 0;
    index++;
    if(index==9 && strcmp(footer,buffer2)==0)
    {
      return;
    }
    else if(index==CHANNEL_COUNT){
      outputToLights(buffer);
      index=0;
    }
  }
}

void waitForVixenHeader()
{
  char *header="VIXEN_START";
  char buffer[12];
  int index = 0;
  unsigned long time = millis();

  while (true) {
  int inByte = Serial.read();
    if(inByte==-1){
      if(index==0 && millis()-time>1000) // we haven't read anything in a second
        return;
      continue;
    }
    time = millis();
    buffer[index] = inByte;
    if(buffer[index]!=header[index]) {// not the right sequence restart
      index=-1;
    }
    buffer[index+1] = 0; // add null
    index++;
    if(index==11 && strcmp(header,buffer)==0)
    {
      return;
    }
  }
}



