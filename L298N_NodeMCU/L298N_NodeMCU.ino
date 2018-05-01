int ENA = 4;
//int IN1 = 0;
//int IN2 = 2;


int IN1 = 16;
int IN2 = 5;



void setup()
{


  // set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(ENA, HIGH);
//analogWrite(ENA, 1);

}

// this function will run the motors in both directions at a fixed speed

void testOne() {
// turn on motor
//digitalWrite(ENA, LOW);  // set speed to 200 out of possible range 0~255
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);

delay(50); 

//digitalWrite(IN1, LOW);
//digitalWrite(IN2, HIGH);
//
//delay(5000); // now turn off motors

digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}

// this function will run the motors across the range of possible speeds
// note that maximum speed is determined by the motor itself and the operating voltage
// the PWM values sent by analogWrite() are fractions of the maximum speed possible by your hardware

void testTwo() {

// turn on motors

digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
delay(50); 

//// accelerate from zero to maximum speed
//for (int i = 0; i < 256; i++)
//  {
//    analogWrite(ENA, i);
//    delay(50);
//   }
//
//// decelerate from maximum speed to zero
//for (int i = 255; i >= 0; --i)
//    {
//      analogWrite(ENA, i);
//      delay(50);
//     }

// now turn off motors
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}



void loop()
{


  testOne();   
  delay(5000);   
  testTwo();   
  delay(5000);

}
