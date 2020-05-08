#ifndef colorConverter_h
#define colorConverter_h
#include <math.h>
#include "colorConverter.h"
#include <avr/io.h>

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

/* auxillary functions for shade */
uint16_t getGreen(uint16_t rgb);
uint16_t getRed(uint16_t rgb);
uint16_t getBlue(uint16_t rgb);
uint16_t compile(uint16_t r, uint16_t g, uint16_t b);

/* A function that lightens/darkens a color and returns the new color.

Parameters:
rgb - The original color to be brightened or darkened in svgrgb565 format
mult - The number by which each color component of rgb is multiplied by to produce the new components for the new color. A mult < 1 will produce a 
darkening effect while a mult > 1 will produce a lightening effect.

Returns:
The svrgb565 code for the color after it has been darkened/lightened
*/
uint16_t shade(uint16_t rgb,  double mult);

#endif