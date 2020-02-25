#include<SoftwareSerial.h>    // software Serial library contains GSM ans GPRS libraries
SoftwareSerial GPRS(5, 6); //Pins to attach your Gsm

//int buzzerPin = 8;                          //  Pin number the buzzer is connected
int echoPin = 7;                           //  Echo Pin of the ultrasonic sensor
int trigPin = 8;                           //  Trigger Pin of the ultrasonic sensor
int echoTime;                               //  Time taken for the echo
long distance;                              //  Distance of the object from sensorlong echoTime;                              
int triggerAlarmDistance = 10;               //  Buzzer will be triggered if the distance is less than this value. (cm)
int numb;
String msg;
String msg_2;

void setup()
{
  //rtc.begin();              // Initialize and start the Realtime clock module
  GPRS.begin(9600);        // the GPRS baud rate 
  Serial.begin(9600);       // the Serial port of Arduino baud rate. 

  //Set pin modes
  pinMode(echoPin, INPUT);  
  pinMode(trigPin, OUTPUT); 
  
  delay(500);
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

    Serial.print("Distance!=  ");
    Serial.println(distance);
       
    if(distance < triggerAlarmDistance) 
      {    
        msg="NEVEREST H2O:  Water Level Critical";   
        numb="+254719409696";    
        msg_2="gdhjfghjhjklghjk";
        sendSMS(numb, msg);  
        sendSMS(numb, msg_2);  
        
        Serial.println("SMS Sent!!!");   
        Serial.println( "Distance triggered the alarm");  
        delay(10);
      } 
         
    delay(600);                                  //Delay 60ms before next reading.
  }

void sendSMS(int numb, String msg) 
  { 
    GPRS.println("AT+CMGF=1");    // Set modem to text mode 
    delay(200);
    GPRS.write("AT+CMGS=");      // Start composing message  
    GPRS.write(0x22);            // hex equivalent of double-quote '"' 
    GPRS.write(numb); // the number to send the SMS to  
    GPRS.write(0x22);  
    GPRS.write(0x0D);            // hex equivalent of Carraige return  
    GPRS.write(0x0A);            // hex equivalent of newline  
    delay(200);  
    
    // Send the text message to the GSM module  
    GPRS.print(msg); 
   // GPRS.print(distance); 
   // GPRS.print(" cm");  
   
    GPRS.write(0x0D);            // hex equivalent of Carraige return  
    GPRS.write(0x0A);            // hex equivalent of newline  
    delay(100); 
    GPRS.write(26);        // equivalent of CTRL-Z  
    delay(3000);
  }
