/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"
#define BLYNK_TEMPLATE_ID "TMPL-xh6T_7x"
#define BLYNK_DEVICE_NAME "Microcontroller ESP 32"
#define BLYNK_AUTH_TOKEN "G3nR-vaqiNMB4srk_vy-bBuuGKIc-2RQ"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "G3nR-vaqiNMB4srk_vy-bBuuGKIc-2RQ";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Ibrahim";
char pass[] = "22222222";

//include library (DHT)
#include "DHT.h"
#include <Adafruit_Sensor.h>

//Temperature sensor pin & type
#define DHT_PIN 4
#define DHT_TYPE DHT22

//DHT function for pin and type (DHT22)
DHT dht(DHT_PIN, DHT_TYPE);

//LED Pin
const int RED_LED_PIN = 25;
const int YELLOW_LED_PIN = 23;

//pin for Ultrasonic Sensor
const int trigPin = 5;
const int echoPin = 18;

#define SPEED_OF_SOUND 0.034

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  //ultrasonic pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //LED Pin
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  //dht22
  dht.begin();

  //blynk  
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  //variables  
  const int total_height = 15;
  int duration, var_height, water_level;

  //Variables for DHT22 sensor 
  float temperature, humidity;
  
  //intializing variables with zeros
  var_height = 0, duration = 0;

  //for statement and mean value
  for (int i=0; i<10; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    //sum of vaiation in height in each time in the loop
    var_height += (duration * SPEED_OF_SOUND)/2;
  }
  
  //one distinct value of var_height as it is divided by 10 from its original value
  var_height = var_height/10;
  //water level as it is the difference between the total_height (15 cm) and the var_height
  water_level = total_height - var_height;
  
  //dht22 readings for temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  //Warning Conditions for turning on or off the LEDs
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW); 
  if (var_height <=3){
      digitalWrite(RED_LED_PIN, HIGH);
      delay(2000);
      digitalWrite(RED_LED_PIN, LOW);
  }
  else if (var_height <=7 && var_height >3){
    digitalWrite(YELLOW_LED_PIN, HIGH);
    delay(2000);  
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  //Blynk sending data 
  Blynk.virtualWrite(V0, water_level);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.run();

  //Print on serial monitor 
  Serial.print("Height Variance: ");
  Serial.print(var_height);
  Serial.print("cm\tWater Level: ");
  Serial.print(water_level);
  Serial.print("cm\tTemperature: ");
  Serial.print(temperature);
  Serial.print(" Celsius\tHumidity: ");
  Serial.print(humidity);
  Serial.print("%\n");
  delay(3000);  

}

