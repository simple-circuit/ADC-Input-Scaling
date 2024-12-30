// Measure ADC input voltage at 1ms or 8ms loop times
// and print the value
// Average ADC readings 8X or 64X
// 12-29-2024 Simple-Circuit

#include "pico/stdlib.h"              
volatile uint64_t time_us;  
volatile unsigned long dt_us; 

void setup() {
   Serial.begin(115200);  //Readings sent out of USB serial port
   analogReadResolution(10);
   pinMode(0, INPUT_PULLUP);  //select 8X or 64X samples
   pinMode(1, INPUT_PULLUP);  //select 1ms or 8ms print loop 
   pinMode(18, OUTPUT);
   pinMode(23, OUTPUT);      //set GPIO 23 high for PWM regulation of 3.3V regulator
   digitalWrite(23,1);       //this improves 3.3V ripple
}

void loop(){
 volatile long i,m,n;
 float v2;
  
 time_us = time_us+1000;  
  
 while(true){
  v2 = 0.0;
  if (digitalRead(0)==1) m=8; else m=64;
  if (digitalRead(1)==1) dt_us=1000; else dt_us=8000;

  for (i=0;i<m;i++)  {
    gpio_put(18,1);
    v2=v2+analogRead(A0);
    gpio_put(18,0);
  }
  v2=v2/m;
  Serial.println(v2*3.3/1024.0,3);
  while (time_us_64() < time_us); 
  time_us = time_us+dt_us;
 }
}
