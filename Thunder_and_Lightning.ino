#include <Adafruit_NeoPixel.h>

 
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"



int rxPin = 10;    // DFplayer RX to Arduino pin 10
int txPin = 11;    // DFplayer TX toArduinopin 11
int busyPin = 12;  // DFplayer BUSY connected to pin 12
int NUM_LEDS = 6;
#define LED_PIN  5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 5, NEO_GRBW + NEO_KHZ800);
SoftwareSerial mySoftwareSerial(rxPin, txPin);
DFRobotDFPlayerMini myDFPlayer;



void setup()
{
  
strip.begin();
strip.show();
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(busyPin, INPUT);
  
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println(F("Initializing DFPlayer..."));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin. Check connection and SD card, or reset the Arduino."));
    while (true);
  }

  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);                   // Set serial communictaion time out 500ms
  myDFPlayer.volume(20);                        // Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);              // Set EQ to BASS (normal/pop/rock/jazz/classic/bass)
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Set device we use SD as default
  myDFPlayer.enableDAC();                       // Enable On-chip DAC
}

void loop() {
  int flashCount = random (3, 35);        // Min. and max. number of flashes each loop
  int flashBrightnessMin =  10;           // LED flash min. brightness (0-255)
  int flashBrightnessMax =  255;          // LED flash max. brightness (0-255)

  int flashDurationMin = 1;               // Min. duration of each seperate flash
  int flashDurationMax = 150;              // Max. duration of each seperate flash

  int nextFlashDelayMin = 1;              // Min, delay between each flash and the next
  int nextFlashDelayMax = 150;            // Max, delay between each flash and the next

  int thunderDelay = random (500, 3000);  // Min. and max. delay between flashing and playing sound
  int thunderFile = random (1, 17);       // There are 17 soundfiles: 0001.mp3 ... 0017.mp3
  int loopDelay = random (500, 3000);   // Min. and max. delay between each loop

  Serial.println();
  Serial.print(F("Flashing, count: "));
  Serial.println( flashCount );
  
for (int flash = 0 ; flash <= flashCount; flash += 1) {
  analogWrite(LED_PIN, random (flashBrightnessMin, flashBrightnessMax));
    delay(random(flashDurationMin, flashDurationMax)); // Keep it tured on, random duration
 
    analogWrite(LED_PIN, 0); // Turn the LED strip off
    delay(random(nextFlashDelayMin, nextFlashDelayMax));

  for(uint16_t i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i, strip.Color(0,0,176,255));
    strip.show();
   for(uint16_t i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i, strip.Color(0,0,0,0));
    strip.show();
  }
}
}
  Serial.print(F("Pausing before playing thunder sound, milliseconds: "));
  Serial.println(thunderDelay);
  delay(thunderDelay);

  Serial.print(F("Playing thunder sound, file number: "));
  Serial.println(thunderFile);
  myDFPlayer.playMp3Folder(thunderFile);
  delay(1000); // Give the DFPlayer some time

  while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
  }

  Serial.print(F("Pausing before next loop, milliseconds: "));
  Serial.println(loopDelay);
  delay(loopDelay);


  

}
