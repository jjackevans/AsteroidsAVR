#include "lcd.h"
#include "bezier.h"
#include "bresenham.h"
#include "wu.h"
#include "svgrgb565.h"

typedef struct ShipVertex{
	Point nose;
	Point leftRear;
	Point rightRear;
	Point leftCentre;
	Point rightCentre;
} ShipVertex;

typedef struct Asteroid{
	Point loc;  			// Location on display
	Point vel;				// Velocity vectors
	int32_t rad;
	int8_t alive;
} Asteroid;


void move_ship(Point pos, int angle, Point *nextPos, int speed);
void position_ship(ShipVertex *sv, int angle, Point pos);
void draw_ship(ShipVertex sv, uint16_t col);
void rotate(Point *p, int angle);
void translate(Point *p, Point o);
void debug(char msg[], int var);
void move_asteroid(Asteroid *a);
void draw_fuel_cell_timebar(int timeLeft);
