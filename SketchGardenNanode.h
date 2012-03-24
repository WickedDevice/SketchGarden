/*
 * SketchGardenNanode.h
 *
 *  Created on: March 21, 2012
 *      Author: Victor Aprea
 */

#ifndef SKETCHGARDEN_NANODE_H_
#define SKETCHGARDEN_NANODE_H_

#if ARDUINO >= 100
  #include <Arduino.h> // Arduino 1.0
#else
  #include <WProgram.h> // Arduino 0022
#endif

#include <stdint.h>

class SketchGardenNanode{
public:
    SketchGardenNanode();     
    static uint8_t start();
    static uint8_t poll();    
private:
    static char MY_API_KEY_STATUS_URL[];
    static char website[] PROGMEM;
    static char root[] PROGMEM;
    static unsigned long lWaitMillis; 
    
    static void HandleSketchGardenStatus(byte status, word off, word len);
    static void PutAPIKeyFromEepromIntoStatusUrl();
    static void soft_reset();    
};

#endif