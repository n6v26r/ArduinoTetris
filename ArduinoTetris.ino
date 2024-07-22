#include "display.h"

#define INPUT_DELAY 0.07f

double lastInputRead = 0.0f;

gameManager game;

bool parseInput(gameManager *game){
  bool updatedPiece = false;
  int input = Serial.read();
  if(!(isalpha(input) || isspace(input))) return false;

  if(getTime()-lastInputRead<=INPUT_DELAY)
    return false;

  lastInputRead = getTime();
  switch(input){
    case 'w':
      rotatePiece(game);
      updatedPiece = true;
      break;
    case 'a':
      moveL(game);
      updatedPiece = true;
      break;
    case 'd':
      moveR(game);
      updatedPiece = true;
      break;
    case 's':
      moveDown(game);
      updatedPiece = true;
      break;
    case ' ':
      drop(game);
      updatedPiece = true;
      break;
    case 'c':
      hold(game);
      updatedPiece = true;
      break;
    case 'e':
      _exit(0);
      break;
    default: 
      break;
  }
  return updatedPiece;
}

void gameLoop(){
  srand(analogRead(8)); // set rand seed to unix time
  debug("Starting game...");
  _init();
  initGame(&game);
  LcdInit(&game);
  getNextPiece(&game);
  spawnPiece(&game);
  // will implement levels soon
  while(1){
    bool redraw = false;
    // Get input
    if(parseInput(&game)) redraw = true;

    if(update(&game)) redraw = true;
    
    if(redraw) drawBoard(&game);
  }
}

void setup(){
}

void loop(){
  gameLoop();
  delay(1000);
}