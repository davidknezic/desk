// Copyright (c) Borislav Bertoldi. All rights reserved.

// DEFINES //////////////////////////

// From Arduino to motor controller
#define PIN_SIGNAL_IN         3    // Must be interrupt pin, depends on board! (YELLOW)
#define PIN_UP_SWITCH_OUT     4    // (GREEN)
#define PIN_DOWN_SWITCH_OUT   5    // (BLUE)

// From hand switch to Arduino
#define PIN_SIGNAL_OUT        7    // (YELLOW)
#define PIN_UP_SWITCH_IN      8    // (GREEN)
#define PIN_DOWN_SWITCH_IN    9    // (BLUE)

// Interrupts
#define INTERRUPT_SIGNAL_IN   0    // Must match pin PIN_SIGNAL_IN above!


// Directions
#define DIR_NONE         0
#define DIR_UP           1
#define DIR_DOWN         2

// Constants
#define MAX_ULONG           4294967295  // Maximum value an unsigned long can have
#define CYCLE_TIME_US       1000        // 1 ms
#define ARRAY_SIZE          32          // Size of the array that stores received bits
#define MATCH_ARRAY_SIZE    23          // Size of the array that contains the bit pattern to compare to
#define BIT_INDEX_HEIGHT    23          // Index at which the height value starts

// Messages
#define MSG_ID_ALIVE_REQUEST       0x01
#define MSG_ID_ALIVE_RESPONSE      0x02
#define MSG_ID_SET_HEIGHT_REQUEST  0x03
#define MSG_ID_GET_HEIGHT_REQUEST  0x04
#define MSG_ID_GET_HEIGHT_RESPONSE 0x05
#define MSG_ID_STOP_REQUEST        0x06
#define MSG_ID_GET_STATUS_REQUEST  0x07
#define MSG_ID_GET_STATUS_RESPONSE 0x08
#define MSG_ID_MOVE_UP             0x0A
#define MSG_ID_MOVE_DOWN           0x0B

// Length (not including the CRC byte!)
#define MESSAGE_LENGTH 2

// Device ID (should be unique!)
#define DEVICE_ID 0xAA


// VARIABLES //////////////////////////

byte                     gCurBit            = 0;
byte                     gCurHeight         = 0;
byte                     gNumMatchingBits   = 0;
byte                     gTargetHeight      = 0;
byte                     gCurStatus         = 0;
byte                     gCurDirection      = DIR_NONE;
bool                     gIsAutoMode        = true;
bool                     gIsSwitchOverride  = false;
volatile unsigned long   gCurTimeUs         = 0;
volatile unsigned long   gLastTimeUs        = 0;


// ARRAYS //////////////////////////

// Array for serial messages
byte gMessageBytes[MESSAGE_LENGTH + 1];

// Array that stores received bits
byte gBitArray[ARRAY_SIZE];

// Array that contains the bit pattern to compare to
byte gMatchArray[MATCH_ARRAY_SIZE] = {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1};


// FUNCTIONS //////////////////////////

void setup()
{
  // Init serial connection
  Serial1.begin(9600);

  // Setup pins
  pinMode(PIN_SIGNAL_IN, INPUT);
  pinMode(PIN_UP_SWITCH_IN, OUTPUT);
  pinMode(PIN_DOWN_SWITCH_IN, OUTPUT);

  pinMode(PIN_SIGNAL_OUT, OUTPUT);
  pinMode(PIN_UP_SWITCH_OUT, OUTPUT);
  pinMode(PIN_DOWN_SWITCH_OUT, OUTPUT);

  // Set initial values
  digitalWrite(PIN_UP_SWITCH_OUT, LOW);
  digitalWrite(PIN_DOWN_SWITCH_OUT, LOW);

  // Init interrupt
  attachInterrupt(INTERRUPT_SIGNAL_IN, onEdgeEvent, CHANGE);

  gLastTimeUs = micros();
}

void loop()
{
  // Make sure loop is only executed every 1ms
  if(!checkTime()) return;

  // Receive and execute messages from the PC
  handleSerialMessaging();

  // Read the current height signal bit and interpret it
  handleCurrentSignalBit();

  // Read input from the real switches (can override PC)
  handleSwitchInputs();

  // Move the table up, down or stop it
  controlTableMovement();
}

// Read input from the real switches (can override PC)
void handleSwitchInputs()
{
  bool upSwitchPressed = digitalRead(PIN_UP_SWITCH_IN);
  bool downSwitchPressed = digitalRead(PIN_DOWN_SWITCH_IN);

  if(gIsSwitchOverride)
  {
    if(!upSwitchPressed && !downSwitchPressed)
    {
      stopTable();
      gIsSwitchOverride = false;
    }
  }
  else
  {
    if(upSwitchPressed)
    {
      moveTableUp();
      gIsSwitchOverride = true;
      gIsAutoMode = false;
    }
    else if(downSwitchPressed)
    {
      moveTableDown();
      gIsSwitchOverride = true;
      gIsAutoMode = false;
    }
  }
}

// Move the table up, down or stop it
void controlTableMovement()
{
  // Don't interfere with the switch override
  if(gIsSwitchOverride) return;

  if(gIsAutoMode)
  {
    if(gCurDirection == DIR_UP   && gCurHeight >= gTargetHeight) stopTable();
    if(gCurDirection == DIR_DOWN && gCurHeight <= gTargetHeight) stopTable();
  }
}

bool checkTime()
{
  gCurTimeUs = micros();
  if(gCurTimeUs - gLastTimeUs >= CYCLE_TIME_US)
  {
    gLastTimeUs = gCurTimeUs;
    return true;
  }
  return false;
}

void handleCurrentSignalBit()
{
   // Grab the current sample and immediately inform the real switch
  gCurBit = digitalRead(PIN_SIGNAL_IN);
  digitalWrite(PIN_SIGNAL_OUT, gCurBit);
  handleReceivedBit();
}

void handleSerialMessaging()
{
  while(Serial1.available())
  {
    // Shift all bytes
    for(int i=0; i<MESSAGE_LENGTH; ++i)
      gMessageBytes[i] = gMessageBytes[i + 1];

    // Insert new byte
    gMessageBytes[MESSAGE_LENGTH] = Serial1.read();

    // Check byte sequence
    checkSequence();
  }
}

void handleReceivedBit()
{
  // Shift the array
  for (byte i = 0; i < ARRAY_SIZE - 1; ++i)
    gBitArray[i] = gBitArray[i + 1];

  // Insert the new value
  gBitArray[ARRAY_SIZE - 1] = gCurBit;

  // Count the number of matching bits
  for (gNumMatchingBits = 0; gNumMatchingBits < MATCH_ARRAY_SIZE; gNumMatchingBits++)
    if (gBitArray[gNumMatchingBits] != gMatchArray[gNumMatchingBits]) break;

  // Update the height if we have a match
  if (gNumMatchingBits >= MATCH_ARRAY_SIZE) extractHeightFromBitArray();
}

void extractHeightFromBitArray()
{
  // Extract the height value from the bit array
  byte newHeight = 0;
  for (byte i = 0; i < 8; ++i)
    newHeight |= (gBitArray[(BIT_INDEX_HEIGHT + i) % ARRAY_SIZE] << i);
  newHeight = ~newHeight;

  // Plausibility check 1: height must be in a valid range
  if (newHeight < 60 || newHeight > 120)
    return;

  // Plausibility check 2: height must not differ more than 5cm from last value
  if (gCurHeight == 0 || abs(gCurHeight - newHeight) < 5)
    gCurHeight = newHeight;
}

void onEdgeEvent()
{
  // Synchronize wait time with edge
  gLastTimeUs = micros() - (CYCLE_TIME_US / 2);
}

// Interpret incoming messages
void checkSequence()
{
  if(!checkChecksum()) return;

  if(gMessageBytes[0] == MSG_ID_ALIVE_REQUEST) // Alive request
  {
    // Respond with alive message
    byte msg[] = {MSG_ID_ALIVE_RESPONSE, DEVICE_ID};
    sendByteSequence(msg, MESSAGE_LENGTH);
  }

  else if(gMessageBytes[0] == MSG_ID_SET_HEIGHT_REQUEST) // Set height request
  {
    // Move table to height
    if(!gIsSwitchOverride)
    {
      gIsAutoMode = true;
      gTargetHeight = gMessageBytes[1];
      if     (gCurHeight < gTargetHeight) moveTableUp();
      else if(gCurHeight > gTargetHeight) moveTableDown();
    }
  }

  else if(gMessageBytes[0] == MSG_ID_GET_HEIGHT_REQUEST) // Get height request
  {
    // Respond with height message
    byte msg[] = {MSG_ID_GET_HEIGHT_RESPONSE, gCurHeight};
    sendByteSequence(msg, MESSAGE_LENGTH);
  }

  else if(gMessageBytes[0] == MSG_ID_STOP_REQUEST) // Stop request
  {
    if(!gIsSwitchOverride) stopTable();
  }

  else if(gMessageBytes[0] == MSG_ID_GET_STATUS_REQUEST) // Get status request
  {
    // Respond with status message
    byte msg[] = {MSG_ID_GET_STATUS_RESPONSE, gCurStatus};
    sendByteSequence(msg, MESSAGE_LENGTH);
  }

  else if(gMessageBytes[0] == MSG_ID_MOVE_UP) // Move request
  {
    gIsAutoMode = false;
    moveTableUp();
  }

  else if(gMessageBytes[0] == MSG_ID_MOVE_DOWN) // Move request
  {
    gIsAutoMode = false;
    moveTableDown();
  }
}

// Send a response to the PC
void sendByteSequence(byte bytes[], int count)
{
  byte chk = 0;
  for(int i=0; i<count; ++i) {
     Serial1.write(bytes[i]);
     chk += bytes[i];
  }
  Serial1.write(chk);
}

// Check the checksum on the currently received bytes
bool checkChecksum()
{
  byte chk = 0;
  for(int i=0; i<MESSAGE_LENGTH; ++i) chk += gMessageBytes[i];
  return gMessageBytes[MESSAGE_LENGTH] == chk;
}

// Moves the table
void moveTableUp()
{
    gCurDirection = DIR_UP;

    digitalWrite(PIN_DOWN_SWITCH_OUT, LOW);
    digitalWrite(PIN_UP_SWITCH_OUT, HIGH);
}

// Moves the table
void moveTableDown()
{
    gCurDirection = DIR_DOWN;

    digitalWrite(PIN_UP_SWITCH_OUT, LOW);
    digitalWrite(PIN_DOWN_SWITCH_OUT, HIGH);
}

// Stops the table
void stopTable()
{
    gCurDirection = DIR_NONE;

    digitalWrite(PIN_DOWN_SWITCH_OUT, LOW);
    digitalWrite(PIN_UP_SWITCH_OUT, LOW);

    gTargetHeight = gCurHeight;
}
