// Copyright 2020 je1g18
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the "Software")
//  , to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef graphics_h
#define graphics_h
#include "bezier.h"
#include "bresenham.h"
#include "wu.h"
#include "svgrgb565.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HIGHLIGHT_COLOUR ALICE_BLUE
#define SHIP_LENGTH 20
#define SHIP_WIDTH  16
#define TIMEBAR_COLOUR GREEN
#define FUEL_CELL_COLOUR GREEN
#define ASTEROID_COLOUR RED

/**
 * Defines a set vertexes used to draw the ship.
 */
typedef struct ShipVertex{
    Point centre;
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

/**
 * Draws text horizontally centrally aligned.
 * @param str - string to write.
 * @param y - vertical offset from centre.
 */
void draw_centered_text(char *str, int8_t y);

/**
 * Draws spaceship below selected difficulty.
 * @param difficulty - to highlight.
 */
Point highlight_difficulty(int difficulty);


#endif