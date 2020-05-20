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





void init_bres() {
	init_lcd();
}

void drawLinePoints(Point p1, Point p2, uint16_t col){
	drawLine(p1.x, p1.y, p2.x, p2.y, col);
}

void drawRectangle(rectangle r , uint16_t col){
    Point tl = {r.left, r.top};
    Point tr = {r.right, r.top};
    Point br = {r.right, r.bottom};
    Point bl = {r.left, r.bottom};
    drawLinePoints(tl,tr,col);
    drawLinePoints(tr,br,col);
    drawLinePoints(br,bl,col);
    drawLinePoints(bl,tl,col);
}

void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t col) {
	// if(x1<0){x1=0;}
	// if(x2<0){x2=0;}
	// if(y1<0){y1=0;}
	// if(y2<0){y2=0;}
	// if(x1>LCDWIDTH){x1=LCDWIDTH;}
	// if(x2>LCDWIDTH){x2=LCDWIDTH;}
	// if(y1>LCDHEIGHT){y1=LCDHEIGHT;}
	// if(y2>LCDHEIGHT){y2=LCDHEIGHT;}

	plotPixel((uint16_t) x1, (uint16_t) y1, col);
	
	uint16_t oct = findOctant(x1,y1,x2,y2);
	
	toOctant0(&x1, &y1, oct);
	toOctant0(&x2, &y2, oct);
	
	int16_t posx;
	int16_t posy = y1;
	int16_t diffx =  x2 -  x1;
	int16_t diffy =   y2 -  y1;
	int16_t twoDiffy = 2 *diffy;
	int32_t p = (int32_t) twoDiffy -  (int32_t) diffx;
	int16_t tempx;
	int16_t tempy;
	
	
	for (posx = x1+1; posx <= x2; posx++) {
		if (p < 0) {
			p = p + twoDiffy;
		}
			
	
		
		else {
			posy++;
			p = p + twoDiffy - 2*diffx;	
		}
		
		tempx = posx;
		tempy = posy;
		
		switchFromOctant0(&tempx, &tempy, oct);
		
		plotPixel((uint16_t)tempx , (uint16_t) tempy, col);
		
	}
}

void switchFromOctant0(int16_t *x, int16_t *y, uint16_t octant) {
	switch(octant)  {
     case 0: break;
     case 1: 
		swap(x,y);
		break;
     case 2: 
		*y = -*y;
		swap(x,y);
		break;
     case 3: 
		*x = -*x;
		break;
     case 4: 
		*x = -*x;
		*y = -*y;
		break;
     case 5: 
		*x = -*x;
		*y = -*y;
		swap(x,y);
		break;
     case 6: 
		*x = -*x;
		swap(x,y);
		break;
     case 7: 
		*y = -*y;
		break;
	}
}

void toOctant0(int16_t *x, int16_t *y, uint16_t octant) {
	switch(octant)  {
     case 0: break;
     case 1: 
		swap(x,y);
		break;
     case 6: 
		*y = -*y;
		swap(x,y);
		break;
     case 3: 
		*x = -*x;
		break;
     case 4: 
		*x = -*x;
		*y = -*y;
		break;
     case 5: 
		*x = -*x;
		*y = -*y;
		swap(x,y);
		break;
     case 2: 
		*x = -*x;
		swap(x,y);
		break;
     case 7: 
		*y = -*y;
		break;
	}
}

void swap(int16_t *x, int16_t *y) {
	uint16_t z;
	z = *x;
	*x = *y;
	*y  = z;
}

uint16_t findOctant(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	int16_t dy = y2 - y1;
	int16_t dx = x2 - x1;
	
	if (fabs(dy) > fabs(dx)) {
		if (dy < 0) {
			
			if (dx < 0) {
				return 5;
			}
			
			else {
				return 6;
			}
		}
		
		else {
			if (dx < 0) {
				return 2;
			}
			
			else {
				return 1;
			}
		}
	}
	
	else {
		if (dy < 0) {
			
			if (dx < 0) {
				return 4;
			} 
			
			else {
				return 7;
			}
		}
		
		else {
			if (dx < 0) {
				return 3;
			}
			
			else {
				return 0;
			}
		}
	}
}



void plotPixel(uint16_t x, uint16_t y, uint16_t col) {
	
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(x);
	write_data16(x);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(y);
	write_data16(y);
	write_cmd(MEMORY_WRITE);
	write_data16(col);
	
}

void drawCircle(uint16_t xc, uint16_t yc, int32_t r, uint16_t col) {
	int32_t x = 0;
	int32_t y = r;
	int32_t diff = 3 - 2*r;
	
	while (x< y) {
		auxCircle(xc,yc, (uint16_t) x, (uint16_t) y,col);
		x++;
		
		if (diff < 0) {
			diff = diff + 4*x + 6;
		}
		
		else {
			y--;
			diff = diff + 4*(x-y) + 10; 
		}
		
		auxCircle(xc,yc, (uint16_t) x, (uint16_t) y,col);
	}
}

void auxCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t col) {
	plotPixel(xc+x, yc+y, col);
	plotPixel(xc-x, yc+y, col);
	plotPixel(xc+x, yc-y, col);
	plotPixel(xc-x, yc-y, col);
	plotPixel(xc+y, yc+x, col);
	plotPixel(xc-y, yc+x, col);
	plotPixel(xc+y, yc-x, col);
	plotPixel(xc-y, yc-x, col);
}

	
	