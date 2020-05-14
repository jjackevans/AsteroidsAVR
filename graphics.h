#ifndef graphics_h
#define graphics_h
#include "bezier.h"
#include "bresenham.h"
#include "wu.h"
#include "svgrgb565.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SHIP_LENGTH 20
#define SHIP_WIDTH  16
#define SHIP_COLOUR ALICE_BLUE
#define TIMEBAR_COLOUR GREEN
#define FUEL_CELL_COLOUR GREEN
#define ASTEROID_COLOUR RED

/**
 * Defines a set vertexes used to draw the ship.
 */
typedef struct ShipVertex{
	Point nose;
	Point leftRear;
	Point rightRear;
	Point leftCentre;
	Point rightCentre;
} ShipVertex;

/**
 * Defines an 'Asteroid' object.
 */
typedef struct Asteroid{
	Point loc;  			// Location on display
	Point vel;				// Velocity vectors
	int32_t rad;            // Radius
	int8_t alive;           // (Still on screen)
} Asteroid;

void init_graphics(void);

/**
 * Moves the ship, using the rotary encoder position and speed.
 * TODO: Move with acceleration
 * @param pos - position of ship
 * @param angle - position of rotary encoder
 * @param nextPos - address to store position of resulting movement
 * @param speed - pixels to move ship by
 */
void move_ship(Point pos, int angle, Point *nextPos, int speed);

/**
 * Moves an individual asteroid by its velocity.
 * @param a - asteroid object to move
 */
void move_asteroid(Asteroid *a);

/**
 * Draws a timebar representing the time left to obtain the fuel cell.
 * @param timeLeft - value 0-10(s)
 */
void draw_fuel_cell_timebar(int timeLeft);

/**
 * Draws the current user score in the bottom left hand corner.
 * @param score
 */
void draw_points(uint16_t score);

/**
 * Draws the fuel cell.
 * @param p - position of the fuel cell.
 */
void draw_fuel_cell(Point p);
/**
 * Removes the fuel cell.
 * @param p - position of the fuel cell.
 */
void remove_fuel_cell(Point p);

#endif