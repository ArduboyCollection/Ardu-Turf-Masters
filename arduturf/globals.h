#define FAIR 0
#define GRASS 1
#define ROUGH 2
#define SAND 3
#define WATER 4
#define GREN 5  //GREEN is reserved for colours :(
#define HOLE 6


#define DIALING 0
#define LAUNCHING 1
#define STOPPED 2
#define ONGREEN 3
#define ONHOLE 4
#define TITLE 5

int estado = TITLE;


#define MAPSIZE 256  // MAX mapsize (biggers maps corrupts memory/uploading issues
#define GROUND 48 // Coordinate Y for floor

// BALL
typedef struct {
  float x = 0;
  float y = GROUND;
  float vx = 0, vy = 0;

  // dial/power vars
  float power = 0;
  float pow_vel = 0.025;
  
  

  // launching vars
  int initpos;
} Ball;

Ball bola;


// TILE for TERRAIN / MAP
typedef struct {
  byte id;
  String tilename;
  float rebound_x;
  float rebound_y;
  float mult_power;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Tile;

const Tile PROGMEM tile[7] = {
  {FAIR,  "FAIRWAY", 0.5, 0.3, 1,   0,55,0},
  {GRASS, "GRASS  ", 0.4, 0.25, 0.92, 0,55,0},
  {ROUGH, "ROUGH  ", 0.3, 0.2, 0.7, 0,50,0},
  {SAND,  "BUNKER ", 0.2, 0.15, 0.4, 55,55,0},
  {WATER, "WATER  ", 0, 0, 0,       0,0,55},
  {GREN,  "GREEN  ", 0.7, 0.25, 1,   0,55,0},
  {HOLE,  "HOLE   ", 0.0, 0.7, 0,   55,55,55},
};

