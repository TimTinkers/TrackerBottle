#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13
#define STRIPSIZE 12
#define USE_SERIAL Serial

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);

// WiFi client setup.
ESP8266WiFiMulti WiFiMulti;

// FSR setup.
int FSR_PIN = A0;
int FSR_READING;

void setup() {
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  // Initialize the display to all red.
  strip.begin();
  strip.setBrightness(5);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(64, 0, 0), 100); // Red
  
  for(uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.println("[SETUP] WAIT " + t);
    USE_SERIAL.flush();
    delay(1000);
  }
  
  WiFiMulti.addAP("Bill Nye the Wifi Guy", "trumpwinsallthetime");
}

void loop() {
  // Wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {

    // Upon connection, set lights to green.
    USE_SERIAL.println("[HTTP] begin.");
    colorWipe(strip.Color(0, 64, 0), 100); // Green

    // Get the force sensor read-out.
    FSR_READING = analogRead(FSR_PIN);  
    Serial.print("Analog reading = ");
    Serial.print(FSR_READING);
    
    // Set the light color based on pressure reading.
    if (FSR_READING < 10) {
      colorWipe(strip.Color(0, 64, 0), 100); // Green
      Serial.println(" - No pressure");
    } else if (FSR_READING < 200) {
      colorWipe(strip.Color(0, 0, 64), 100); // Blue
      Serial.println(" - Light touch");
    } else if (FSR_READING < 500) {
      colorWipe(strip.Color(64, 0, 64), 100); // Purple
      Serial.println(" - Light squeeze");
    } else if (FSR_READING < 800) {
      colorWipe(strip.Color(255, 215, 0), 100); // Gold
      Serial.println(" - Medium squeeze");
    } else {
      colorWipe(strip.Color(255, 128, 0), 100); // Orange
      Serial.println(" - Big squeeze");
    }
    
    // Send the HTTP GET Request.
    HTTPClient http;
    String readingString = String(FSR_READING);
    String dataURL = "http://104.131.79.67:3000/setWeight?weight=" + readingString;
    Serial.println(dataURL);
    http.begin(dataURL);
    int httpCode = http.GET();
    
    // httpCode will be negative on error.
    if (httpCode > 0) {
      
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
  }

  // Send every five seconds.
  delay(5000);
}

// Fill the dots one after the other with a color.
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
