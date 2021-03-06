#include "digits.hpp"

#define CS_topdisplay 15

#define CS_bottomdisplay 25

#define Segments 4

controller_configuration<Segments,2> config;
LedController<Segments,2> display = LedController<Segments,2>();

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
      
  display.displayOnSegment(0,1,digits[places[3-0]]);
  output = digits[places[3-1]] << 1;
  if(time.second % 2) output = output & generateDots(false);
  display.displayOnSegment(1,1,output);

  display.displayOnSegment(3,1,digits[places[3-3]]);

  output = digits[places[3-2]] >> 1;
  if(time.second % 2) output = output & generateDots();
  display.displayOnSegment(2,1, output);  

  Serial.println("Displaying bottom row");
  
  places[0] = (time.month + 1) % 10;
  places[1] = (time.month + 1) / 10 % 10;
  places[2] = time.day % 10;
  places[3] = time.day / 10 % 10;

  Serial.println("displaying segment 0");
  display.displayOnSegment(0,0,digits[places[3]]);
  
  Serial.println("displaying segment 1");
  output = digits[places[2]] << 1;
  output = output & generateDots(false,false);
  display.displayOnSegment(1,0, output);

  Serial.println("displaying segment 3");
  display.displayOnSegment(3,0,digits[places[0]]);
         
  Serial.println("displaying segment 2");
  output = digits[places[1]] >> 1;
  output = output & generateDots(true,false);
  display.displayOnSegment(2,0, output); 

  Serial.println("finished displaying");

  //update time to full hour
  if(time == Time_format(16,0,0)){
    timer.updateTime();
    
    display.refreshSegments();
  }

  Serial.println("finished updating time");
}

void setup(){
  Serial.begin(115200);
  Serial.println("Start of setup");

  config.row_SPI_CS[1] = CS_topdisplay;
  config.row_SPI_CS[0] = CS_bottomdisplay;

  config.useHardwareSpi = true;
  config.virtual_multi_row = false;

  config.spiTransferSpeed = 10 * controllerSpeedKHZ;
  //config.debug_output = true;

  display.init(config);

  assert(display.isInitilized());

  Serial.println("Controller initilized");

  display.activateAllSegments();

  display.setIntensity(0);

  Serial.println("Matrix reset");

  for(unsigned int i = 0;i < 4;i++){
    display.displayOnSegment(3-i,1,digits[0]);    
  } 

  //delay(10000);

  timer.init();

}

void loop(){
  Serial.println("\nStarted loop\n");
  delay(500);
  printLocalTime();
}
