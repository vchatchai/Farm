

//#define EnB  5
#define PUMP1  D5
#define PUMP2 D6


void pumpOn()
{ 
   
  digitalWrite(PUMP1, HIGH); 


  //LED_STATUS ON
  ledState = 1;
  digitalWrite(LED_STATUS, !ledState);
  Serial.print("Pump On");
}

void pumpOff()
{ 
  digitalWrite(PUMP1, HIGH); 

  //LED_STATUS ON
  ledState = 0;
  digitalWrite(LED_STATUS, !ledState);
  Serial.print("pump Off");
}
