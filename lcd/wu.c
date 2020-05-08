#include "wu.h"


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

double fraction(double val) {
	if (val<0) {
		return 1 - (val - floor(val));
	}
	
	return val - floor(val);
	
}

double remainingFraction(double val) {
	return 1 - fraction(val);
}

void drawAALine(double x0, double y0, double x1, double y1, uint16_t col) {
	double steep = fabs(y1 - y0) - fabs (x1 - x0);
	
	if (steep > 0) {
		swapDouble(&y0, &x0);
		swapDouble(&y1, &x1);
	}
	
	if (x0 > x1) {
		swapDouble(&x0, &x1);
		swapDouble(&y1, &y0);
	}
	
	double dx = x1 - x0;
	double dy = y1 - y0;
	
	double gradient = dy / dx;
	
	int16_t xend = (int16_t) (x0 +0.5);
	double yend = y0 + gradient * (xend-x0);
	double xgap = remainingFraction(x0 + 0.5);
	uint16_t xpxl1 = (uint16_t) xend;
	uint16_t ypxl1 = (uint16_t) yend;
	
	if(steep > 0) {
		plotPixel(ypxl1, xpxl1, shade(col,1+remainingFraction(yend)*xgap));
		plotPixel(ypxl1+1,xpxl1, shade(col,1+fraction(yend)*xgap));
	}
	else {
		plotPixel(xpxl1, ypxl1, shade(col,1+remainingFraction(yend)*xgap));
		plotPixel(xpxl1,ypxl1+1, shade(col,1+fraction(yend)*xgap));
	}
	
	double intery = yend + gradient;
	
	xend = (int16_t) (x1 +0.5);
    yend = y1 + gradient * (xend - x1);
    xgap = remainingFraction(x1 + 0.5);
    uint16_t xpxl2 = (uint16_t) xend; 
    uint16_t ypxl2 = (uint16_t) yend;
    if (steep > 0) {
        plotPixel(ypxl2  , xpxl2, shade(col,remainingFraction(yend) * xgap));
        plotPixel(ypxl2+1, xpxl2, shade(col,fraction(yend) * xgap));
	}
    else {
        plotPixel(xpxl2, ypxl2,  shade(col,remainingFraction(yend) * xgap));
        plotPixel(xpxl2, ypxl2+1, shade(col,fraction(yend) * xgap));
	}
	
	uint16_t i;
	
	for(i = xpxl1+1; i<xpxl2;i++) {
		if(steep >0) {
			plotPixel((uint16_t) intery  , i, shade(col,remainingFraction(intery)));
            plotPixel((uint16_t) intery+1, i, shade(col,fraction(intery)));
		}
		else {
			plotPixel(i,(uint16_t) intery  , shade(col,remainingFraction(intery)));
            plotPixel(i,(uint16_t) intery+1, shade(col,fraction(intery)));
		}
		intery = intery + gradient;
	}
	
}

void swapDouble(double *x, double *y) {
	double z;
	z = *x;
	*x = *y;
	*y  = z;
}