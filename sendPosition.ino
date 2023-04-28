#include "Ubidots.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 33,TXPin = 32;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial GPS(RXPin, TXPin);


const char* UBIDOTS_TOKEN = "BBFF-r0r23J4YrwPJyACYDZVq0PE9ZoRxI8";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Real Me Narzo 30 Pro";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "12345678";      // Put here your Wi-Fi password

Ubidots ubidots(UBIDOTS_TOKEN, UBI_TCP);

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);

  GPS.begin(GPSBaud);
  // ubidots.setDebug(true); //Uncomment this line for printing debug messages
}

void loop() {
  float value = random(0, 9) * 10;

  while(GPS.available() > 0)
  {
    gps.encode(GPS.read());
    if (gps.location.isUpdated())
    {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    Serial.print(latitude);
    Serial.print(longitude);
  

  /* Reserves memory to store context key values, add as much as you need */
  char* str_lat = (char*)malloc(sizeof(char) * 10);
  char* str_lng = (char*)malloc(sizeof(char) * 10);

  /* Saves the coordinates as char */
  sprintf(str_lat, "%f", latitude);
  sprintf(str_lng, "%f", longitude);

  /* Reserves memory to store context array */
  char* context = (char*)malloc(sizeof(char) * 30);

  /* Adds context key-value pairs */
  ubidots.addContext("lat", str_lat);
  ubidots.addContext("lng", str_lng);

  /* Builds the context with the coordinates to send to Ubidots */
  ubidots.getContext(context);

  /* Sends the position */
  ubidots.add("position", value, context);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id
  
  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  Serial.println(str_lat);
  Serial.println(str_lng);
  /* free memory */
  free(str_lat);
  free(str_lng);
  free(context);
  delay(4000);
  }
}
}
