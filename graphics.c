#include "graphics.h"

#define PI 3.14159265

ShipVertex previousDraw;
ShipVertex toDraw;
ShipVertex natural;

void draw_asteroid(Point pos, int32_t r, uint16_t col);
void translate(Point *p, Point o);
void rotate(Point *p, int angle);
void position_ship(ShipVertex *sv, int angle, Point pos);
void draw_ship(ShipVertex sv, uint16_t col);

/*
 * Constructs the natural ship (ship vertex at origin)
 */
void init_graphics(void){
	init_bres();
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
	draw_ship(toDraw,SHIP_COLOUR);
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
    rectangle c = {0, 20, LCDWIDTH-20, LCDHEIGHT};
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

void debug(char msg[], int var){
	char buffer[25];
	itoa(var, buffer, 10);
	display_string("\n");
	display_string(msg);
	display_string(" : ");
	display_string(buffer);
}


