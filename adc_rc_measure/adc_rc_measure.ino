// Sample the ADC continuously at 50KHz, 20KHz, 10KHz or 5KHz
// Send the ADC value to PWM GPIO-14 which is filtered to make a DAC
// Every 15,000 cycles, GPIO-21 is switched to a logic 1
// then after another 15,000 cycles by to a high impedance input state 
// 12-29-2024 Simple-Circuit

#include "pico/stdlib.h"               

volatile uint64_t time_us;  //changed to 64-bits to preven lock up at 1hr11min
volatile unsigned long dt_us = 100; 

void setup() {

  analogWriteRange(1024);
  analogWriteFreq(100000);
  analogReadResolution(10);
  pinMode(0, INPUT_PULLUP);    //inputs select sampling freq
  pinMode(1, INPUT_PULLUP);    
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP); 
  pinMode(14, OUTPUT);        //PWM outputfiltered is the RC time contant voltage
  pinMode(18, OUTPUT);        //verify sample freq on this pin 
  pinMode(21, INPUT);         //generates step function for time constant test
  pinMode(23, OUTPUT); //set GPIO 23 high for PWM regulation of 3.3V regulator
  digitalWrite(23,1);  //this improves 3.3V ripple
       
}

void loop(){
  volatile long n;
  int v2;
  n=0;
  time_us = time_us+dt_us;  
  
 while(true){

   if (((++n/15000)&1)==0)  pinMode(21, INPUT); else{
    pinMode(21, OUTPUT);
    digitalWrite(21, 1);
   }

   if (digitalRead(0)==0)dt_us=20; 
   if (digitalRead(1)==0)dt_us=50; 
   if (digitalRead(2)==0)dt_us=100; 
   if (digitalRead(3)==0)dt_us=200; 
 
   gpio_put(18,1);
   v2=analogRead(A0);
   gpio_put(18,0);
   analogWrite(14,v2);
  while (time_us_64() < time_us); 
  time_us = time_us+dt_us;
 }
}
