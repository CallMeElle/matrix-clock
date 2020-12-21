#pragma once

//create a cpp file whiche defines the value of 'const char* pwd::ssid' and 'const char* pwd::password'
//make sure to add it to the gitignore file to prevent your wifi credentials from leaking
class pwd{
public:
    static const char* ssid; //Set it to the name of your Wifi Network
    static const char* password; //Set this to the password for that Network
};
