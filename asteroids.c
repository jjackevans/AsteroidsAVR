#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "os.h"
#include "graphics.h"

#define MAX_ASTEROIDS 15
#define PLAYING 1
#define ENDING 2

/*
 * TODO: Add start interface
 * TODO: Add end game interface - explosion?
 * TODO: Add defined "rounds" of the game
 * TODO: Add "stages" - consisting of rounds, where the difficulty increases at each stage.
 *                      ship colour and speed will also increase.
 * TODO: Add game-completion and supporting interfaces.
 * TODO: Add new ships that can be unlocked with different game-modes.
 */

void init(void);
int fuel_cell(int state);
int increase_difficulty(int);
int collect_delta(int);
int refresh_screen(int);
void update_ship(void);
int update_asteroids(int);
int check_pos(int);
uint16_t rand_num(uint16_t limit) ;
void create_asteroid(Asteroid *a);

Asteroid asteroids[MAX_ASTEROIDS];
int astroid_count;
int game_state = 0;  
int encoder_position   = 0;
int asteroid_state=0;
int cells_collected=0;
Point p;
Point fuel_cell_position;
int fuel_cell_active;

uint8_t max_asteroid_speed;
uint8_t current_asteroids;
uint8_t max_asteroid_radius;
uint8_t ship_speed;

void main(void) {
	init();
    os_init();
	init_graphics();

    os_add_task( collect_delta,     60, 1 );
    os_add_task( update_asteroids,  6, 0 );
    os_add_task( check_pos,        120, 0 );
    os_add_task( increase_difficulty, 10000, 0);
    os_add_task( fuel_cell, 960,0);

    sei();;
    for(;;){}

}

void init(void){
	srand(time(NULL));
	p.x = LCDHEIGHT/2;
	p.y = LCDWIDTH/2;
	astroid_count=0;
	ship_speed=3;
	max_asteroid_speed = 2;
    current_asteroids = 3;
	max_asteroid_radius = 2;
	game_state = PLAYING;
	
}

int fuel_cell(int state){
    if(game_state != PLAYING){ return 0; };
    if(state==10){
        fuel_cell_position.x = rand_num(LCDHEIGHT-60)+30;
        fuel_cell_position.y = rand_num(LCDWIDTH-50)+25;
        draw_fuel_cell(fuel_cell_position);
        draw_fuel_cell_timebar(state);
        fuel_cell_active=1;
    }
    if(state<10 && fuel_cell_active){
        draw_fuel_cell(fuel_cell_position);
        if(state==0){
            game_state = ENDING;
        }
        draw_fuel_cell_timebar(state);
    }
    return state > 0 ? state-1 : 14;
}

int increase_difficulty(int state){
    if(state %2 && current_asteroids <= MAX_ASTEROIDS){
        current_asteroids++;
    }
    if(state % 3==0){
        max_asteroid_speed++;
    }
    if(state % 7==0){
        max_asteroid_radius++;
    }
    if(state % 5==0){
        ship_speed++;
    }
    return state+1;
}

int collect_delta(int state) {
	int v = os_enc_delta();
	encoder_position += v;
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
	move_ship(p, encoder_position*10, &n, ship_speed);
	p=n;
	draw_points(cells_collected);
}

int update_asteroids(int state) {
    if (game_state != PLAYING) { return state; };
    if (state < current_asteroids) {
        if (asteroids[state].alive == 1) {
            move_asteroid(&asteroids[state]);
        } else {
            create_asteroid(&asteroids[state]);
        }
    }
	return state < MAX_ASTEROIDS ? state+1 : 0 ;
}

int check_pos(int state){
	if(game_state != PLAYING){ return state; };
	if(p.x > LCDHEIGHT -15 || p.x < 15 || p.y > LCDWIDTH -15 || p.y <15){
		game_state = ENDING;
    }else{
        if(fuel_cell_active==1){
            int xDist = abs(fuel_cell_position.x - p.x);
            int yDist = abs(fuel_cell_position.y - p.y);
            if(yDist < 14 && xDist < 14){
                fuel_cell_active=0;
                draw_fuel_cell_timebar(0);
                remove_fuel_cell(fuel_cell_position);
                cells_collected++;
            }
        }
        int i;
        Point a;
        for(i = 0; i < MAX_ASTEROIDS; i++){
			if( asteroids[i].alive == 1)
			{
			    a = asteroids[i].loc;
				int xDist = abs(a.x - p.x);
				int yDist = abs(a.y - p.y);
				if(yDist < 11 && xDist < 11){
					game_state = ENDING;
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
		(*a).loc.y = rand_num(LCDWIDTH);
		(*a).vel.x = rand_num(max_asteroid_speed-1)+1;
		(*a).vel.y = rand_num(max_asteroid_speed*2)-max_asteroid_speed;
	}else if(axis==1){
		(*a).loc.x = rand_num(LCDHEIGHT);
		(*a).loc.y = 0;
		(*a).vel.y = rand_num(max_asteroid_speed*2)-max_asteroid_speed;
		(*a).vel.y = rand_num(max_asteroid_speed-1)+1;
	}else if(axis==2){
		(*a).loc.x = rand_num(LCDHEIGHT);
		(*a).loc.y = LCDWIDTH;
		(*a).vel.x = rand_num(max_asteroid_speed*2)-max_asteroid_speed;
		(*a).vel.y = -rand_num(max_asteroid_speed-1)-1;
	}else{
		(*a).loc.x = LCDHEIGHT;
		(*a).loc.y = rand_num(LCDWIDTH);
		(*a).vel.x = -rand_num(max_asteroid_speed-1)-1;
		(*a).vel.y = rand_num(max_asteroid_speed*2)-max_asteroid_speed;
	}
	(*a).alive = 1;
	(*a).rad = (int32_t) rand_num(max_asteroid_radius)+2;
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

//void debug_point(Point p){
//    char buffer[25];
//    itoa(p.x, buffer, 10);
//    display_string("Point: (");
//    display_string(buffer);
//    display_string(", ");
//    itoa(p.y, buffer, 10);
//    display_string(buffer);
//    display_string(" )\n");
//}