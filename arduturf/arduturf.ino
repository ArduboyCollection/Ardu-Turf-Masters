/*

    ARDU TURF MASTERS
    Fernando Jerez 2017

    Entry for the Arduboy Jam #2 (December 2017)

    INSTRUCTIONS:------------------------------------
    
    Fill your card with the less score possible.
    You can replay any hole to improve your score.

    In TITLE Screen you can select hole using LEFT-RIGHT
    After every hole, you advance to the next (push A) or back to the title (push B)

    TIPS---------------------------------------------

    Strategy: 
    
    Plan yor hit: Check the terrain using LEFT-RIGHT.
    Select Club (UP-DOWN keys): The number at right is the distance of first rebund at 100% power
    Click A when the POWER BAR is where you nedd it.
    Avoid WATER!! +1 hit when ball falls into water.
     
    
    OTHER------------------------------------------- 
    Card Data saved in EEPROM (300-317)

    Good luck and enjoy!

*/
#include <EEPROM.h>
#include <Arduboy2.h>
#include "globals.h"
#include "clubs.h"
#include "tiles.h"


Arduboy2 arduboy;

byte M[MAPSIZE]; // Map (floor)
int offset = 0; // offset when drawing (for moving map without moving ball)

// GREEN/HOLE vars (init in fillMap());
int holePos; // Position of hole
uint8_t greenSize; // Length of the GREEN zone
uint8_t greenIni; // Beginning of GREEN
uint8_t greenEnd;  //End of GREEN

// Game vars
uint8_t seed = 1; // seed / hole number
uint8_t hits = 1;
uint8_t par = 0; // estimated par

uint8_t card[18];

int alcance; // for temporary calculations

// title/menu vars
uint8_t menu = 0;

// EEPROM vars
int eepromAddress = 200;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();

  resetCard();
  checkEEPROM();

  loadCard();
  // set seed first hole not completed or 1
  uint8_t i = 0;
  while (i < 18 && card[i] > 0) {
    i++;
  }
  if (i == 18) {
    seed = 1;
  } else {
    seed = i + 1;
  }


}



void loop() {
  // put your main code here, to run repeatedly:
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  arduboy.setRGBled(0, 0, 0);

  switch (estado) {
    //***********************************
    //  TITLE/SELECTION SCREEN
    //***********************************
    case TITLE:
      arduboy.drawBitmap(0, 0, titleimg, 32, 32, WHITE);
      arduboy.setCursor(35, 40);
      arduboy.print("Play Hole #");
      arduboy.print(seed);
      arduboy.setCursor(35, 50);
      arduboy.print("Reset Card");
      arduboy.drawBitmap(25, 39 + menu * 10, imgball, 8, 8, WHITE);

      if(menu==0){
        arduboy.setCursor(5, 40);
        arduboy.print("< >");
      }
      

      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (menu == 0) {
          seed = max(1, seed - 1);
        }
      }
      if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (menu == 0) {
          seed = min(18, seed + 1);
        }
      }
      if (arduboy.justPressed(UP_BUTTON)) {
        menu = 0;
      }
      if (arduboy.justPressed(DOWN_BUTTON)) {
        menu = 1;
      }
      drawCard(35, 0);
      if (arduboy.justPressed(A_BUTTON)) {
        if (menu == 0) initGame();
        if (menu == 1) {
          resetCard();
          saveCard();
        }
      }
      break;
    //***********************************
    //  ONHOLE: Ball in hole. WIN match
    //***********************************
    case ONHOLE:
      // INFO

      drawCard(32, 0);

      arduboy.setCursor(32, 40);
      if (seed <= 18) {
        arduboy.print("A:NEXT ");
      }
      arduboy.print("B:MENU");

      if (arduboy.justPressed(A_BUTTON)) {
        initGame();
      }
      if (arduboy.justPressed(B_BUTTON)) {
        estado = TITLE;
      }
      drawMap();
      drawBall();
      break;
    //***********************************
    //  GAME: Dialing or Launching
    //***********************************
    case DIALING:
    case LAUNCHING:
      // Text GUI
      if (estado == DIALING) {
        // Offset
        if (offset / 16 > 0) {
          arduboy.setCursor(12, 40);
          arduboy.print(offset / 16);
          arduboy.drawLine(14, 48, 14, 56, WHITE);
        }
        alcance = round( 5 * (sq(clubs[club].power * tile[getTile()].mult_power) * sin(2 * PI * 0.1)) / 7.85 );
      }

      // Club:power
      arduboy.setCursor(55, 0);
      arduboy.print(clubs[club].clubname);
      arduboy.print(":");
      arduboy.print(alcance );

      //Rest
      arduboy.setCursor(0, 10);
      arduboy.print("REST:");
      arduboy.print( round(holePos - (((int)bola.x + 16) / 16)) );

      // Hits / Par
      arduboy.setCursor(0, 0);
      arduboy.print("HIT:");
      arduboy.print(hits);


      // last distance
      /*arduboy.setCursor(0, 10);
        arduboy.print("Last:");
        arduboy.setCursor(30, 10);
        arduboy.print(max(0, (bola.x - bola.initpos) / 16));
      */



      // Terrain
      /*
        arduboy.setCursor(0, 0);
        arduboy.print(tile[getTile()].tilename);
      */


      // DIALING
      if (estado == DIALING) {
        // LAUNCH!!
        if (arduboy.justPressed(A_BUTTON)) {
          float p = tile[getTile()].mult_power * sqrt(bola.power);
          bola.vx = clubs[club].power * p * cos(PI * 0.1);
          bola.vy = -clubs[club].power * p * sin(PI * 0.1);
          offset = 0;
          bola.initpos = bola.x;
          estado = LAUNCHING;
        }

        bola.power += bola.pow_vel;
        if (bola.power <= 0 || bola.power >= 1.05) {
          bola.pow_vel *= -1;
          bola.power = constrain(bola.power, 0, 1.05);
        }

        // Select CLUB
        if (arduboy.justPressed(UP_BUTTON)) {
          club = max(0, club - 1);
        }
        if (arduboy.justPressed(DOWN_BUTTON)) {
          club = min(4, club + 1);
        }




        if (arduboy.pressed(RIGHT_BUTTON)) {
          offset += 12;
        }

        if (arduboy.pressed(LEFT_BUTTON)) {
          offset = max(0, offset - 12);
        }
        if (arduboy.pressed(B_BUTTON)) {
          offset = 0;
        }
      }

      // POW bar
      arduboy.drawRoundRect(55, 11, 68, 6, 2, WHITE);
      arduboy.drawFastHLine(57, 13, (int)(bola.power * 60), WHITE);
      arduboy.drawFastHLine(57, 14, (int)(bola.power * 60), WHITE);
      arduboy.drawFastVLine(117, 12, 4, WHITE);




      // draw MAP - BALL
      drawMap();
      drawBall();


      if (estado == LAUNCHING) {
        moveBall();

      }


      break;
    default:
      break;
  }

  arduboy.display();
}
