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

ByteBlock generateDots(bool left = true , bool alsoTop = true){
  ByteBlock ret = ByteBlock();
  unsigned int shift = left ? 0 : 7;

  ret[1] |= 0x01 << shift;
  ret[2] |= 0x01 << shift;

  if (alsoTop){
      ret[5] |= 0x01 << shift;
      ret[6] |= 0x01 << shift;
  }

  return ret;
}

void printLocalTime(){
  Serial.println("\n printing the time");

  /*if(!topdisplay.isInitilized() || !bottomdisplay.isInitilized()){
    Serial.println("controller not initilized");
    return;
  }*/

  auto time = timer.getTime();
  
  Serial.println("local time updated");

  unsigned int places[4] = {0,0,0,0};
  ByteBlock output = {0,0,0,0,0,0,0,0};

  places[0] = time.minute % 10;
  places[1] = time.minute / 10 % 10;
  places[2] = time.hour % 10;
  places[3] = time.hour / 10 % 10;

  Serial.println("Displaying top row");
      
  topdisplay.displayOnSegment(0,digits[places[3-0]]);
  output = digits[places[3-1]] << 1;
  if(time.second % 2) output = output & generateDots(false);
  topdisplay.displayOnSegment(1,output);

  topdisplay.displayOnSegment(3,digits[places[3-3]]);

  output = digits[places[3-2]] >> 1;
  if(time.second % 2) output = output & generateDots();
  topdisplay.displayOnSegment(2, output);  

  Serial.println("Displaying bottom row");
  
  places[0] = (time.month + 1) % 10;
  places[1] = (time.month + 1) / 10 % 10;
  places[2] = time.day % 10;
  places[3] = time.day / 10 % 10;

  Serial.println("displaying segment 0");
  bottomdisplay.displayOnSegment(0,digits[places[3]]);
  
  Serial.println("displaying segment 1");
  output = digits[places[2]] << 1;
  output = output & generateDots(false,false);
  bottomdisplay.displayOnSegment(1, output);

  Serial.println("displaying segment 3");
  bottomdisplay.displayOnSegment(3,digits[places[0]]);
         
  Serial.println("displaying segment 2");
  output = digits[places[1]] >> 1;
  output = output & generateDots(true,false);
  bottomdisplay.displayOnSegment(2, output); 

  Serial.println("finished displaying");

  //update time to full hour
  if(time == Time_format(16,0,0)){
    timer.updateTime();
    
    topdisplay.refreshSegments();
    bottomdisplay.refreshSegments();
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
