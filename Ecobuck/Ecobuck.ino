
#include <Bonezegei_DHT11.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//param = DHT11 signal pin
Bonezegei_DHT11 dht(2);

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

  dht.begin();
}

void loop() {

  if (dht.getData()) {                         // get All data from DHT11
    float tempDeg = dht.getTemperature();      // return temperature in celsius
    float tempFar = dht.getTemperature(true);  // return temperature in fahrenheit if true celsius of false
    int hum = dht.getHumidity();               // return humidity
    Serial.print("\nHumidity:");
    Serial.print(hum);
     Serial.print("\nTemperature ");
    Serial.print(tempDeg);
     Serial.print("\nTemperature F:");
    Serial.print(tempFar);
    
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Humidity = ");
  display.print(hum);
  display.setCursor(0, 15);
  display.print("Temperature = ");
  display.print(tempDeg);
  display.print(" C");
  display.setCursor(0, 30);
  display.print("Temperature = ");
  display.print(tempFar);
  display.print(" F");

  display.display(); 

  }
  delay(2000);  //delay atleast 2 seconds for DHT11 to read tha data
}
