#define ROW1 13
#define ROW2 12
#define ROW3 11
#define ROW4 10
#define ROW5 9
#define ROW6 8
#define ROW7 7
#define ROW8 6

#define COL1 5 
#define COL2 4
#define COL3 3
#define COL4 A4
#define COL5 A3
#define COL6 A2
#define COL7 A1
#define COL8 A0

#define BUTTON 2


//music in
#include "pitch.h"
const int  BUTTON_PIN = A5;
int buttonState = 0;

// notes in the melody:
int melody[] = {
  NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0, NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
  16, 16, 8, 8, 16, 16, 16, 16, 16, 8, 16, 16, 16,
};


int thisNote = -1, noteDuration = 0;
unsigned long previousTime = 0,
              presentTime = 0,
              pauseBetweenNotes = 1;

const int row[] = {
  ROW1,ROW2,ROW3,ROW4,ROW5,ROW6,ROW7,ROW8
};

const int col[] = {
  COL1,COL2,COL3,COL4,COL5,COL6,COL7,COL8
};

byte scan[8][8] = {
  {1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,1}
};

byte heart[8][8] = {
  {1,1,1,1,1,1,1,1},
  {1,0,0,1,1,0,0,1},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,0,0,1,1,1}
};

  byte A[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,1,0,0,1,0,0},
  {0,1,0,0,0,0,1,0},
  {0,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,1,0},
  {0,1,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0}
  };

byte circle[8][8] = {
  {0,0,1,1,1,1,0,0},
  {0,1,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {0,1,0,0,0,0,1,0},
  {0,0,1,1,1,1,0,0}
};

byte previousState = 1, presentState = 1, patternNumber = 0;




void setup() 
{
  // put your setup code here, to run once: 
  Serial.begin(9600);
  for (byte i = 0; i <= sizeof(row); i++) {
    pinMode(row[i], OUTPUT);
  }

  for (byte i = 0; i <= sizeof(col); i++) {
    pinMode(col[i], OUTPUT);
  }

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(A5, OUTPUT);

}

/*void loop(){
  showPattern (circle);
}*/

void checkToPlay() {
  presentTime = millis();
  if(presentTime - previousTime >= pauseBetweenNotes) {
    thisNote += 1;
    if (thisNote >= 105) {
      thisNote = -1;
      pauseBetweenNotes = 100;
      previousTime = millis();
      } else {
      noteDuration = 1500 / noteDurations[thisNote];
      tone (A5, melody[thisNote], noteDuration);
      pauseBetweenNotes = noteDuration * 1.2;
      previousTime = millis();
      }
    }
  }
 void loop(){

  checkToPlay();
   presentState = digitalRead(BUTTON);
   if(presentState == 0 && previousState == 1)
   {
    
     patternNumber ++;
     if(patternNumber > 2) {
       patternNumber = 0;
     }
   }

   Serial.println(presentState);

   if(patternNumber == 0){
     showPattern(heart);
   }
   else if(patternNumber == 1) {
     showPattern(A);
     }

   else  
   {
     showPattern(circle);
   }
   
   delay(1);
   previousState = presentState;

 }

void showPattern(byte matrix[8][8]){
  for (byte  i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      digitalWrite(row[j], 1-scan[i][j]);
      digitalWrite(col[j], 1-matrix[i][j]);
    }
    for (byte j = 0; j < 8; j++) {
      digitalWrite(row[j], HIGH);
      digitalWrite(col[j], LOW);
    }
  }
}
