
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TO_TOPIC_0 "AWS_Python"
#define AWS_IOT_SUBSCRIBE_TO_TOPIC_0   "ESP32_01"

#define DHT11_PIN 16 //(Rx2)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA(21), SCL(22) pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//param = DHT11 signal pin
DHT dht(DHT11_PIN, DHT11);

void DHT_Sensor_Serial(float humidity, float TempDeg);
void Oled_Display(float humidity, float TempDeg);

void Wifi_AWS_connection() ;
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
void Data_From_ESP32_TO_AWS(float humidity,float Temperature);

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  Wifi_AWS_connection();
  client.subscribe(AWS_IOT_PUBLISH_TO_TOPIC_0);

  dht.begin();
}

void loop() {

    client.loop(); //when data is publish from AWS it call Data_From_AWS_Publis function
    // Read data from Sensors                  
    // float temperature_C = dht.readHumidity() ;    // return temperature in celsius
    // float humidity = dht.readHumidity();              // return humidity
    
    float temperature_C = random(1,100);    // return temperature in celsius
    float humidity = random(1,100);              // return humidity


    //Serial Print
    DHT_Sensor_Serial(humidity,temperature_C);
    //Send data to Cloud 
    Data_From_ESP32_TO_AWS(humidity,temperature_C);
    //Lcd Print
    Oled_Display(humidity,temperature_C);
    //delay atleast 2 seconds for DHT11 to read tha data
    delay(5000);  

  
  
}



void Data_From_AWS_Publish(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Incoming Topic: ");
  Serial.println(topic);
  for(int i=0 ;i<length;i++)
  {
    Serial.print((char)payload[i]);
  }
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  String Data = doc["message"];
  Serial.print("Incoming Data: ");
  Serial.println(Data);
 // int r1 = Relay1.toInt();
  
}


void Data_From_ESP32_TO_AWS(float humidity,float Temperature) {
  StaticJsonDocument<200> doc;
  doc["TEMP"] = Temperature;
  doc["HUMID"] = humidity; // Add the HUMID key with a random value
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_SUBSCRIBE_TO_TOPIC_0, jsonBuffer);
}


void Wifi_AWS_connection()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WIFI is Connected With IP address");
  Serial.println(WiFi.localIP());
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(Data_From_AWS_Publish);


  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(200);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  Serial.println("AWS IoT Connected!");
}

void DHT_Sensor_Serial(float humidity, float TempDeg)
{
    Serial.print("\nHumidity:");
    Serial.print(humidity);
    Serial.print("\nTemperature ");
    Serial.print(TempDeg);
}
void Oled_Display(float humidity, float TempDeg)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Humidity = ");
    display.print(humidity);
    display.setCursor(0, 15);
    display.print("Temperature = ");
    display.print(TempDeg);
    display.print(" C");
    display.setCursor(0, 30);
    display.display(); 
}