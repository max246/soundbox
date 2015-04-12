#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library



#define MODE_GUITAR 0
#define MODE_DRUM 1
#define MODE_KEYBOARD 2

 

int MODE = MODE_GUITAR;

int ORIENTATION_GUITAR[] = {0,0,0,1,1,1}; 
int ORIENTATION_DRUM[] = {0,0,0,1,1,1}; 
int ORIENTATION_KEYBOARD[] = {0,0,0,1,1,1}; 

SdFat sd; // Create object to handle SD functions
SFEMP3Shield MP3player; // Create Mp3 library object

const uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max

  int xRate, yRate, zRate;


void setup() {

  Serial.begin(9600);
  //Setup mp3 shield 
  //Setup gyro
  //Serial connection
  
  initGyro();
  
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
  
  
}

void loop() {
  checkOrientation();
  
  int touch1 = 0;
  int touch2 = 0;
  int touch3 = 0;
  int touch4 = 0;
  
  touch1 = analogRead(A0);
  touch2 = analogRead(A1);
  touch3 = analogRead(A2);
  touch4 = analogRead(A3);
  
  
  if  (touch1 < 300) {
      playNote(1);
  }  else if  (touch2 < 300) {
      playNote(2);
  }  else if  (touch3 < 300) {
      playNote(3);
  } else if  (touch4 < 300) {
      playNote(4);
  }  
   
  
  
  delay(500);
}


void playNote(int number) {
    if (MODE == MODE_GUITAR) {
        if (number == 1)  MP3player.playMP3("guitar_note1.mp3");
        else if (number == 2)  MP3player.playMP3("guitar_note2.mp3");
        else if (number == 3)  MP3player.playMP3("guitar_note3.mp3");
        else if (number == 4)  MP3player.playMP3("guitar_note4.mp3");
    } else if (MODE == MODE_DRUM) {
        if (number == 1)  MP3player.playMP3("drum_note1.mp3");
        else if (number == 2)  MP3player.playMP3("drum_note2.mp3");
        else if (number == 3)  MP3player.playMP3("drum_note3.mp3");
        else if (number == 4)  MP3player.playMP3("drum_note4.mp3");
    } else if (MODE == MODE_KEYBOARD) {
        if (number == 1)  MP3player.playMP3("keyboard_note1.mp3");
        else if (number == 2)  MP3player.playMP3("keyboard_note2.mp3");
        else if (number == 3)  MP3player.playMP3("keyboard_note3.mp3");
        else if (number == 4)  MP3player.playMP3("keyboard_note4.mp3");
    }
     
}

void checkOrientation() {
    xRate = readX();
    yRate = readY();
    zRate = readZ();

  
    int x = 0;
    int y = 0;
    int z = 0;
    
    
    if (x <= ORIENTATION_GUITAR[0] && ORIENTATION_GUITAR[1] >= x) {
        if (y <= ORIENTATION_GUITAR[2] && ORIENTATION_GUITAR[3] >= y) {
          if (z <= ORIENTATION_GUITAR[4] && ORIENTATION_GUITAR[5] >= z) {
              MODE = MODE_GUITAR;
          }
        }
    }
    
     
    if (x <= ORIENTATION_DRUM[0] && ORIENTATION_DRUM[1] >= x) {
        if (y <= ORIENTATION_DRUM[2] && ORIENTATION_DRUM[3] >= y) {
          if (z <= ORIENTATION_DRUM[4] && ORIENTATION_DRUM[5] >= z) {
              MODE = MODE_DRUM;
          }
        }
    }
    
     
    if (x <= ORIENTATION_KEYBOARD[0] && ORIENTATION_KEYBOARD[1] >= x) {
        if (y <= ORIENTATION_KEYBOARD[2] && ORIENTATION_KEYBOARD[3] >= y) {
          if (z <= ORIENTATION_KEYBOARD[4] && ORIENTATION_KEYBOARD[5] >= z) {
              MODE = MODE_KEYBOARD;
          }
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
