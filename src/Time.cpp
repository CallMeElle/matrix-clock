#include "Time.hpp"

Time::Time() noexcept{}

void Time::init() noexcept{
    Serial.printf("Connecting to %s ", pwd::ssid);
    WiFi.begin(pwd::ssid, pwd::password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    
    Serial.println("Getting Time");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("turning wifi off");
    //disconnect WiFi as it's no longer needed
    delay(1000);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

Time_format Time::getTime() noexcept{
    struct tm timeinfo;
    Time_format currentTime;

    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return currentTime;
    }

    currentTime.day = timeinfo.tm_mday;
    currentTime.month = timeinfo.tm_mon;
    currentTime.hour = timeinfo.tm_hour;
    currentTime.minute = timeinfo.tm_min;
    currentTime.second = timeinfo.tm_sec; 
    currentTime.year = timeinfo.tm_year;

    return currentTime;
}

void Time::updateTime() noexcept{
    //connect to WiFi if not connected
    Serial.printf("Connecting to %s ", pwd::ssid);
    WiFi.begin(pwd::ssid, pwd::password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    
    
    //init and get the time
    Serial.println("Getting Time");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    //disconnect WiFi as it's no longer needed
    delay(1000);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}