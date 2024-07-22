#include "utils.h"
#include "tetris.h"
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

//if the IC model is not known and the modules is readable,you can use this constructed function
LCDWIKI_KBV lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

#define BLOCKSIZE 19

//define some colour values
enum Color{
  BLACK=0x0000,
  BLUE=0x001F,
  RED=0xF800,
  GREEN=0x07E0,
  CYAN=0x07FF,
  MAGENTA=0xF81F,
  YELLOW=0xFFE0,
  WHITE=0xFFFF
};

const Color TetriColor[TETRIMINOS+1] = {BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE};

//clear screen
void clearScreen(){
  lcd.Fill_Screen(BLACK);
}

void DrawBlock(int x, int y, Color clr=0){
  lcd.Set_Draw_color(clr);
  lcd.Fill_Rectangle(x, y, x+BLOCKSIZE-1, y+BLOCKSIZE-1);
}

void DrawEmptyBlock(int x, int y, Color clr=0){
  lcd.Set_Draw_color(clr);
  lcd.Draw_Rectangle(x, y, x+BLOCKSIZE-1, y+BLOCKSIZE-1);
}

void DrawTetrimino(int x, int y, Tetriminos tetri){
  lcd.Set_Draw_color(TetriColor[tetri]);
  for(int i=0; i<size(tetri); i++)
    for(int j=0; j<size(tetri); j++){
      if(piece[tetri][i][j])
        lcd.Fill_Rectangle(x+j*8, y+i*8, x+(j+1)*8-1, y+(i+1)*8-1);
    }
}

void LcdInit(gameManager* game) 
{
  Serial.begin(9600);
  lcd.Init_LCD();
  Serial.println(lcd.Read_ID(), HEX);
  lcd.Fill_Screen(BLACK);
  lcd.Set_Draw_color(BLACK);
  for(int i=game->sizeY-1; i>=0; i--){
    for(int j=0; j<game->sizeX; j++){
      if(game->board[i][j]!=0)DrawBlock(j*BLOCKSIZE, (game->sizeY-i-1)*BLOCKSIZE, WHITE);
    }
  }
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Set_Text_colour(WHITE);
  lcd.Set_Text_Size(2);
  lcd.Print_String("Next:", 215, 0);
  lcd.Print_String("Hold:", 215, 40);
  lcd.Set_Text_colour(CYAN);
  lcd.Print_String("Lines:", 215, 80);
  lcd.Print_String("Level:", 215, 140);
  lcd.Set_Text_colour(GREEN);
  lcd.Print_String("Score:", 215, 180);
}

void drawBoard(gameManager* game){
  for(int i=game->sizeY-1; i>=0; i--){
    for(int j=0; j<game->sizeX; j++){
      // in projection box bounds
      if(game->projectionPos.y-i<size(game->curr) && game->projectionPos.y-i>=0 && j-game->projectionPos.x<size(game->curr) && j-game->projectionPos.x>=0 
        && game->currPieceState[game->projectionPos.y-i][j-game->projectionPos.x] && !game->board[i][j]){
        DrawBlock(j*BLOCKSIZE, (game->sizeY-i-1)*BLOCKSIZE, BLACK);
        DrawEmptyBlock(j*BLOCKSIZE, (game->sizeY-i-1)*BLOCKSIZE, TetriColor[game->curr]);
        game->board[i][j]=-100;
        }
      else{
        if(game->board[i][j]>0) DrawBlock(j*BLOCKSIZE, (game->sizeY-i-1)*BLOCKSIZE, TetriColor[game->board[i][j]]);
      }

      if(game->board[i][j]==0 && game->lastFrame[i][j]!=game->board[i][j])
        DrawBlock(j*BLOCKSIZE, (game->sizeY-i-1)*BLOCKSIZE, BLACK);
      game->lastFrame[i][j] = game->board[i][j];
      if(game->board[i][j]==-100) game->board[i][j] = 0;
    }
  }
  lcd.Set_Draw_color(BLACK);
  lcd.Fill_Rectangle(275, 0, 320, 80);
  DrawTetrimino(275, 0, *(game->next));
  DrawTetrimino(275, 40, game->hold);

  char LinesStr[3];
  itoa(game->linesCleared, LinesStr, 10);
  lcd.Set_Text_colour(CYAN);
  lcd.Print_String(LinesStr, 215, 100);

  char LvlStr[3];
  itoa(game->level, LvlStr, 10);
  lcd.Set_Text_colour(CYAN);
  lcd.Print_String(LvlStr, 290, 140);

  char ScoreStr[9];
  itoa(game->score, ScoreStr, 10);
  lcd.Set_Text_colour(GREEN);
  lcd.Print_String(ScoreStr, 215, 200);
}
