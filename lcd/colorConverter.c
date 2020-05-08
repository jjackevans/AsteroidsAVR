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

uint16_t getGreen(uint16_t rgb) {
	uint16_t sub = 63488 & rgb;
	uint16_t val = rgb - sub;
	return val >> 5;
}

uint16_t getRed(uint16_t rgb) {
	return rgb >> 11;
}

uint16_t getBlue(uint16_t rgb) {
	uint16_t sub = 65504 & rgb;
	return rgb - sub;
}

uint16_t compile(uint16_t r, uint16_t g, uint16_t b) {
	return (r << 11) + (g << 5) + b;
} 

uint16_t shade(uint16_t rgb,  double mult) {
	uint16_t r = getRed(rgb);
	uint16_t g = getGreen(rgb);
	uint16_t b = getBlue(rgb);
	
	r = (uint16_t) ((r * mult) + 0.5);
	g = (uint16_t) ((g * mult) + 0.5);
	b = (uint16_t) ((b * mult) + 0.5);
	
	if (r > 31) {
		r = 31;
	}
	
	if (b > 31) {
		b = 31;
	}
	
	if (g > 63) {
		g = 63;
	}
	
	return compile(r,g,b);
}



/*

typedef struct {
	double h;
	double s;
	double l;
} hsl;

hsl rgb2hsl(uint16_t rgb) {
	double r,g,b;
	
	r = getRed(rgb) / 32.0;
	g = getGreen(rgb) / 64.0;
	b = getBlue(rgb) / 32.0;
	
	double max, min;
	
	//find max
	if (r >= g && r >= b) {
		max = r;
	}
	
	else if (g >= b) {
		max = g;
	}
	
	else {
		max = b;
	}
	
	//find min
	if (r < g && r < b) {
		min = r;
	}
	
	else if (g < b) {
		min = g;
	}
	
	else {
		min = b;
	}
	
	double h, s;
	
	double l = (max + min) / 2;
	
	if (max == min) {
		h = 0;
		s = 0;
	}
	
	else {
		double diff = max - min;
		if (l > 0.5) {
			s = diff / ( 2- max - min);
		}
		
		else {
			diff / (max + min);
		}
		uint16_t i = 0;
		
		
		switch(max) {
			case r: 
				if (g < b) { 
					i = 6;
				} 
				h = (g-b)/diff + i;
				break;
			case g: 
				h = (b-r)/diff + 2;
				break;
			case b:
				h = (r-g) /diff + 4; 
				break;
		}
		
		h /= 6;
	}
	
	hsl color = {h,s,l};
	return hsl;
}

uint16_t hsltorgb(hsl color) {
	
}
*/
