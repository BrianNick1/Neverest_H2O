#include <SoftwareSerial.h>

SoftwareSerial sim(7,8);
int _timeout;
String _buffer;
String number = "+254740352940"; //-> change with your number
String SMS = "Welcome home buddy!";


int EmptyTrigger = 200;
int OverflowTrigger = 20;
int echoPin = 5;
int trigPin = 6;
int echoTime;
long distance;




void setup() 
  {
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    
    delay(7000); //delay for 7 seconds to make sure the modules get the signal
    Serial.begin(9600);
    _buffer.reserve(50);
    Serial.println("All Systems are at Go...");
    sim.begin(19200);
    delay(1000);
    Serial.println("IGNITION!");
    Serial.println("LIFTOFF");
  }
void loop() 

  {
    digitalWrite(trigPin, LOW);                  //Set the trigger pin of the SR04 sensor to low  
    delayMicroseconds(2);    
    digitalWrite(trigPin, HIGH);                //Set the trigger pin of the SR04 sensor to high which will which will send a pulse.  
    delayMicroseconds(10);    
    digitalWrite(trigPin, LOW);                 //Set the trigger pin of the SR04 sensor to low  
    echoTime = pulseIn(echoPin, HIGH);          //Reads a pulse (either HIGH or LOW) on a pin. For example, if value is HIGH, pulseIn() waits for the pin to go HIGH, starts timing, then waits for the pin to go LOW and stops timing. Returns the length of the pulse in microseconds or 0 if no complete pulse was received within the timeout.                                              //https://www.arduino.cc/en/Reference/PulseIn                                          
    distance = echoTime * 0.0340/2;  

    Serial.print("water level is ");
    Serial.println(distance);

        if(distance > EmptyTrigger) 
        {
          Serial.print("Empty Trigger send message");
          SendMessage(number,SMS);
          }

          
        if(distance < OverflowTrigger) 
        {
                    Serial.print("Empty Trigger call the number ");

          callNumber();
          }
          Serial.print("Loop ended repeat");

       delay(200);
  }


void SendMessage(String msg,String no)
  {
    Serial.println ("Sending Message");
    sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
    delay(1000);
    //Serial.println ("Set SMS Number");
    sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(1000);
   
    sim.println(SMS);
    delay(100);
    sim.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    _buffer = _readSerial();
  }
void RecieveMessage()
  {
    Serial.println ("SIM800L Read an SMS");
    delay (1000);
    sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    delay(1000);
    Serial.write ("Unread Message done");
  }
String _readSerial()
  {
    _timeout = 0;
    while (!sim.available() && _timeout < 12000 )
      {
        delay(13);
        _timeout++;
      }
    if (sim.available()) 
      {
        return sim.readString();
      }
  }
void callNumber()
  {
    sim.print (F("ATD"));
    sim.print (number);
    sim.print (F(";\r\n"));
    _buffer = _readSerial();
    Serial.println(_buffer);
  }





  /* this code is formal property of Neverest, IOT Department, can be used for learning purposes but retains its copyrights.*/

  
