/* COMP2215 Task 5---SKELETON */

#include "os.h"
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>

#define MAX_ROIDS 15
#define PLAYING 1
#define ENDING 2
#define LED_ON      PORTB |=  _BV(PINB7)
#define LED_OFF     PORTB &= ~_BV(PINB7)


//TODO: Ship colour changes on stage increments
//TODO:

void init(void);
int collect_delta(int);
void update_ship(void);
int update_asteroids(int);
int check_pos(int);
uint16_t rand_num(uint16_t limit) ;
void create_asteroid(Asteroid *a);
int refresh_screen(int);
int fuel_cell(int state);
int increase_difficulty(int);

Asteroid astroids[MAX_ROIDS];
int astroid_count;
int game_state = 0;  
int position   = 0;
int asteroid_state=0;
int cells_collected=0;
Point p;


Point fuelCell;
int fuel_cell_active;

uint8_t max_roid_speed;
uint8_t curr_roids;
uint8_t max_roid_radius;
uint8_t ship_speed;

void main(void) {
	init();
    os_init();

	init_graphics();

    os_add_task( collect_delta,     60, 1 );
    os_add_task( update_asteroids,  6, 0 );
    os_add_task( check_pos,        120, 0 );
    os_add_task( increase_difficulty, 10000, 0);
    os_add_task( fuel_cell, 1000,0);

    sei();;
    for(;;){}

}


void init(void){
	LED_ON;
	srand(time(NULL));
	p.x = LCDHEIGHT/2;
	p.y = LCDWIDTH/2;
	astroid_count=0;
	ship_speed=3;
	max_roid_speed = 2;
    curr_roids = 3;
	max_roid_radius = 2;

	game_state = PLAYING;
	
}

int fuel_cell(int state){
    if(game_state != PLAYING){ return ; };
    if(state==10){
        fuelCell.x = rand_num(LCDHEIGHT-60)+30;
        fuelCell.y = rand_num(LCDWIDTH-50)+25;
        draw_fuel_cell(fuelCell);
        draw_fuel_cell_timebar(state);
        fuel_cell_active=1;
    }
    if(state<10 && fuel_cell_active){
        draw_fuel_cell(fuelCell);
        if(state==0){
            game_state = ENDING;
        }
        draw_fuel_cell_timebar(state);
    }
    return state > 0 ? state-1 : 14;
}

int increase_difficulty(int state){
    if(state %2 && curr_roids <= MAX_ROIDS){
        curr_roids++;
    }
    if(state % 3==0){
        max_roid_speed++;
    }
    if(state % 7==0){
        max_roid_radius++;
    }
    if(state % 5==0){
        ship_speed++;
    }
    return state+1;
}

int collect_delta(int state) {
	int v = os_enc_delta();
	position += v;
	update_ship();
	return state;
}

int refresh_screen(int state){
    clear_screen();
    return state;
}

void update_ship(void) {
	if(game_state != PLAYING){ return ; };
	Point n;
	move_ship(p, position*10, &n, ship_speed);
	p=n;
	draw_points(cells_collected);
	// asteroid_state = update_asteroids(asteroid_state);
	// return state;
}

int update_asteroids(int state) {
    if (game_state != PLAYING) { return state; };
    if (state < curr_roids) {
        if (astroids[state].alive == 1) {
            move_asteroid(&astroids[state]);
        } else {
            create_asteroid(&astroids[state]);
        }
    }
	return state < MAX_ROIDS ? state+1 : 0 ;
//	for(i=0; i< max_roids;i++){
//		if(astroids[i].alive == 1){
//			move_asteroid(&astroids[i]);
//		}else{
//			create_asteroid(&astroids[i]);
//		}
//	}
//	return state;
}

int check_pos(int state){
	if(game_state != PLAYING){ return state; };
	if(p.x > LCDHEIGHT -15 || p.x < 15 || p.y > LCDWIDTH -15 || p.y <15){
		LED_ON;
		game_state = ENDING;
    }else{
        if(fuel_cell_active==1){
            int xDist = abs(fuelCell.x - p.x);
            int yDist = abs(fuelCell.y - p.y);
            if(yDist < 14 && xDist < 14){
                fuel_cell_active=0;
                draw_fuel_cell_timebar(0);
                remove_fuel_cell(fuelCell);
                cells_collected++;
            }
        }
        int i;
        Point a;
        for(i = 0; i < MAX_ROIDS; i++){
			if( astroids[i].alive == 1)
			{
			    a = astroids[i].loc;
				int xDist = abs(a.x - p.x);
				int yDist = abs(a.y - p.y);
				if(yDist < 14 && xDist < 14){
					game_state = ENDING;
					LED_OFF;
					break;
				}
			}
		}
	}
		return state;
}


void create_asteroid(Asteroid *a){
	int axis = rand_num(3);
	if(axis==0){
		(*a).loc.x = 0;
		(*a).loc.y = rand_num(LCDHEIGHT);
		(*a).vel.x = rand_num(max_roid_speed-1)+1;
		(*a).vel.y = rand_num(max_roid_speed*2)-max_roid_speed;
	}else if(axis==1){
		(*a).loc.x = rand_num(LCDHEIGHT);
		(*a).loc.y = 0;
		(*a).vel.y = rand_num(max_roid_speed*2)-max_roid_speed;
		(*a).vel.y = rand_num(max_roid_speed-1)+1;
	}else if(axis==2){
		(*a).loc.x = rand_num(LCDHEIGHT);
		(*a).loc.y = LCDWIDTH;
		(*a).vel.x = rand_num(max_roid_speed*2)-max_roid_speed;
		(*a).vel.y = -rand_num(max_roid_speed-1)-1;
	}else{
		(*a).loc.x = LCDHEIGHT;
		(*a).loc.y = rand_num(LCDHEIGHT);
		(*a).vel.x = -rand_num(max_roid_speed-1)-1;
		(*a).vel.y = rand_num(max_roid_speed*2)-max_roid_speed;
	}
	(*a).alive = 1;
	(*a).rad = (int32_t) rand_num(max_roid_radius)+2;
}

void debug_point(Point p){
	char buffer[25];
	itoa(p.x, buffer, 10);
	display_string("Point: (");
	display_string(buffer);
	display_string(", ");
	itoa(p.y, buffer, 10);
	display_string(buffer);
	display_string(" )\n");
}

uint16_t rand_num(uint16_t limit) {
/* return a random number between 0 and limit inclusive.
 */

    uint16_t divisor = RAND_MAX/(limit+1);
    uint16_t retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}