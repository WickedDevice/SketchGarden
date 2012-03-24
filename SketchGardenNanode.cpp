#include "SketchGardenNanode.h"
#include "../EtherCard/EtherCard.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#define SKETCH_GARDEN_POLLING_INTERVAL 30000

extern EtherCard ether;

char SketchGardenNanode::MY_API_KEY_STATUS_URL[] = "devices/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/status.csv";
char SketchGardenNanode::website[] PROGMEM = "sketchgarden.co";
char SketchGardenNanode::root[] PROGMEM = "/";
unsigned long SketchGardenNanode::lWaitMillis = 0;

SketchGardenNanode::SketchGardenNanode(){

}

uint8_t SketchGardenNanode::start(){
  SketchGardenNanode::lWaitMillis = millis() + SKETCH_GARDEN_POLLING_INTERVAL;
  SketchGardenNanode::PutAPIKeyFromEepromIntoStatusUrl();
  return ether.dnsLookup(SketchGardenNanode::website);
}


void SketchGardenNanode::HandleSketchGardenStatus(uint8_t status, uint16_t off, uint16_t len) {
  char * ptr =(char*) (Ethernet::buffer + off);
  char * last = (char*) (ptr + len);
  char status_code[] = "xxx";  
  
  // if we scan the response for the first line that starts with '3' (i.e. '\r', '\n', '3', '\r', '\n')
  // the next line will contain the 3-digit status code
  while(ptr != last){
    if( strncmp_P( ptr, PSTR("\r\n3\r\n"), 5 ) == 0 ){
      // we know the next line contains the status code
      strncpy(status_code, ptr + 5, 3);

      if(strncmp_P(status_code, PSTR("300"), 3) == 0){
        soft_reset();
      }
      
      break;
    }
    ptr++; 
  }  
}

void SketchGardenNanode::PutAPIKeyFromEepromIntoStatusUrl(){
  int address = 0x2C1;   // beginning of API key in EEPROM
  byte value = 0;
  for(byte ii = 0; ii < 36; ii++){ // there are 36 bytes in the API key    
    value = eeprom_read_byte((const uint8_t *) address++);
    MY_API_KEY_STATUS_URL[8 + ii] = value; // the API key starts at offset 8 in the URL variable string
  }
}

void SketchGardenNanode::soft_reset(){
  do                          
  {                          
      wdt_enable(WDTO_15MS); 
      for(;;)                
      {                      
      }                      
  } while(0);
}

uint8_t SketchGardenNanode::poll(){
  unsigned long currentMillis = millis();

  if( (long)( currentMillis - SketchGardenNanode::lWaitMillis ) >= 0)
  {   
    SketchGardenNanode::lWaitMillis += SKETCH_GARDEN_POLLING_INTERVAL;  
    ether.browseUrl((prog_char *) SketchGardenNanode::root, (const char *) MY_API_KEY_STATUS_URL, (prog_char *) website, SketchGardenNanode::HandleSketchGardenStatus);      
    
    return 1;
  }  
  
  return 0;
}