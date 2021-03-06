#include "digits.hpp"

#define CS_topdisplay 15

#define CS_bottomdisplay 25

#define Segments 4

#define delayTime 200 // Delay between Frames

controller_configuration conf_top;
controller_configuration conf_bottom;
LedController topdisplay;  
LedController bottomdisplay;

#include "Time.hpp"

Time timer = Time();

const int controllerSpeedKHZ = 8000;

ByteBlock* digits;

void shiftleft (ByteBlock data, ByteBlock* result){
  if(result == nullptr){
    return;
  }

  for(unsigned int row=0;row < 8;row++){
      (*result)[row] = data[row]>>1;
  }
}

void shiftright (ByteBlock data, ByteBlock* result){
    if(result == nullptr){
    return;
  }

  for(unsigned int row=0;row < 8;row++){
      (*result)[row] = data[row]<<1;
  }
}

void displayDots(ByteBlock* data, bool left = true , bool alsoTop = true){
  if (data == nullptr){
    return;
  }

  if(left){
    (*data)[1] |= 0x01;
    (*data)[2] |= 0x01; 

    if(alsoTop){
      (*data)[5] |= 0x01;
      (*data)[6] |= 0x01;
    }    
  }else{
    (*data)[1] |= 0x80; 
    (*data)[2] |= 0x80; 

    if(alsoTop){
      (*data)[5] |= 0x80;
      (*data)[6] |= 0x80;
    }
  }
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
  
  shiftleft(digits[places[3-1]], &output);
  if(time.second % 2) displayDots(&output,false);
  topdisplay.displayOnSegment(1,output);

  topdisplay.displayOnSegment(3,digits[places[3-3]]);
  
  shiftright(digits[places[3-2]], &output);
  if(time.second % 2) displayDots(&output);
  topdisplay.displayOnSegment(2, output);  

  Serial.println("Displaying bottom row");
  
  places[0] = (time.month + 1) % 10;
  places[1] = (time.month + 1) / 10 % 10;
  places[2] = time.day % 10;
  places[3] = time.day / 10 % 10;

  Serial.println("displaying segment 0");
  bottomdisplay.displayOnSegment(0,digits[places[3]]);
  
  Serial.println("displaying segment 1");
  shiftleft(digits[places[2]], &output);
  displayDots(&output,false,false);
  bottomdisplay.displayOnSegment(1, output);

  Serial.println("displaying segment 3");
  bottomdisplay.displayOnSegment(3,digits[places[0]]);
         
  Serial.println("displaying segment 2");
  shiftright(digits[places[1]], &output);
  displayDots(&output,true,false);
  bottomdisplay.displayOnSegment(2, output); 

  Serial.println("finished displaying");

  //update time to full hour
  if(time == Time_format(16,0,0)){
    timer.updateTime();
    
    topdisplay.refreshSegments();
    bottomdisplay.refreshSegments();
  }

  Serial.println("finished updating time");

  return;
}

//sets all rows on all displays to 0
void switchLED(){
  static bool LEDON = false;
  if(LEDON){
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }
  LEDON = !LEDON;
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

  pinMode(13, OUTPUT);

  digits = new ByteBlock[10];
    
  for(unsigned int i = 0;i < 10;i++){
    topdisplay.rotate180(digit[i], &(digits[i]));
  }

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
  Serial.println("\nendet loop\n");
}
