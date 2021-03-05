#include "digits.hpp"

#define CS_topdisplay 15

#define CS_bottomdisplay 25

#define Segments 4

controller_configuration<Segments,1> conf_top;
controller_configuration<Segments,1> conf_bottom;
LedController<Segments,1> topdisplay = LedController<Segments,1>();
LedController<Segments,1> bottomdisplay = LedController<Segments,1>();

#include "Time.hpp"

Time timer = Time();

const int controllerSpeedKHZ = 8000;

ByteBlock displayDots(const ByteBlock& data, bool left = true , bool alsoTop = true){
  ByteBlock ret = data;

  if(left){
    ret[1] |= 0x01;
    ret[2] |= 0x01;

    if(alsoTop){
      ret[5] |= 0x01;
      ret[6] |= 0x01;
    }    
  }else{
    ret[1] |= 0x80;
    ret[2] |= 0x80;

    if(alsoTop){
      ret[5] |= 0x80;
      ret[6] |= 0x80;
    }
  }

  return ret;
}

void printLocalTime(){
  Serial.println("\n printing the time");

  /*if(!topdisplay.isInitilized() || !bottomdisplay.isInitilized()){
    Serial.println("controller not initilized");
    return;
  }*/

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  Serial.println("local time updated");

  unsigned int places[4] = {0,0,0,0};
  ByteBlock output = {0,0,0,0,0,0,0,0};

  places[0] = timeinfo.tm_min % 10;
  places[1] = timeinfo.tm_min / 10 % 10;
  places[2] = timeinfo.tm_hour % 10;
  places[3] = timeinfo.tm_hour / 10 % 10;

  Serial.println("Displaying top row");
      
  topdisplay.displayOnSegment(0,digits[places[3-0]]);

  output = digits[places[3-1]] << 1;
  if(timeinfo.tm_sec % 2) output = displayDots(output,false);
  topdisplay.displayOnSegment(1,output);

  topdisplay.displayOnSegment(3,digits[places[3-3]]);

  output = digits[places[3-2]] >> 1;
  if(timeinfo.tm_sec % 2) output = displayDots(output);
  topdisplay.displayOnSegment(2, output);  

  Serial.println("Displaying bottom row");
  
  places[0] = (timeinfo.tm_mon + 1) % 10;
  places[1] = (timeinfo.tm_mon + 1) / 10 % 10;
  places[2] = timeinfo.tm_mday % 10;
  places[3] = timeinfo.tm_mday / 10 % 10;

  Serial.println("displaying segment 0");
  bottomdisplay.displayOnSegment(0,digits[places[3]]);
  
  Serial.println("displaying segment 1");
  output = digits[places[2]] << 1;
  output = displayDots(output,false,false);
  bottomdisplay.displayOnSegment(1, output);

  Serial.println("displaying segment 3");
  bottomdisplay.displayOnSegment(3,digits[places[0]]);
         
  Serial.println("displaying segment 2");
  output = digits[places[1]] >> 1;
  output = displayDots(output,true,false);
  bottomdisplay.displayOnSegment(2, output); 

  Serial.println("finished displaying");

  static bool RefreshMatrix = true; 

  //update time to full hour
  if(timeinfo.tm_min == 0 && timeinfo.tm_hour == 16 && timeinfo.tm_sec == 0){
    timer.updateTime();
  } 

  if (!RefreshMatrix && timeinfo.tm_min %5 == 1) {
    RefreshMatrix = true;
  }

  if (timeinfo.tm_min %5 == 0 && timeinfo.tm_sec == 0 && RefreshMatrix) {
    topdisplay.refreshSegment(1);
    topdisplay.refreshSegment(2);

    if (timeinfo.tm_min %30 == 0){
      topdisplay.refreshSegment(0);
      topdisplay.refreshSegment(3);
      bottomdisplay.refreshSegments();
    }
    
    RefreshMatrix = false;
  }

  Serial.println("finished updating time");
}

void setup(){
  Serial.begin(115200);
  Serial.println("Start of setup");

  conf_top.SPI_CS = CS_topdisplay;
  conf_bottom.SPI_CS = CS_bottomdisplay;

  conf_top.useHardwareSpi = true;
  conf_bottom.useHardwareSpi = true;

  conf_top.spiTransferSpeed = 10 * controllerSpeedKHZ;
  conf_bottom.spiTransferSpeed = 10 * controllerSpeedKHZ;

  //conf_top.onlySendOnChange = false;
  //conf_bottom.onlySendOnChange = false;

  topdisplay.init(conf_top);
  bottomdisplay.init(conf_bottom);

  //assert(topdisplay.isInitilized() && bottomdisplay.isInitilized());

  Serial.println("Controller initilized");

  topdisplay.activateAllSegments();
  bottomdisplay.activateAllSegments();

  topdisplay.setIntensity(0);
  bottomdisplay.setIntensity(0);

  bottomdisplay.clearMatrix();

  Serial.println("Matrix reset");

  /*Serial.println("Rotated ByteBlocks:");

  for(unsigned int i = 0;i<10;i++){
    for(unsigned int j = 0;j<8;j++){
      Serial.printf("\n seg: %i, row %i: %i",i,j,digits[i][j]);
    }
  }*/

  for(unsigned int i = 0;i < 4;i++){
    topdisplay.displayOnSegment(3-i,digits[0]);    
  } 

  //delay(10000);

  timer.init();

}

void loop(){
  Serial.println("\nStarted loop\n");
  delay(500);
  printLocalTime();
}
