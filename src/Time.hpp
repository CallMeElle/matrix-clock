#pragma once

#define TZ_INFO  "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"

/**
 * wifi_pwd.hpp & wifi_pwd.cpp define the wifi login by declaring two constant char arrays.
 * const char* pwd::ssid     = "some ssid"     //the ssid of the router.
 * const char* pwd::password = "some password" //the password needed to log into the wifi network.
 */
#include "wifi_pwd.hpp"
#include <WiFi.h>
#include "time.h"


class Time_format{
public:
    unsigned int hour = 0;
    unsigned int minute = 0;
    unsigned int second = 0;
    unsigned int month = 0;
    unsigned int day = 0;
    unsigned int year = 0;
};

class Time{
private:
    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 3600; //3600s = 1 hour -> CET(Central European Time)
    const int   daylightOffset_sec = 3600; //This adds 3600s = 1 hour to the timezone -> CEST(Central European Summer Time)
    const bool summer = false; //This is true if it is summer otherwise it should be false.
    
public:
    Time() noexcept;
    void init() noexcept;
    Time_format getTime() noexcept;
    void updateTime() noexcept;
};
