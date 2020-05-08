#ifndef wu_h
#define wu_h
#include "bresenham.h"
#include <math.h>
#include "colorConverter.h"

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

/* A library for drawing anti-aliased lines using wu's algorithm */

/* auxillary functions for drawAAline */
double fraction(double val);
double remainingFraction(double val);
void swapDouble(double *x, double *y);

/* This function is used for drawing anti-aliased lines. Lines are drawn using Xiaolin Wu's algorithm so take slightly longer to load than lines drawn
using bresenham's algorithm.

Parameters:
x0 - The  coordinate of the start point of the line
y0 - The y coordinate of the start point of the line
x1 - The x coordinate of the end point of the line
y1 - The y coordinate of the end point of the line
col - The color of the line in svgrgb565 format*/
void drawAALine(double x0, double y0, double x1, double y1, uint16_t col);


#endif