
// This example uses the compact protocol, so the Tic's device
// number is not used, and can be set to anything.
//
// If you have sent a De-energize command to the Tic, for example
// by clicking "De-energize" in the Tic Control Center, you will
// need to undo that by clicking "Resume" or power-cycling the
// Tic.
//
// Please see https://github.com/pololu/tic-arduino for details
// on how to make the connections between the Arduino and the
// Tic.

#include <Tic.h>
#include <NewPing.h>
#include <stdlib.h>

// On boards with a hardware serial port available for use, use
// that port to communicate with the Tic. For other boards,
// create a SoftwareSerial object using pin 10 to receive (RX)
// and pin 11 to transmit (TX).
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define ticSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial ticSerial(10, 11);
#endif
// distanse sensor

#define TRIGGER_PIN 5

#define ECHO_PIN 6

#define MAX_DISTANCE 200
long distance;
int looper;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
TicSerial tic(ticSerial);

void setup()
{
  looper = 0;

  ticSerial.begin(9600);

  // Set the baud rate.
  Serial.begin(9600);

  Serial.println("Hello Arduino online");

  // Give the Tic some time to start up.
  delay(20);

  // Set the Tic's current position to 0, so that when we command
  // it to move later, it will move a predictable amount.
  tic.haltAndSetPosition(0);
  Serial.println("Halt tic pos 0");

  // Tells the Tic that it is OK to start driving the motor.  The
  // Tic's safe-start feature helps avoid unexpected, accidental
  // movement of the motor: if an error happens, the Tic will not
  // drive the motor again until it receives the Exit Safe Start
  // command.  The safe-start feature can be disbled in the Tic
  // Control Center.
  tic.exitSafeStart();
  Serial.println("Tic exit safe start");

}

// Sends a "Reset command timeout" command to the Tic.  We must
// call this at least once per second, or else a command timeout
// error will happen.  The Tic's default command timeout period
// is 1000 ms, but it can be changed or disabled in the Tic
// Control Center.
void resetCommandTimeout()
{
  tic.resetCommandTimeout();
}

// Delays for the specified number of milliseconds while
// resetting the Tic's command timeout so that its movement does
// not get interrupted by errors.
void delayWhileResettingCommandTimeout(uint32_t ms)
{
  uint32_t start = millis();
  do
  {
    resetCommandTimeout();
  } while ((uint32_t)(millis() - start) <= ms);
}

// Polls the Tic, waiting for it to reach the specified target
// position.  Note that if the Tic detects an error, the Tic will
// probably go into safe-start mode and never reach its target
// position, so this function will loop infinitely.  If that
// happens, you will need to reset your Arduino.
void waitForPosition(int32_t targetPosition)
{
  do
  {
    resetCommandTimeout();
  } while (tic.getCurrentPosition() != targetPosition);
}

void loop()
{
  tic.exitSafeStart();

  //Serial.println("in tha loop");
  distance = calculateDistance();
  Serial.print(distance);
  Serial.println (" cm");


  if (distance < 40 && distance > 5 )
  {
    int r = rand() % 3;
    Serial.println(r);
    if (r % 2 == 0) {
      tic.setTargetPosition(230);
      waitForPosition(100);
      // delayWhileResettingCommandTimeout(3000);
      delay(1500);
      // Tell the Tic to move to position -100, and delay for 3000 ms
      // to give it time to get there.

      tic.setTargetPosition(0);
      Serial.print("setTargetPosition 0");

      // waitForPosition(100);
      delayWhileResettingCommandTimeout(2000);
      Serial.print("waitForPosition 3000");

      //delayWhileResettingCommandTimeout(3000);
    }
    Serial.print("delay 500");

    delay(500);
  }

}

int calculateDistance() {
  int dis = 0;
  delay(50);

  unsigned int uS = sonar.ping();

  pinMode(ECHO_PIN, OUTPUT);

  digitalWrite(ECHO_PIN, LOW);

  pinMode(ECHO_PIN, INPUT);

  // Serial.print("Ping: ");
  dis = uS / US_ROUNDTRIP_CM;
  return (dis);

}
