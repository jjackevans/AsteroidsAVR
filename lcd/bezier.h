#ifndef bezier_h
#define bezier_h
#include <avr/io.h>
#include "bresenham.h"
#include "bezier.h"
#include "wu.h"
#include <stdlib.h>

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

/*The bezier library is a library for drawing simple curves. This library is an implementation of cubic bezier curves, please note that b-splines are not
supported.*/


/* Auxillary function for drawing bezier curves */
int16_t findBezier(double t, uint16_t x[4]);

/* A function for drawing cubic bezier curves.

Parameters:
x - An array of x coordinates needed to build the curve. x[0] represents the x coordinate of the start point of the line. x[1] represents the 
x coordinate of the first control point. x[2] represents the x coordinate of the second control point. x[3] represents the x coordinate
of the end point of the line.
y - The same as above but for the y coordinates of each point.
col - The color of the curve in svgrgb565 format
segSize - A value between 0 and 1 that effects the number of straight lines used to build the curve. The closer segSize is to 0 the more, straight
lines will be used to build the curve, generally resulting in a smoother effect, but bear in mind this comes at computational cost. The smaller
the value of segSize the faster the curve will be drawn, but the more "jagged" it will be.
*/

void plotBezier(uint16_t x[4], uint16_t y[4], uint16_t col, double segSize);

void plotBezierPoints(Point p[4], uint16_t col, double segSize);

/* The same function as plotBezier, except that points for each line segment are stored inside a buffer until all points have been 
generated. All line segments are not drawn on the screen until all line segment points have been calculated. This means there may be a small delay
between the curve appearing on the screen and the function being called, but the curve will display all at once instead of line segment by line segment
like when plotBezier is used. This has memory implications and requires 4*(1/segSize) bytes of memory that plotBezier does not need */
void plotBezierBuffer(uint16_t x[4], uint16_t y[4], uint16_t col, double segSize);
#endif