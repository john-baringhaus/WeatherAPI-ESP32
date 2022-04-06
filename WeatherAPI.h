#pragma once 

#ifndef WEATHERAPI
#define WEATHERAPI

#include "OpenWeatherMapCurrent.h"

#define SECONDS_BETWEEN_UPDATES 900

class WeatherAPI {

  private:

    TaskHandle_t _task;

    uint16_t _updateInterval;
    uint32_t _weatherAge = 0;
    bool _useMetric = false;
    String _location, _secret;

    OpenWeatherMapCurrentData _data;

    static void startTaskImpl(void*);
    void task();

  public:
    WeatherAPI(String location, String apiKey, uint16_t interval = SECONDS_BETWEEN_UPDATES);

    void refreshWeatherData();

    void start();
    void stop();

    void setConditions(OpenWeatherMapCurrentData data);
    void metric(bool metric);

    OpenWeatherMapCurrentData* getData();
    uint32_t getAge();

    

    void printWeatherToConsole();

};
#endif