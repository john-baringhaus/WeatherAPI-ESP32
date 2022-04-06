
#include <WiFi.h>
#include <esp_wifi.h>  
#include <Update.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include "WeatherAPI.h"

TaskHandle_t Task2;

WiFiManager wifiManager;

WeatherAPI wx = WeatherAPI("New York City,NY,US", "XXX");

void setup() {
  Serial.begin(115200);
  
  wifiManager.autoConnect("Lamp");
  delay(1500);

  wx.start();

  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);
  delay(500);
}


void Task2code(void* parameter) {

  for (;;) {
    Serial.printf("Task2 is running on core %d\n", xPortGetCoreID());
    wx.printWeatherToConsole();
    delay(5000);
  }
}

void loop() {
}