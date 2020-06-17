 #include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(11,10); //SIM800L Tx & Rx is connected to Arduino #3 & #2

int hasSent;
void setup()
{
  hasSent = 0;

  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(115200);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  mySerial.println("AT+CMGS=\"+447518103652\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("System on!"); //text content
  updateSerial();
  mySerial.write(26);

  delay(1000);
}

int milli = millis();
void loop()
{
  if (digitalRead(2) == HIGH && hasSent == 0) {
    hasSent = 1;
    digitalWrite(3, HIGH);
    mySerial.println("AT+CMGS=\"+447518103652\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print("Button Pressed!"); //text content
    updateSerial();
    mySerial.write(26);
    Serial.println(hasSent);
  } else {
    digitalWrite(3, LOW);
    hasSent=0;
  }

  if (milli <= millis() - 500) {
    updateSerial();
    milli = millis();
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
