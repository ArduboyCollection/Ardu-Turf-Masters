void initGame() {
  randomSeed(seed); // set Seed / course
  fillMap();

  bola.x = 0;
  bola.y = GROUND;
  hits = 1;
  club = 0;
  par = max(3,floor(holePos / 70) + 2);

  arduboy.clear();
  arduboy.drawBitmap(0, 18, titleimg, 32, 32, WHITE);

  // print info!
  arduboy.setCursor(37, 20);
  arduboy.print("HOLE #");
  arduboy.print(seed);

  arduboy.setCursor(37, 30);
  arduboy.print("DIST:");
  arduboy.print(holePos);
  arduboy.print(" yards");

  arduboy.setCursor(37, 40);
  arduboy.print("PAR:");
  arduboy.print(par);


  arduboy.display();
  delay(3000);

  estado = DIALING;
}

void resetCard() {
  for (uint8_t i = 0; i < 18; i++) card[i] = 0;
  seed = 1;
}
void drawCard(uint8_t x, uint8_t y) {
  int suma = 0;
  for (uint8_t i = 0; i < 18; i++) {
    uint8_t xx = (i % 9) * 10;
    uint8_t yy = (i / 9) * 12;
    arduboy.drawRoundRect(x, y, 93, 25, 2, WHITE);
    arduboy.drawFastHLine(x, y + 12, 93, WHITE);

    uint8_t offs = (card[i] > 9) ? 1 : 4;

    arduboy.setCursor(x + xx + offs, y + yy + 3);
    if ((seed - 1) != i || (arduboy.frameCount % 4) != 0) {
      if (card[i] > 0) {
        arduboy.print(card[i]);
      } else {
        arduboy.print("-");
      }
    }
    suma += card[i];
  }

  arduboy.setCursor(x, y + 27);
  arduboy.print("Score:");
  arduboy.print(suma);

}

