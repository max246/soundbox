#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library



#define MODE_TOP 0
#define MODE_BOTTOM 1

#define VALUE_TOUCH 300

 
int sequence[8] =  {-1,-1,-1,-1,-1,-1,-1,-1};
int indexSeq = 0;
int MODE = MODE_TOP;

SdFat sd; // Create object to handle SD functions
SFEMP3Shield MP3player; // Create Mp3 library object

const uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max

  int xRate, yRate, zRate;


unsigned long lastPress = 0;
void setup() {

  Serial.begin(9600);
   
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  
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
  
//  Serial.println(touch4);
  
//  readSerial();
  
  
  
  if  (touch1 < VALUE_TOUCH) {
      if (checkLast()) playNote(1);
  } 
  if  (touch2 < VALUE_TOUCH) {
      if (checkLast()) playNote(2);
  }  
  if  (touch3 < VALUE_TOUCH) {
      if (checkLast()) playNote(3);
  } 
  if  (touch4 < VALUE_TOUCH) {
      if (checkLast()) sendLoop();
  }
  
  
  delay(100);
}


boolean checkLast() {
    unsigned long result = (millis() - lastPress);

    if (result > 700)  {
      lastPress = millis();
      return true;
    } 
    else false;
  
}

//Read the serial port for any incoming messages
void readSerial() {
    if (Serial.available() > 0) {
          int charIn = 0;
          boolean isTouched = false;
          while(charIn = Serial.read()) {
             if (charIn == 13 || charIn == -1) break; 
             
             if (charIn == 'S') isTouched = true; //Check if the incoming is about loop
             else {
                if (isTouched) {
                      int number = charIn -'0'; 
                      if (number == 7) playNote(3);
                      else if (number == 8) sendLoop();
                      break;
                }
             } 
             delay(10);
          }
    }

}


//Send the index to the next Arduino, so it will loop the right sound track
void sendLoop() {
    if (MODE == MODE_TOP)
      Serial.print("T");
    else
      Serial.print("B");
      
    for (int i = 0;i < 8;i++) {
        if (sequence[i] == -1) Serial.print(99);
        else  Serial.print(sequence[i]);
    }
    Serial.println("");
    reset();
}


//Play the right sound track after the user pressed the pad
void playNote(int number) {
    if (MODE == MODE_TOP) {
        if (number == 1)  MP3player.playMP3("TC1.MP3");
        else if (number == 2)  MP3player.playMP3("TC2.MP3");
        else if (number == 3)  MP3player.playMP3("TC3.MP3");
    } else if (MODE == MODE_BOTTOM) {
        if (number == 1)  MP3player.playMP3("BC1.MP3");
        else if (number == 2)  MP3player.playMP3("BC2.MP3");
        else if (number == 3)  MP3player.playMP3("BC3.MP3");
    } 
    
  //  Serial.println(number);
    
    if (indexSeq <= 7) {
        sequence[indexSeq] = number;
        indexSeq++;
    }
    
     
}



unsigned timeS = 0;

//Check the orientation of the box
void checkOrientation() {
    xRate = readX();
    yRate = readY();
   // zRate = readZ();

  if ((yRate > 1000 || xRate > 1000 ) && timeS == 0) {
        timeS = millis();
  } else {
        if (yRate < 100 && xRate < 100 && timeS != 0) {
             //Serial.print("time rotate");
             //Serial.println((millis()-timeS));
             
             if ((millis() - timeS) >= 200) {
                 if (MODE == MODE_TOP) MODE= MODE_BOTTOM;
                 else MODE  = MODE_TOP;
             }
             timeS  =0;
        }
  }
    
    
    /*
    
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
    }*/
    
   /* 
    if (x == 0) { 
        MODE = MODE_TOP; 
        reset();
    }
    else  { 
      reset();
      MODE= MODE_BOTTOM;
    }*/
  }



void reset() {
  indexSeq = 0;
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
