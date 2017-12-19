void loadCard(){
  arduboy.setRGBled(0, 100, 0);
  for(int i = 0;i<18;i++){
    card[i] = EEPROM.read(eepromAddress+i);
  }
  arduboy.setRGBled(0, 0, 0);
}

void saveCard(){
  arduboy.setRGBled(0, 0, 100);
  for(int i = 0;i<18;i++){
    EEPROM.update(eepromAddress+i,card[i]);
  }
  arduboy.setRGBled(0, 0, 0);
}


void checkEEPROM(){
  arduboy.setRGBled(0, 100, 0);
  // check if there's any weird value, if yes -> saves card
  bool weird = false;
  for(int i = 0;i<18;i++){
    uint8_t v = EEPROM.read(eepromAddress+i);
    if(v==255){
      weird = true;
    }
  }
  arduboy.setRGBled(0, 0, 0);
  if(weird) saveCard();
}

