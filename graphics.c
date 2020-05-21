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

#include "graphics.h"
#include <string.h>

#define PI 3.14159265

ShipVertex previousDraw;
ShipVertex toDraw;
ShipVertex natural;
ShipVertex sv1;
ShipVertex sv2;
ShipVertex sv3;

uint16_t ship_colour;
Point startLoc;

void draw_asteroid(Point pos, int32_t r, uint16_t col);
void translate(Point *p, Point o);
void rotate(Point *p, int angle);
void position_ship(ShipVertex *sv, int angle, Point pos);
void draw_ship(ShipVertex sv, uint16_t col);

/*
 * Constructs the natural ship (ship vertex at origin)
 */
void init_graphics(void){
    natural.nose.x = 0;
    natural.nose.y = - SHIP_LENGTH/2;
    natural.leftRear.x = - SHIP_WIDTH/2;
    natural.leftRear.y = SHIP_LENGTH/2;
    natural.rightRear.x = SHIP_WIDTH/2;
    natural.rightRear.y = SHIP_LENGTH/2;
    natural.leftCentre.x = - SHIP_LENGTH/2;
    natural.leftCentre.y = 0;
    natural.rightCentre.x = SHIP_LENGTH/2;
    natural.rightCentre.y = 0;
    previousDraw=natural;
    init_bres();
    char s1[] = "               _                 _     _     \n";
    char s2[] = "     /\\       | |               (_)   | |    \n";
    char s3[] = "    /  \\   ___| |_ ___ _ __ ___  _  __| |___ \n";
    char s4[] = "   / /\\ \\ / __| __/ _ \\ '__/ _ \\| |/ _` / __|\n";
    char s5[] = "  / ____ \\\\__ \\ ||  __/ | | (_) | | (_| \\__ \\\n";
    char s6[] = " /_/    \\_\\___/\\__\\___|_|  \\___/|_|\\__,_|___/\n";
    draw_centered_text(s1,  - 34);
    draw_centered_text(s2,  - 26);
    draw_centered_text(s3,  - 18);
    draw_centered_text(s4,  - 10);
    draw_centered_text(s5,  - 2);
    draw_centered_text(s6,    6);
    int offset = 12;
    sv1 = natural;
    sv2 = natural;
    sv3 = natural;
    Point d1 = {LCDHEIGHT/2 -(9*3 + 12*6) -2 , LCDWIDTH/2 +39 + offset + 1};
    Point d2 = {LCDHEIGHT/2, LCDWIDTH/2 +39 + offset + 1};
    Point d3 = {LCDHEIGHT/2 +(9*3 + 12*6) -2 , LCDWIDTH/2 +39 + offset + 1};
    sv1.centre=d1;
    sv2.centre=d2;
    sv3.centre=d3;
    position_ship(&sv1, 0, d1);
    position_ship(&sv2, 0, d2);
    position_ship(&sv3, 0, d3);
    highlight_difficulty(0);
}

// rectangle left, right, top, bottom
Point highlight_difficulty(int difficulty){
    if(difficulty==0){
        ship_colour = ALICE_BLUE;
        draw_ship(sv1, ship_colour);
        ship_colour=BLUE_VIOLET;
        draw_ship(sv3, ship_colour);
        ship_colour=ROYAL_BLUE;
        draw_ship(sv2, ship_colour);
    }
    if(difficulty==1){
        startLoc = sv1.centre;
        ship_colour = ALICE_BLUE;
        draw_ship(sv1, ship_colour);
        draw_ship(sv2, display.background);
        draw_ship(sv3, display.background);
    }
    else if(difficulty==2){
        startLoc = sv2.centre;
        ship_colour=ROYAL_BLUE;
        draw_ship(sv1, display.background);
        draw_ship(sv2, ship_colour);
        draw_ship(sv3, display.background);
    }
    else if(difficulty==3){
        startLoc = sv3.centre;
        ship_colour=BLUE_VIOLET;
        draw_ship(sv1, display.background);
        draw_ship(sv2, display.background);
        draw_ship(sv3, ship_colour);
    }else if(difficulty==4){
        draw_ship(sv1, display.background);
        draw_ship(sv2, display.background);
        draw_ship(sv3, display.background);
    }
    return startLoc;
}

void draw_centered_text(char *str, int8_t y){
      display_string_xy(str,LCDHEIGHT/2-strlen(str)*3, LCDWIDTH/2 +y);
}

void move_ship(Point pos, int angle, Point *nextPos, int speed){
	(*nextPos).y = -speed;
	(*nextPos).x = 0;
	rotate(nextPos, angle);
	translate(nextPos, pos);
	toDraw = natural;
	position_ship(&toDraw, angle, pos);
	if(previousDraw.nose.x!=natural.nose.x){
		draw_ship(previousDraw,display.background);
	}
	draw_ship(toDraw,ship_colour);
	previousDraw=toDraw;
}

/*
 * Auxiliary functions to move the ship
 */
void position_ship(ShipVertex *sv, int angle, Point pos){
	rotate(&(*sv).nose, angle);
	rotate(&(*sv).leftRear, angle);
	rotate(&(*sv).rightRear, angle);
	rotate(&(*sv).leftCentre, angle);
	rotate(&(*sv).rightCentre, angle);
	translate(&(*sv).nose, pos);
	translate(&(*sv).leftRear, pos);
	translate(&(*sv).rightRear, pos);
	translate(&(*sv).leftCentre, pos);
	translate(&(*sv).rightCentre, pos);
}

void draw_ship(ShipVertex sv, uint16_t col){
	drawLinePoints(sv.nose, sv.leftRear, col);
	drawLinePoints(sv.nose, sv.rightRear, col);
	Point rear[] = {sv.leftRear, sv.leftCentre, sv.rightCentre, sv.rightRear};
	plotBezierPoints(rear, col, 0.5);
}

void rotate(Point *p, int angle){
    int16_t newX, newY;
    float val = angle * PI/ 180;
    newX = (*p).x * cos(val) - (*p).y * sin(val);
    newY = (*p).x * sin(val) + (*p).y * cos(val);
    (*p).x = newX;
    (*p).y = newY;

}

void move_asteroid(Asteroid *a){
    Point n = (*a).loc;
    translate(&n, (*a).vel);
    draw_asteroid( (*a).loc, (*a).rad, display.background);
    if(n.x > LCDHEIGHT-1 || n.x < 5 || n.y > LCDWIDTH-1 || n.y <1){
        (*a).alive = 0;
    }else{
        draw_asteroid( n, (*a).rad, ASTEROID_COLOUR);
    }
    (*a).loc = n;
}
/*
 * Auxiliary functions to move the asteroid
 * TODO: Improve asteroid shape
 */
void draw_asteroid(Point pos, int32_t r, uint16_t col){
    drawCircle( pos.x, pos.y, r, col);
}

/*
 * TODO: Improve fuel cell graphics
 */
void draw_fuel_cell(Point p){
    drawCircle( p.x, p.y, 4, FUEL_CELL_COLOUR);
    drawCircle( p.x, p.y, 3, YELLOW);
    drawCircle( p.x, p.y, 2, FUEL_CELL_COLOUR);
    drawCircle( p.x, p.y, 1, FUEL_CELL_COLOUR);
}

void remove_fuel_cell(Point p){
    drawCircle( p.x, p.y, 4, display.background);
    drawCircle( p.x, p.y, 3, display.background);
    drawCircle( p.x, p.y, 2, display.background);
    drawCircle( p.x, p.y, 1, display.background);
}

void draw_fuel_cell_timebar(int timeLeft){
    rectangle c = {0, LCDHEIGHT, 0, 5};
    fill_rectangle(c, display.background);
    if(timeLeft==0){
        return;
    }
    uint16_t length = (LCDHEIGHT/10)*timeLeft;
    rectangle p = {0, length, 0, 5};
    fill_rectangle(p, TIMEBAR_COLOUR);
}

void draw_points(uint16_t score){
    rectangle c = {0, 5, LCDWIDTH-15, LCDWIDTH-10};
    fill_rectangle(c, display.background);
    char buffer[25];
    itoa(score, buffer, 10);
    display_move(0, LCDWIDTH-15);
    display_string(&buffer[0]);
}

/*
 * Auxiliary function for both asteroid and ship movement
 */
void translate(Point *p, Point o){
    int16_t newX, newY;
    newX = (*p).x + o.x;
    newY = (*p).y + o.y;
    (*p).x = newX;
    (*p).y = newY;
}


