/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(4, 3);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
#include <TinyGPS++.h>
volatile float minutes, seconds;
volatile int degree, secs, mins;int f=0;


#define SIM800_TX_PIN 8
#define SIM800_RX_PIN 7
#define trigPin1 9
#define echoPin1 10
SoftwareSerial mySerial(SIM800_TX_PIN, SIM800_RX_PIN);
int i1 = 5;
int i2 = 6;
int s1,s2,buzz=11;
int counter = 3,flag=0,iflag=0;

float duration, distance, UltraSensor;
float perc;
TinyGPSPlus gps;

// setup for ir2
void setup()
{
Serial.begin (9600);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(i1, INPUT);
  pinMode(i2, INPUT);
  pinMode(buzz,OUTPUT);
  
  Serial.begin(9600);
  while(!Serial);
  mySerial.begin(9600);
  delay(500);
   GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
  delay(100);
  // put your setup code here, to run once:
}
// setup for ir2 end
 
// setup for ultrasonic
void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
delay(100);
}


void SendTextMessage()
{
  mySerial.println("AT+CMGF=1\r");
  delay(2000);
  mySerial.println("AT+CMGS=\"+918291742174\"\r\n");
  delay(2000);
  mySerial.println("Please clean the garbage bin.It is 80% full !!!");
  delay(200);
  mySerial.println((char)26);
  delay(5000);
  return;
}
void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}

void sendmsg(double lat,double lon,int per)
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918291742174\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(lat,6);// The SMS text you want to send
  mySerial.print(" ");
  mySerial.print(lon,6);
  mySerial.print(" ");
  mySerial.print(per);
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
   delay(100);
}

 void SendMessage(double lat,double lon)
{
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918291742174\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(lat,6);// The SMS text you want to send
  mySerial.print(" ");
  mySerial.print(lon,6);
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z

    Serial.println("SMS sent!!");
  delay(1000);
}

 void RecieveMessage()
{
   Serial.println("Recieving sms!!");
 mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
 delay(1000);

  Serial.println("SMS recieved!!");
  
}
 




void loop() {

smartDelay(1000); /* Generate precise delay of 1ms */
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */
        if (!loc_valid)
        {          
          /*Serial.print("Latitude : ");
          Serial.println("*****");
          Serial.print("Longitude : ");
          Serial.println("*****");*/
          
        }
        else
        {
          if(iflag==0){
          DegMinSec(lat_val);
          Serial.print("Latitude in Decimal Degrees : ");
          Serial.println(lat_val, 6);
          Serial.print("Latitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
          DegMinSec(lng_val);
          Serial.print("Longitude in Decimal Degrees : ");
          Serial.println(lng_val, 6);
          Serial.print("Longitude in Degrees Minutes Seconds : ");
          Serial.print(degree);         
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
          iflag=1;
          }
//**********
SonarSensor(trigPin1, echoPin1);
UltraSensor = distance;
//Serial.print("distance:\t");
//Serial.print(distance);
perc=(distance/22.1)*100;
perc=100-perc;
//Serial.print("percentage:\t");
//Serial.print(perc);
//**********
  if(counter == 3 && digitalRead(i1) == LOW)
  {
    delay(1000);
    for(int i=1; i<=5; i++)
    {
      //s1= digitalRead(i1);
      if(digitalRead(i1) == LOW)
      delay(500);
    }
    if(digitalRead(i1) == LOW)
    {
      Serial.println(" the bin is 40 % full\n\n");
      int per=40;
      sendmsg(lat_val,lng_val,per);
    counter--;
    }
  }
    
  if(counter == 2 && digitalRead(i1)== LOW && digitalRead(i2) == LOW)
  {
    delay(1000);
    for(int i=1; i<=5; i++)
    {
      //s2= digitalRead(i1);
      if(digitalRead(i2) == LOW)
      delay(500);
    }
    if(digitalRead(i2) == LOW)
    {
      Serial.println(" Now the bin is 80 % full\n\n");
      int per=80;
      sendmsg(lat_val,lng_val,per);
    //tone(buzz, 1000,1000);
    counter--;
    }
  
  }

if((!(perc<=100 && perc>=80)||(perc<=100 && perc>=80)) && counter==1 && flag==0)
{
  Serial.println("Sending SMS :)");
  flag=1;
   SendTextMessage();
    sendmsg(lat_val,lng_val,80);
}
else
//Serial.println("SMS Not Sent!!! Require more details;)");

  if(counter ==1 && digitalRead(i1) == HIGH && digitalRead(i2) == HIGH)
    {
      Serial.print(" The bin is now empty \n\n");
      counter = 3;
      flag=0;iflag=0;
      }
  // put your main code here, to run repeatedly:
/*
//**********
if (flag==1){
Serial.println("distance:");
Serial.println(UltraSensor);
Serial.println("percentage:");
Serial.println(perc);}
delay(1000);
//**********
*/

if (mySerial.available()>0)
   Serial.write(mySerial.read());
}
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}
/*
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
   /*   gps.encode(GPS_SoftSerial.read());
///* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
/*  } while (millis() - start < ms);*/
//}*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
