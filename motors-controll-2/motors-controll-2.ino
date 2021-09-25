#include <DualTB9051FTGMotorShield.h>

char incomingByte = 0;
char* available ;
//motor library ref
DualTB9051FTGMotorShield md;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Dual TB9051FTG Motor Shield");
  //init led pin + motors
  pinMode(13, OUTPUT);
  md.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  md.enableDrivers();
  delay(1); // wait for drivers to be enabled so fault pins are no longer low

  //if BT gets signal
  if (Serial.available() > 0)
  {

    incomingByte = Serial.read();

    // say what you got:
    Serial.println(Serial.available());
    Serial.print("I received from BT: ");
    Serial.println(incomingByte);


    if (incomingByte == '1') {
      //set led
      digitalWrite(13, HIGH);
      //start motors full speed
      Serial.println("Go go go");
      md.flipM1(true);
      md.setM1Speed(400);
      stopIfFault();
      if (abs(400) % 200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
      }


      md.setM2Speed(400);
      stopIfFault();
      if (abs(400) % 200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM2CurrentMilliamps());
      }

    }
    else if (incomingByte == '0')
    {
      //led off
      digitalWrite(13, LOW);
      Serial.println("Stop you are going over me");

      //stop motors full speed
      md.setM1Speed(0);
      stopIfFault();
      if (abs(0) % 200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
      }


      md.setM2Speed(0);
      stopIfFault();
      if (abs(0) % 200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM2CurrentMilliamps());
      }

    }

  }
}

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while (1);
  }
}
