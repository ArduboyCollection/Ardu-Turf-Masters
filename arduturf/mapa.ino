// Map functions


void fillMap() {

  // Green + HOLE positions
  holePos = floor(random(60, 250)); // Position of hole
  greenSize = 7;
  greenIni = holePos - greenSize; //holePos - floor(greenSize / 2);
  greenEnd = holePos - 1; //holePos + greenSize / 2;

  int next = 10;
  int tile = FAIR;
  for (int i = 0; i < MAPSIZE - 1; i++) {
    //M[i] = floor(random(0,5));  // Totally random
    // make 'zones' of every material
    if (i > holePos) {
      tile = FAIR; // After the HOLE, all tiles are FAIR
    }
    M[i] = tile;
    next--;
    if (next == 0) {
      int ntile;
      do {
        ntile =  floor(random(0, 5));
      } while (ntile == tile);
      tile = ntile;



      if (tile == WATER) {
        i++;
        M[i] = GRASS;
      }
      switch (tile) {
        case SAND:
          next = random(3, 6);
          break;
        case WATER:
          next = random(3, 7);
          break;
        case FAIR:
          next = random(6, 18);
          break;
        default:
          next = random(3, 10);
          break;
      }
    }
  }
  M[MAPSIZE - 1] = FAIR;

  // GREEN/ HOLE

  for (int i = greenIni; i <= greenEnd; i++) {
    M[i] = GREN;
  }
  M[holePos] = HOLE;
}
int getTile() {
  return M[ min(MAPSIZE - 1, ((int)bola.x + 16) / 16 )];
}
void drawMap() {
  for (int i = 0; i < 9; i++) {
    int ntile = i + ((int)bola.x + offset) / 16;
    int xx = i * 16 - ((int)bola.x + offset) % 16;
    arduboy.drawBitmap(xx, 48, tiles[M[min(MAPSIZE - 1, ntile)]], 16, 16, WHITE);

    if (ntile > 0 && ntile<greenIni && ntile % 25 == 0) {
      arduboy.drawRoundRect(xx - 6, 34, (ntile > 99) ? 29 : 24 , 11, 4, WHITE);
      arduboy.drawFastVLine((ntile > 99) ? xx + 8 : xx + 5 , 45, 11, WHITE);
      arduboy.setCursor(i * 16 - ((int)bola.x + offset) % 16, 36);
      arduboy.print(ntile);

    }

    // draw Flag
    if (ntile == holePos) {
      arduboy.drawFastVLine(xx + 8, GROUND - 22, 30, WHITE);
      arduboy.drawTriangle(xx + 8, GROUND - 22, xx + 20, GROUND - 18, xx + 8, GROUND - 14, WHITE);
    }

  }

}

