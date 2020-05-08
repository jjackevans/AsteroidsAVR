#ifndef bresenham_h
#define bresenham_h
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "ili934x.h"
#include "lcd.h"
#include "bresenham.h"


 /*{Small Graphics library for the LaFortuna}
    Copyright (C) {2016}  {Nicholas Bishop}

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* The bresenham library provides an efficient way of drawing lines using the bresenham algorithm. The bresenham library
also provides an efficient function for drawing circles, based off the midpoint circle algorithm. */

/* Initialising function that initialises the LCD screen */
void init_bres();

/* Draws a straight line on the screen using bresenham's line algorithm 

Parameters:
x1 - The  coordinate of the start point of the line
y1 - The y coordinate of the start point of the line
x2 - The x coordinate of the end point of the line
y2 - The y coordinate of the end point of the line
col - The color of the line in svgrgb565 format
*/
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t col);

void drawLinePoints(Point p1, Point p2, uint16_t col);

/* Colors a pixel on the screen at the specified x and y coordinates with the specified color (in svgrgb565 format) 

Parameters:
x - The  coordinate of the pixel
y - The y coordinate of the spixel
col - The new color of the pixel in svgrgb565 format
*/
void plotPixel(uint16_t x, uint16_t y, uint16_t col);

/*auxillary functions for drawing lines */
uint16_t findOctant(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void swap(int16_t *x, int16_t *y);
void switchFromOctant0(int16_t *x, int16_t *y, uint16_t octant);
void toOctant0(int16_t *x, int16_t *y, uint16_t octant);

/*Draws a circle on the screen using the midpoint circle algorithm

Parameters:
xc - The x coordinate of the centre of the circle
yc - The y coordinate of the centre of the circle
r - The radius of the circle
col - The color of the circle in svgrgb565 format
*/
void drawCircle(uint16_t xc, uint16_t yc, int32_t r, uint16_t col);

/* auxillary function for drawing circles */
void auxCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t col);

#endif