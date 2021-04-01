#include <Bridge.h>
#include <HttpClient.h>

String value_1 = "0";
String value_2 = "0";
String value_3 = "0";
String value_4 = "0";

//parameters for 2nd sensor.
const int trig_1 = 7;
const int echo_1 = 6;

//parameters for 1st sensor.
const int trigPin = 10;
const int echoPin = 8;

//parameters for dust sensor.
int measurePin = 5;
int ledPower = 12;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
long duration_1;
int distance_1 = 0;
long duration; int distance = 0;

void setup() {
 Serial.begin(9600);
 pinMode(ledPower,OUTPUT);
 pinMode(trig_1, OUTPUT);
 pinMode(echo_1, INPUT);  
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 
 //parameter for LED.
 pinMode(2, OUTPUT);

 //code for php starts from here in setup.
 pinMode(13, OUTPUT);
 digitalWrite(13, LOW);
 Bridge.begin();
 digitalWrite(13, HIGH);
  
 //SerialUSB.begin(9600);
  
 //while (!SerialUSB); // wait for a serial connection
 //code for the php finishes here in setup.
}

void loop() {
//code for php request start from here.
// Initialize the client library
  HttpClient client;
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;

  //Printing the Distance measured by the sensor.
  Serial.print("Dsiatance from the 1st sensor is : ");
  Serial.println(distance);

  //Adding the condition for LED.
  if(distance <= 5){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(100);  
  }
  
 digitalWrite(trig_1, LOW);
 delayMicroseconds(2);

 digitalWrite(trig_1, HIGH);
 delayMicroseconds(10);
 digitalWrite(trig_1, LOW);

 duration_1 = pulseIn(echo_1, HIGH);
  distance_1= duration_1*0.034/2;

  Serial.print("Distance from 2nd sensor is : ");
  Serial.println(distance_1);

  //Adding the condition for LED.
  if(distance_1 <= 5){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(100);  
  }

 delay(1000);

   digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  
  calcVoltage = voMeasured * (5.0 / 1024);
 
  
  dustDensity = (0.17 * calcVoltage - 0.1)*1000;
 
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
 
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
 
  Serial.print(" - Dust Density [ug/m3]: ");
  Serial.println(dustDensity);

   //Adding the condtion for LED.
   if(dustDensity > 200){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(100);
    
    //Serial.println("Dust level is rising.");
 
  }

   // String url = "http://musaronaldo16.000webhostapp.com/check.php?value_1="+value_1+"&value_2="+value_2;
  String url2 = "http://musaronaldo16.000webhostapp.com/check.php?value_1="+String(dustDensity)+"&value_2="+String(distance)+"&value_3="+String(distance_1)+"&value_4="+value_4;
  
  // Make a HTTP request:
  client.get(url2);
  //code for php request finishes here.
  
  
  delay(1000);
}
