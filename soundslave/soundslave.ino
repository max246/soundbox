#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

#define VALUE_TOUCH 300


SdFat sd; // Create object to handle SD functions
SFEMP3Shield MP3player; // Create Mp3 library object

const uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max

int indexLoopSound = -1;
int sequence[8] =  {-1,-1,-1,-1,-1,-1,-1,-1};

unsigned long lastSerial = 0;
void setup() {
  
  Serial.begin(9600);
  
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
}

void loop () {
  

  
  readSerial();
  
  
  if (!MP3player.isPlaying()) { //IF the  music is finished , restart it
       MP3player.stopTrack();
       playMusic();
  }
  
  
  delay(200);
  
 /* for (int i = 0;i < 8;i++) {
      Serial.print(sequence[i]);
  }
  Serial.println();*/
}

void playMusic() {
   
    if (sequence[0] != -1) {
      
      if (sequence[indexLoopSound] != -1)  {
        MP3player.playMP3(getFilename(sequence[indexLoopSound]));
        indexLoopSound++;
        if (indexLoopSound >7) indexLoopSound = 0;
      } else {
          indexLoopSound = 0;
      }
    }
   // Serial.println(indexLoopSound);
}


//Return the right file audio to play based on the index
char* getFilename (int index) {
   if (index == 1) return "TC1.MP3";
   else if (index == 2) return "TC2.MP3";
   else if (index == 3) return "TC3.MP3";
   else if (index == 4) return "BC1.MP3";
   else if (index == 5) return "BC2.MP3";
   else if (index == 6) return "BC3.MP3";
   else return "TC1.MP3";
}



void readSerial() {
  
    
    
    if (Serial.available() > 0) {
          unsigned long result = millis()-lastSerial;
          
          Serial.println(result);
          if (result < 900) {
            while(Serial.available() > 0) {
                Serial.read();
            }
            Serial.flush();
            return;
          }
          lastSerial = millis();
          
          
          int charIn = 0;
          boolean isLoopTop = false;
          boolean isLoopBottom = false;
          int index = 0;
          while(charIn = Serial.read()) {
             if (charIn == 13 || charIn == -1) break; 
             
             Serial.println((char)charIn);
             
             if (charIn == 'T') { isLoopTop = true;  reset();continue; } //Check if the incoming is about loop
             if (charIn == 'B') { isLoopBottom = true; reset();  continue;} //Check if the incoming is about loop
             
             if (isLoopTop) {
                 int number = charIn -'0'; //double check that the number is in range
                 if (number > 3 || number <= 0) continue;
                 sequence[index] = number;
                 index++;
                
             }
             
             if (isLoopBottom) {
                 int number = charIn -'0'; //double check that the number is in range
                 if (number > 3 || number <= 0) continue;
                 if (number == 1) number = 3;
                 else if (number == 2) number = 4;
                 else if (number == 3) number = 5;
                 
                 sequence[index] = number;
                 index++;
             }
            
             
             delay(10); 
          }
          Serial.flush();
    }

}


void reset() {
  indexLoopSound = 0;
  for (int i = 0;i < 8;i++) {
    sequence[i] = -1;
  }
  
}

// initSD() initializes the SD card and checks for an error.
void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(9, SPI_HALF_SPEED)) 
    sd.initErrorHalt();
  if(!sd.chdir("/")) 
    sd.errorHalt("sd.chdir");
}

// initMP3Player() sets up all of the initialization for the
// MP3 Player Shield. It runs the begin() function, checks
// for errors, applies a patch if found, and sets the volume/
// stero mode.
void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  if(result != 0) // check result, see readme for error codes.
  {
    // Error checking can go here!
  }
  MP3player.setVolume(volume, volume);
  MP3player.setMonoMode(monoMode);
}
