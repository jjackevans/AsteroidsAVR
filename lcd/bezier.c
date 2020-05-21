#include "bezier.h"

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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

     MODIFIED BY JE1G18 - 2020

     */

int16_t findBezier(double t, uint16_t x[4]) {
	double omt = 1-t;
	
	double xt = omt*omt*omt*x[0] + 3*omt*omt*t*x[1] +
				3*omt*t*t*x[2] +  t*t*t*x[3];
				
	return (int16_t) xt+0.5;
}

// JE1G18 2020
void plotBezierPoints(Point p[4], uint16_t col, double segSize){
	uint16_t x[4] = {p[0].x, p[1].x, p[2].x, p[3].x}; 
	uint16_t y[4] = {p[0].y, p[1].y, p[2].y, p[3].y};
	plotBezier(x, y, col, segSize); 
}

void plotBezier(uint16_t x[4], uint16_t y[4], uint16_t col, double segSize) {
	int16_t px0 = findBezier(0.0, x);
	int16_t py0 = findBezier(0.0, y);
	double t;
	for (t = segSize; t < 1.0+segSize; t+=segSize) {
		int16_t px1 = findBezier(t,x);
		int16_t py1 = findBezier(t,y);
		
		drawLine(px0,py0,px1, py1, col);
		px0 = px1;
		py0 = py1;
	}
	
}

void plotBezierBuffer(uint16_t x[4], uint16_t y[4], uint16_t col, double segSize) {
	int16_t px0 = findBezier(0.0, x);
	int16_t py0 = findBezier(0.0, y);
	double t = segSize;
	uint16_t pointNo = (uint16_t) ((1/segSize) + 1.5); //+1 for n+1 points for n lines  and +0.5 for round up
	int16_t* xbuff = (int16_t*) malloc(sizeof(int16_t) * pointNo); 
	int16_t* ybuff = (int16_t*) malloc(sizeof(int16_t) * pointNo); 
	ybuff[0] = py0;
	xbuff[0] = px0;
	uint16_t i;
	for (i = 1; i < pointNo; i++) {
		int16_t px1 = findBezier(t,x);
		int16_t py1 = findBezier(t,y);
		
		ybuff[i] = py1;
		xbuff[i] = px1;
		px0 = px1;
		py0 = py1;
		t+= segSize;
	}
	
	for (i = 0; i < pointNo-1; i++ ) {
		drawLine(xbuff[i],ybuff[i],xbuff[i+1], ybuff[i+1],col);
	}
	
	free(ybuff);
	free(xbuff);
}



