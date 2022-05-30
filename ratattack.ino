#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"
#include "SevSeg.h"
//#include "Guitarhero.h"

#define HT_DATA 2
#define HT_WR 3
#define HT_CS 4

#define X_CIRC 0
#define Y_CIRC 9
#define X_X 0
#define Y_X 4
#define X_TRI 0
#define Y_TRI 15

Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
SevSeg sevseg;

int mode;
//bool easyMode;
byte velocity;

int score;

byte count;

const byte button1 = 6; //green 
const byte button2 = 7; //red 
const byte button3 = 8; //blue
const byte button4 = 9; //yellow 

void setup() {
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.clearScreen();

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

//  byte numDigits = 4;
//  byte digitPins[] = {10, 11, 12, 13};
//  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
//
//  bool resistorsOnSegments = true;
//  bool updateWithDelaysIn = true;
//  byte hardwareConfig = COMMON_CATHODE;
//
//  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
//  sevseg.setBrightness(90);

  randomSeed(analogRead(1));

  Serial.begin(9600);
  Serial.println("Start game");
}

void loop() {
  startGame();

  displayScore(score);
  
  while (count++ < 15) {
    playGame(matrix);
    displayScore(score);
  }

  gameOver();
}

void playGame(Adafruit_HT1632LEDMatrix matrix) {
  if (mode == 1) // easy mode
    velocity = 80;
  else if (mode == 2)         // hard mode
    velocity = 50;
  else if (mode == 3)
    velocity = 25;
  else if (mode == 4)
    velocity = 15;
    
  drawLine(matrix);
  
  int x = random(12);
  
  if (x>=0 && x<=2) {
    bool added = false;
    for (int i=0; i<23; i++) {  // Rectangle
      matrix.drawRect(i, 0, matrix.width()/6, matrix.height()/4, 1);
      matrix.writeScreen();
      delay(velocity);
      if (i >= 19 && i < 23 && digitalRead(button1) == LOW) {
        if (!(added)) {
          score += 200;
          added = true; 
        }
        Serial.println(score);
      }
      matrix.drawRect(i, 0, matrix.width()/6, matrix.height()/4, 0);
    }
  } else if (x>=6 && x<=8){
  bool added = false;
  for (int i=0; i<23; i++) {  // X
    drawX(matrix, i, Y_X).writeScreen();
    delay(velocity);
    if (i >= 19 && i < 23 && digitalRead(button2) == LOW) {
      if (!(added)) {
        score += 200;
        added = true; 
      }
      Serial.println(score);
    }
    deleteX(matrix, i, Y_X).writeScreen();
  }
  } else if (x>=3 && x<=5){
    bool added = false;
    for (int i=0; i<23; i++) {  // Circle
      drawCircle(matrix, i, Y_CIRC).writeScreen();
      delay(velocity);
      if (i >= 19 && i < 23 && digitalRead(button3) == LOW) {
        if (!(added)) {
          score += 200;
          added = true; 
        }
        Serial.println(score);
      }
    deleteCircle(matrix, i, Y_CIRC).writeScreen();
    }  
  } else {
    bool added = false;
    for (int i=0; i<23; i++){  // Triangle
      drawTri(matrix, i, Y_TRI).writeScreen();
      delay(velocity);
      if (i >= 19 && i < 23 && digitalRead(button4) == LOW) {
        if (!(added)) {
          score += 200;
          added = true; 
        }
        Serial.println(score);
      }
      deleteTri(matrix, i, Y_TRI).writeScreen();
    }
  }
}

//void playGame(Adafruit_HT1632LEDMatrix matrix) {
//  if (easyMode) // easy mode
//    velocity = 200;
//  else          // hard mode  
//    velocity = 50;
//    
//  drawLine(matrix);
//  
//  int x = random(12);
//  
//  if (x>=0 && x<=2){
//    for (int i=0; i<23; i++) {  // Rectangle
//      matrix.drawRect(i, 0, matrix.width()/6, matrix.height()/4, 1);
//      matrix.writeScreen();
//      delay(velocity);
//      if (i >= 19 && i < 23) {
//        int num = random(4);
//        if (num == 1)
//          score += 200;  
//          Serial.println(score);
//      }
//      matrix.drawRect(i, 0, matrix.width()/6, matrix.height()/4, 0);
//    }
//  } else if (x>=3 && x<=5){
//    for (int i=0; i<23; i++) {  // Circle
//      drawCircle(matrix, i, Y_CIRC).writeScreen();
//      delay(velocity);
//      if (i >= 19 && i < 23) {
//        int num = random(4);
//        if (num == 1)
//          score += 200; 
//          Serial.println(score);
//    }
//    deleteCircle(matrix, i, Y_CIRC).writeScreen();
//    }  
//  } else if (x>=6 && x<=8){
//    for (int i=0; i<23; i++) {  // X
//      drawX(matrix, i, Y_X).writeScreen();
//      delay(velocity);
//      if (i >= 19 && i < 23) {
//        int num = random(4);
//        if (num == 1)
//          score += 200;   
//          Serial.println(score);
//      }
//      deleteX(matrix, i, Y_X).writeScreen();
//    }
//  } else {
//    for (int i=0; i<23; i++){  // Triangle
//      drawTri(matrix, i, Y_TRI).writeScreen();
//      delay(velocity);
//      if (i >= 19 && i < 23) {
//        int num = random(4);
//        if (num == 1)
//          score += 200;   
//          Serial.println(score);
//      }
//      deleteTri(matrix, i, Y_TRI).writeScreen();
//    }
//  }
//}

void startGame() {
  Serial.println("Reset!");
  
  score = 0;
  count = 0;
  
//  drawbigT(matrix, 1, 14);
//  drawbigA(matrix, 9, 9);
//  drawbigR(matrix, 22, 9);
//  drawK(matrix, 3, 1); 
//  drawC(matrix, 7, 1);
//  drawT(matrix, 17, 1);
//  drawT(matrix, 13, 1);
//  drawA(matrix, 8, 1);
//  drawA(matrix, 20, 1);
//  matrix.writeScreen();
//
  while (digitalRead(button1) && digitalRead(button2) && digitalRead(button3) && digitalRead(button4)) {
    drawbigT(matrix, 1, 14);
    drawbigA(matrix, 9, 9);
    drawbigR(matrix, 22, 9);
    drawK(matrix, 3, 1); 
    drawC(matrix, 7, 1);
    drawT(matrix, 17, 1);
    drawT(matrix, 13, 1);
    drawA(matrix, 8, 1);
    drawA(matrix, 20, 1);
    
    matrix.writeScreen();
  }

  matrix.clearScreen();

//  easyMode = (int)random(1);
  
  if (digitalRead(button1) == LOW) { // Easy mode
    mode = 1;
    matrix.clearScreen();
  }
  else if (digitalRead(button2) == LOW) { // Hard mode
    mode = 2;
    matrix.clearScreen();
  }
  else if (digitalRead(button3) == LOW) { // Hard mode
    mode = 3;
    matrix.clearScreen();
  }
  else if (digitalRead(button4) == LOW) { // Hard mode
    mode = 4;
    matrix.clearScreen();
  }

  matrix.drawRect(0, 0, matrix.width()/6, matrix.height()/4, 1);
  drawX(matrix, 0, Y_X).writeScreen();
  drawCircle(matrix, 0, Y_CIRC).writeScreen();
  drawTri(matrix, 0, Y_TRI).writeScreen();

  delay(1000);

  matrix.clearScreen();
}

void gameOver() {
  matrix.clearScreen();

//  drawG(matrix, 19, 9); 
//  drawbigA(matrix, 12, 9); 
//  drawM(matrix, 6, 9);
//  drawE(matrix, 4, 9);
//  drawO(matrix, 19, 1);
//  drawV(matrix, 14, 1);
//  drawE(matrix, 10, 1);
//  drawbigR(matrix, 5, 1);
//
//  matrix.writeScreen();

  Serial.print("Total score: ");
  Serial.println(score);
  Serial.println();

  resetScore();

  while (digitalRead(button1) && digitalRead(button2) && digitalRead(button3) && digitalRead(button4)) {
    drawG(matrix, 19, 9); 
    drawbigA(matrix, 12, 9); 
    drawM(matrix, 6, 9);
    drawE(matrix, 4, 9);
    drawO(matrix, 19, 1);
    drawV(matrix, 14, 1);
    drawE(matrix, 10, 1);
    drawbigR(matrix, 5, 1);
  
    matrix.writeScreen();
  }

  matrix.clearScreen();
}

void displayScore(int sc) {
  sevseg.setNumber(sc);
  sevseg.refreshDisplay();
}

void resetScore() {
  sevseg.setNumber(0);
  sevseg.refreshDisplay();
}

void drawLine(Adafruit_HT1632LEDMatrix matrix) {
  for (int i = 0; i < 16; i++) {
    matrix.drawPixel(23, i, 1);
    matrix.writeScreen();
  }
}

Adafruit_HT1632LEDMatrix drawCircle(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x + 1, y - 1, 1);
    matrix.drawPixel(x + 2, y - 1, 1);
    matrix.drawPixel(x + 3, y, 1);
    matrix.drawPixel(x + 3, y + 1, 1);
    matrix.drawPixel(x + 2, y + 2, 1);
    matrix.drawPixel(x + 1, y + 2, 1);
    matrix.drawPixel(x, y + 1, 1);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix deleteCircle(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 0);
    matrix.drawPixel(x + 1, y - 1, 0);
    matrix.drawPixel(x + 2, y - 1, 0);
    matrix.drawPixel(x + 3, y, 0);
    matrix.drawPixel(x + 3, y + 1, 0);
    matrix.drawPixel(x + 2, y + 2, 0);
    matrix.drawPixel(x + 1, y + 2, 0);
    matrix.drawPixel(x, y + 1, 0);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix drawX(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x + 3, y, 1);
    matrix.drawPixel(x + 1, y + 1, 1);
    matrix.drawPixel(x + 2, y + 1, 1);
    matrix.drawPixel(x + 2, y + 2, 1);
    matrix.drawPixel(x + 1, y + 2, 1);
    matrix.drawPixel(x + 3, y + 3, 1);
    matrix.drawPixel(x, y + 3, 1);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix deleteX(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 0);
    matrix.drawPixel(x + 3, y, 0);
    matrix.drawPixel(x + 1, y + 1, 0);
    matrix.drawPixel(x + 2, y + 1, 0);
    matrix.drawPixel(x + 2, y + 2, 0);
    matrix.drawPixel(x + 1, y + 2, 0);
    matrix.drawPixel(x + 3, y + 3, 0);
    matrix.drawPixel(x, y + 3, 0);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix drawTri(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x + 1, y - 1, 1);
    matrix.drawPixel(x + 3, y - 1, 1);
    matrix.drawPixel(x + 3, y - 3, 1);
    matrix.drawPixel(x + 1, y, 1);
    matrix.drawPixel(x + 2, y, 1);
    matrix.drawPixel(x + 3, y, 1);
    matrix.drawPixel(x + 3, y - 2, 1);
    matrix.drawPixel(x + 2, y - 2, 1);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix deleteTri(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 0);
    matrix.drawPixel(x + 1, y - 1, 0);
    matrix.drawPixel(x + 3, y - 1, 0);
    matrix.drawPixel(x + 3, y - 3, 0);
    matrix.drawPixel(x + 1, y, 0);
    matrix.drawPixel(x + 2, y, 0);
    matrix.drawPixel(x + 3, y, 0);
    matrix.drawPixel(x + 3, y - 2, 0);
    matrix.drawPixel(x + 2, y - 2, 0);
    
    return matrix;
}

Adafruit_HT1632LEDMatrix drawK(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y;
    
    for (int i= new_y ; i < new_y + 6; i++)
      matrix.drawPixel(x, i, 1); 
    
    matrix.drawPixel(x - 1, y + 2, 1);
    matrix.drawPixel(x - 1, y + 3, 1);
    matrix.drawPixel(x - 2, y + 1, 1);
    matrix.drawPixel(x - 2, y + 4, 1);
    matrix.drawPixel(x - 3, y, 1);
    matrix.drawPixel(x - 3, y + 5, 1);
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawT(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y; 
    
    for (int i = new_y; i < new_y + 6; i++)
      matrix.drawPixel(x, i, 1); 
    
    for (int i = new_y; i < new_y + 6; i++)
      matrix.drawPixel(x + 1, i, 1); 
    
    matrix.drawPixel(x - 1, y + 5, 1);
    matrix.drawPixel(x + 2, y + 5, 1);
    
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawA(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y; 
    for (int i=new_y; i<new_y+4; i++){
      matrix.drawPixel(x, i, 1); 
    }
    for (int i=new_y; i<new_y+4; i++){
      matrix.drawPixel(x+3, i, 1); 
    }
    matrix.drawPixel(x + 1, y + 2, 1);
    matrix.drawPixel(x + 2, y + 2, 1);
    matrix.drawPixel(x + 1, y + 4, 1);
    matrix.drawPixel(x + 2, y + 4, 1);
    matrix.drawPixel(x + 1, y + 5, 1);
    matrix.drawPixel(x + 2, y + 5, 1);
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawC(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y; 
    for (int i=new_y; i<new_y+6; i++){
      matrix.drawPixel(x, i, 1); 
    }
    matrix.drawPixel(x - 1, y, 1);
    matrix.drawPixel(x - 1, y + 5, 1);
    matrix.drawPixel(x - 2, y, 1);
    matrix.drawPixel(x - 2, y + 5, 1);
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawbigT(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x + 1, y, 1);
    int new_y = y;
    for (int i=new_y-5; i<=new_y; i++){
      matrix.drawPixel(x + 2, i, 1); 
    }
    new_y = y;
    for (int i=new_y-5; i<=new_y; i++){
      matrix.drawPixel(x + 3, i, 1); 
    }
    matrix.drawPixel(x + 4, y, 1);
    matrix.drawPixel(x + 5, y, 1);

    return matrix; 
}

Adafruit_HT1632LEDMatrix drawbigA(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x, y + 1, 1);
    matrix.drawPixel(x + 1, y + 2, 1);
    matrix.drawPixel(x + 1, y + 3, 1);
    matrix.drawPixel(x + 2, y + 2, 1);
    matrix.drawPixel(x + 3, y + 2, 1);
    matrix.drawPixel(x + 4, y + 2, 1);
    matrix.drawPixel(x + 4, y + 3, 1);
    matrix.drawPixel(x + 3, y + 5, 1);
    matrix.drawPixel(x + 3, y + 4, 1);
    matrix.drawPixel(x + 2, y + 5, 1);
    matrix.drawPixel(x + 2, y + 4, 1);
    matrix.drawPixel(x + 5, y, 1);
    matrix.drawPixel(x + 5, y + 1, 1);
    
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawbigR(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y;
    for (int i = new_y; i<new_y+6; i++){
      matrix.drawPixel(x, i, 1); 
    }
    matrix.drawPixel(x-1, y+3, 1);
    matrix.drawPixel(x-1, y+5, 1);
    matrix.drawPixel(x-2, y+3, 1);
    matrix.drawPixel(x-2, y+5, 1);
    new_y = y+2;
    for (int i = new_y; i<new_y+4; i++){
      matrix.drawPixel(x-3, i, 1);
    }
    matrix.drawPixel(x - 4, y + 1, 1);
    matrix.drawPixel(x - 5, y, 1); 
    
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawG(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_x = x; 
    for (int i=new_x; i<new_x + 4; i++){
      matrix.drawPixel(i, y, 1); 
      matrix.drawPixel(i, y + 5, 1);
    }
    int new_y = y;
    for (int i=new_y+1; i<new_y+5; i++){
      matrix.drawPixel(x + 3, i, 1); 
    }
    matrix.drawPixel(x, y + 1, 1);
    matrix.drawPixel(x, y + 2, 1); 
    matrix.drawPixel(x + 1, y + 2, 1); 
  
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawM(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y; 
    for (int i=new_y; i<new_y+6; i++){
      matrix.drawPixel(x, i, 1);
      matrix.drawPixel(x+5, i, 1); 
    }
    matrix.drawPixel(x+1, y+4, 1);
    matrix.drawPixel(x+2, y+3, 1);
    matrix.drawPixel(x+3, y+3, 1);
    matrix.drawPixel(x+4, y+4, 1);
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawE(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y;
    for (int i=new_y; i<new_y+6; i++){
      matrix.drawPixel(x, i, 1);
    }
    int new_x = x;
    for (int i=new_x-1; i>new_x-4; i--){
      matrix.drawPixel(i, y, 1);
      matrix.drawPixel(i, y + 3, 1);
      matrix.drawPixel(i, y + 5, 1); 
    }
    
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawO(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    int new_y = y;
    for (int i=new_y; i<new_y+6; i++){
      matrix.drawPixel(x, i, 1);
      matrix.drawPixel(x + 3, i, 1);
    }
    matrix.drawPixel(x + 1, y, 1);
    matrix.drawPixel(x + 2, y, 1);
    matrix.drawPixel(x + 1, y + 5, 1);
    matrix.drawPixel(x + 2, y + 5, 1);
    return matrix; 
}

Adafruit_HT1632LEDMatrix drawV(Adafruit_HT1632LEDMatrix matrix, int x, int y) {
    matrix.drawPixel(x, y, 1);
    matrix.drawPixel(x + 1, y, 1);
    matrix.drawPixel(x, y + 1, 1);
    matrix.drawPixel(x + 1, y + 1, 1);
    matrix.drawPixel(x - 1, y + 2, 1);
    matrix.drawPixel(x - 1, y + 3, 1);
    matrix.drawPixel(x + 2, y + 2, 1);
    matrix.drawPixel(x + 2, y + 3, 1);
    matrix.drawPixel(x + 3, y + 4, 1);
    matrix.drawPixel(x + 3, y + 5, 1);
    matrix.drawPixel(x - 2, y + 4, 1);
    matrix.drawPixel(x - 2, y + 4, 1);
    
    return matrix; 
}
