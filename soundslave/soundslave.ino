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

void setup() {
  
  Serial.begin(9600);
  
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
}

void loop () {
  
  int touch7 = 0;
  int touch8 = 0;
  touch7 = analogRead(A0);
  touch8 = analogRead(A1);
  if  (touch7 < VALUE_TOUCH) {
      sendNote(7);
  }  else if  (touch8 < VALUE_TOUCH) {
      sendNote(8);
  } 
  
  readSerial();
  
  
  if (!MP3player.isPlaying() && indexLoopSound != -1) { //IF the  music is finished , restart it
       MP3player.stopTrack();
       playMusic();
  } else {
       if (indexLoopSound != -1)  //Start the loop if music is not idle
           playMusic();
  }
  
}

void playMusic() {
    MP3player.playMP3(getFilename(indexLoopSound));
}


//Return the right file audio to play based on the index
char* getFilename (int index) {
   if (index == 0) return "guitar_note1.mp3";
   else if (index == 1) return "guitar_note2.mp3";
   else if (index == 2) return "guitar_note3.mp3";
   else if (index == 3) return "guitar_note4.mp3";
   else if (index == 4) return "drum_note1.mp3";
   else if (index == 5) return "drum_note2.mp3";
   else if (index == 6) return "drum_note3.mp3";
   else if (index == 7) return "drum_note4.mp3";
   else if (index == 8) return "keyboard_note1.mp3";
   else if (index == 9) return "keyboard_note2.mp3";
   else if (index == 10) return "keyboard_note3.mp3";
   else if (index == 11) return "keyboard_note4.mp3";
   else return "guitar_note1.mp3";
}

void sendNote(int number) {
    Serial.print("S");
    Serial.println(number);
}

void readSerial() {
    if (Serial.available() > 0) {
          int charIn = 0;
          boolean isLoop = false;
          while(charIn = Serial.read()) {
             if (charIn == 13 || charIn == -1) break; 
             
             if (charIn == 'L') isLoop = true; //Check if the incoming is about loop
             else {
                if (isLoop) {
                      int number = charIn -'0'; //double check that the number is in range
                      if (number >= 0 && number <= 7) indexLoopSound = number;
                      break;
                }
             }
            
             delay(10); 
          }
    }

}



// initSD() initializes the SD card and checks for an error.
void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
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
