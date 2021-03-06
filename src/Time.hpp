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
    Time_format(){};
    Time_format(unsigned int _hour, unsigned int _minute, unsigned int _second):hour(_hour),minute(_minute),second(_second){};
    bool operator==(const Time_format& other){
        bool ret = hour == other.hour;
        ret = ret && (minute == other.minute);
        ret = ret && (second == other.second);

        if( !(year == 0 && month == 0 && day == 0) && !(other.year == 0 && other.month == 0 && other.day == 0) ){
            ret = ret && (year == other.year);
            ret = ret && (month == other.month);
            ret = ret && (day == other.day);
        }

        return ret;
    }

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
