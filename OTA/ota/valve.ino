

//Motor Connections
//Change this if you wish to use another diagram
//#define EnA  10
//#define EnB  5
#define In1  D5
#define In2  D6
#define In3  D7
#define In4  D8


void valveSetup()
{
  // All motor control pins are outputs
  //  pinMode(EnA, OUTPUT);
  //  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
}

void valveOn()
{
  // turn on motor A
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  // set speed to 150 out 255
  //  analogWrite(EnA, 200);
  // turn on motor B
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  // set speed to 150 out 255
  //  analogWrite(EnB, 200);
  delay(500);
  // now turn off motors
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  //LED_STATUS ON
  ledState = 1;
  digitalWrite(LED_STATUS, !ledState);
  Serial.print("Valve On");
}

void valveOff()
{
  // turn on motor A
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  // set speed to 150 out 255
  //  analogWrite(EnA, 200);
  // turn on motor B
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  // set speed to 150 out 255
  //  analogWrite(EnB, 200);
  delay(500);
  // now turn off motors
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  //LED_STATUS ON
  ledState = 0;
  digitalWrite(LED_STATUS, !ledState);
  Serial.print("Valve Off");
}
