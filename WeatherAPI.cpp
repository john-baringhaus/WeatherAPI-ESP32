#include "WeatherAPI.h"
#include <ctime>
#include <WiFi.h>

WeatherAPI::WeatherAPI(String location, String apiKey, uint16_t interval) : _location(location), _secret(apiKey), _updateInterval(interval) {
}


void WeatherAPI::refreshWeatherData() {
  //Abort if not connected to wifi
  if(WiFi.status() != WL_CONNECTED /*|| (_weatherAge + _updateInterval * 1000 > millis() && _weatherAge)*/ ) {
    return;
  }

  OpenWeatherMapCurrentData temp; 
  OpenWeatherMapCurrent client;
  client.setLanguage("en");
  client.setMetric(_useMetric);
  client.updateCurrent(&temp, _secret, _location);
  _data = temp;
  _weatherAge = millis();

}

void WeatherAPI::setConditions(OpenWeatherMapCurrentData data) {
  _data = data;
}

void WeatherAPI::metric(bool metric) {
  _useMetric = metric;
}

OpenWeatherMapCurrentData* WeatherAPI::getData() {
  return &_data;
}

uint32_t WeatherAPI::getAge() {
  return millis() - _weatherAge;
}

void WeatherAPI::task(){
     while(1){
        this->refreshWeatherData();
        vTaskDelay( this->_updateInterval * 1000 );
     }
}

void WeatherAPI::startTaskImpl(void* _this){
    static_cast<WeatherAPI*>(_this)->task();
}
void WeatherAPI::start(){
    xTaskCreatePinnedToCore(this->startTaskImpl, "Update Weather Task", 4096, this, 1, &_task, 0);
}

void WeatherAPI::stop() {
  vTaskSuspend( _task );
}

void WeatherAPI::printWeatherToConsole() {

  Serial.println("------------------------------------");

  // "lon": 8.54, float lon;
  Serial.printf("lon: %f\n", _data.lon);
  // "lat": 47.37 float lat;
  Serial.printf("lat: %f\n", _data.lat);
  // "id": 521, weatherId weatherId;
  Serial.printf("weatherId: %d\n", _data.weatherId);
  // "main": "Rain", String main;
  Serial.printf("main: %s\n", _data.main.c_str());
  // "description": "shower rain", String description;
  Serial.printf("description: %s\n", _data.description.c_str());
  // "icon": "09d" String icon; String iconMeteoCon;
  Serial.printf("icon: %s\n", _data.icon.c_str());
  Serial.printf("iconMeteoCon: %s\n", _data.iconMeteoCon.c_str());
  // "temp": 290.56, float temp;
  Serial.printf("temp: %f\n", _data.temp);
  // "pressure": 1013, uint16_t pressure;
  Serial.printf("pressure: %d\n", _data.pressure);
  // "humidity": 87, uint8_t humidity;
  Serial.printf("humidity: %d\n", _data.humidity);
  // "temp_min": 289.15, float tempMin;
  Serial.printf("tempMin: %f\n", _data.tempMin);
  // "temp_max": 292.15 float tempMax;
  Serial.printf("tempMax: %f\n", _data.tempMax);
  // "wind": {"speed": 1.5}, float windSpeed;
  Serial.printf("windSpeed: %f\n", _data.windSpeed);
  // "wind": {"deg": 1.5}, float windDeg;
  Serial.printf("windDeg: %f\n", _data.windDeg);
  // "clouds": {"all": 90}, uint8_t clouds;
  Serial.printf("clouds: %d\n", _data.clouds);
  // "dt": 1527015000, uint64_t observationTime;
  time_t time = _data.observationTime;
  Serial.printf("observationTime: %d, full date: %s", _data.observationTime, ctime(&time));
  // "country": "CH", String country;
  Serial.printf("country: %s\n", _data.country.c_str());
  // "sunrise": 1526960448, uint32_t sunrise;
  time = _data.sunrise;
  Serial.printf("sunrise: %d, full date: %s", _data.sunrise, ctime(&time));
  // "sunset": 1527015901 uint32_t sunset;
  time = _data.sunset;
  Serial.printf("sunset: %d, full date: %s", _data.sunset, ctime(&time));

  // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", _data.cityName.c_str());
  Serial.println();
  Serial.println("---------------------------------------------------/\n");
}
