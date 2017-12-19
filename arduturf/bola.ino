

void drawBall() {
  arduboy.drawBitmap(10 - offset, bola.y, imgball, 8, 8, WHITE);
}


void moveBall() {
  bola.x += bola.vx;
  bola.y += bola.vy;

  // rebote
  if (bola.y >= GROUND) {
    int t = getTile();
    arduboy.setRGBled(tile[getTile()].r, tile[getTile()].g, tile[getTile()].b);
    if (t == WATER) {
      // WATER!! back to last not water tile
      stopBall();
      do {
        bola.x -= 16;
      } while (tile[getTile()].id == WATER);
      estado = DIALING;
    } else {
      // REBOUND
      bola.y = GROUND - (bola.y - GROUND);
      bola.vy *= -tile[t].rebound_y;
      bola.vx *= tile[t].rebound_x;


    }

    // testing... stop in first rebound
    /*
      bola.y = GROUND;
      bola.vx = 0;
      bola.vy = 0;
    */

    if (bola.vx < 0.5) {
      stopBall();
    }
  } else {
    bola.vy += 0.1; // gravity
  }
}

void stopBall() {
  bola.vx = 0;
  bola.vy = 0;
  bola.y = GROUND;

  int xmessage = 30;
  // Check if pass the GREEN
  int ntile =  ((int)bola.x + 16) / 16;
  if (ntile > holePos) {
    bola.x = bola.initpos;
    xmessage = 42;
  }

  if (estado == LAUNCHING) {
    if (getTile() == HOLE) {
      bola.y = GROUND + 7;
      bola.x = holePos * 16 - 6;
    //  arduboy.clear();
    //  drawMap();
    //  drawBall();
    }

    //Message
    arduboy.fillRect(0, 22, 127, 14, BLACK);
    arduboy.drawFastHLine(0, 22, 128, WHITE);
    arduboy.drawFastHLine(0, 36, 128, WHITE);
    arduboy.setCursor(xmessage, 26);
    if (xmessage == 30) {
      if(getTile()!=HOLE){
        arduboy.print(tile[getTile()].tilename);
      }else{
        uint8_t dif = hits-par;
        if(hits==1){
            arduboy.print("HOLE IN ONE!!!");
        }else{
          switch(dif){
            case -3:
              arduboy.print("ALBATROS");
              break;
            case -2:
              arduboy.print("EAGLE");
              break;
            case -1:
              arduboy.print("BIRDIE");
              break;
            case 0:
              arduboy.print("PAR");
              break;
            case 1:
              arduboy.print("BOGEY");
              break;
            case 2:
              arduboy.print("DOUBLE BOGIE");
              break;
            default:
              arduboy.print("PAR +");
              arduboy.print(dif);
              break;
          }
        }
      
      }

      
    } else {
      arduboy.print("OUT!!");
      arduboy.setRGBled(255, 0, 0);
    }

    arduboy.display();
    delay(2000);
  }

  // check HOLE
  if (getTile() == HOLE) {
    estado = ONHOLE;
    card[seed-1] = hits;
    seed++;
    if(seed>18) seed = 1;

    saveCard();
    
  } else {
    hits++;
    estado = DIALING;
  }

}

