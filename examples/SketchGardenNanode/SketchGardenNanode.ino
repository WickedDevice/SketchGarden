/* Sketch Garden Nanode.ino
 For use with http://sketchgarden.co
 This sketch is released into the public domain by 
 Wicked Device LLC
 Author: Vic Aprea, March 2012
 Change Log:
    - March 21, 2012, Initial Sketch, Vic Aprea
	- March 22, 2012, Minor changes, Dirk Swart
*/

// YOU MUST INCLUDE THESE HEADERS (you probably were going to anyway :)
#include <EtherCard.h>
#include <SketchGardenNanode.h> 
#include <NanodeMAC.h> // Automatically reads your Nanode's MAC chip for you

// THE USUAL HELPERS FOR THE EtherCard LIBRARY
static uint8_t mymac[6] = { 0,0,0,0,0,0 };
NanodeMAC mac( mymac );
byte Ethernet::buffer[700];

// CREATE YOUR SketchGardenNanode OBJECT
SketchGardenNanode sgn;

void setup () {
  // SERIAL PRINTS CAN BE REMOVED IF YOU DON'T WANT THEM 
  Serial.begin(57600);
  Serial.println("\n[webClient]");
  
  // INITIALIZE THE ETHERCARD LIBRARY AS YOU NORMALLY WOULD
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  
  // GET AN IP ADDRESS WITH DHCP (YOU CAN CHANGE THIS IF YOU WANT TO USE STATIC IP)
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");          
  
  // INITIALIZE THE SketchGardenNanode OBJECT
  if(!sgn.start()){
    Serial.println("Sketch Garden Initialization Failed");
  } 
  
  ether.printIp("SketchGarden IP: ", ether.hisip);  
    
  // INSERT YOUR CUSTOM CODE HERE
    
}

void loop(){
  // SERVICE THE ETHERNET CONTROLLER PACKET LOOP
  ether.packetLoop(ether.packetReceive());
  
  // CALL THE SketchGardenNanode POLL METHOD
  // THIS INTERNALLY KEEPS TRACK OF TIME AND POLLS SKETCHGARDEN FOR STATUS EVERY 30 SECONDS
  // IF YOU ACTIVATE A SKETCH ON THE SKETCHGARDEN WEBSITE THE POLL METHOD WILL CAUSE A RESET!
  if(sgn.poll()){
    Serial.print(millis(), DEC);
    Serial.print(" ");
    Serial.println("Polled Sketch Garden"); 
  }
  
  // INSERT YOUR CUSTOM CODE HERE... YOU SHOULD TRY NOT TO BLOCK AS MUCH AS POSSIBLE
  // OR YOUR PACKET LOOP MIGHT FALL BEHIND AND YOU MIGHT START DROPPING PACKETS (NOTHING NEW HERE)  
  
}
