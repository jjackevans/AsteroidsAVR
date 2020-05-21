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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "graphics.h"

#define MAX_ASTEROIDS 15

void init(void);
int end_game(int);
int difficulty_select(int);
int fuel_cell(int);
int increase_difficulty(int);
int collect_delta(int);
void update_ship(void);
int update_asteroids(int);
int check_collisions(int);
void start_game(int);
uint16_t rand_num(uint16_t) ;
void create_asteroid(Asteroid);
int write_highscore(uint16_t);
uint16_t get_highscore(void);

// Location of ship on screen
Point p;

// 0 = Easy, 1 = Difficult, 2 = Impossible
int difficulty_factor;
int encoder_position;

// Cells collected / Points scored
int cells_collected;
Point fuel_cell_position;
int fuel_cell_active;

// File containing highscore
FIL File;

// Asteroid data
Asteroid asteroids[MAX_ASTEROIDS];
uint8_t max_asteroid_speed;
uint8_t current_asteroids;
uint8_t asteroid_radius;
uint8_t ship_speed;
uint8_t base_speed;

/**
* Invokes initialisation.
*/
void main(void) {
    os_init();
	init_graphics();
    _delay_ms(3000);
    init();
    sei();;
    for(;;){}
}

/**
* Invokes difficult selection task.
*/
void init(void){
    srand(0);
    os_add_task(difficulty_select,100,0);
}

/**
* Prompts the user to select between different gamemodes.
*/
int difficulty_select(int state){
    if(state==0) {
        os_add_task( collect_delta,     60,0 );
        clear_screen();
        uint16_t highscore = get_highscore();
        if(highscore==0){
            char noHighscoreStr[] = "No highscore found";
            draw_centered_text(noHighscoreStr,- 21);
        }else{
            char buffer[20] = "Highscore: ";
            itoa(highscore, &buffer[11], 10);
            draw_centered_text(buffer,  - 21);
        }
        char s1[] = "Please select a difficulty";
        char s2[] = "  Normal         Difficult       Impossible";
        draw_centered_text(s1,-34);
        draw_centered_text(s2, 32);
    }
    if(get_switch_press(_BV(SWC))){ start_game(state-1); return 1;}
    if(get_switch_press(_BV(SWE))) state++;
    if(get_switch_press(_BV(SWW))) state--;
    if(state<1){state=1;}
    if(state>3){state=3;}
    highlight_difficulty(state);
    return state;
}

/**
* Invokes in-game processes and re-initialises variables.
*/
void start_game(int difficulty){
    p = highlight_difficulty( difficulty + 1 );
    difficulty_factor=difficulty;
    clear_screen();
    os_reinit();
    encoder_position   = 0;
    fuel_cell_active   = 0;
    base_speed         = 3;
    ship_speed         = 3;
    cells_collected    = 0;
    max_asteroid_speed = 2 + difficulty_factor;
    current_asteroids  = 3 + difficulty_factor;
    asteroid_radius    = 2 + difficulty_factor;
    os_add_task( increase_speed ,     200 - 20 * difficulty_factor,    0);
    os_add_task( increase_difficulty, 10000 - 150 * difficulty_factor, 0);
    os_add_task( collect_delta,       80 - 10 * difficulty_factor,     1);
    os_add_task( update_asteroids,    10,  0 );
    os_add_task( check_collisions,           120, 0 );
    os_add_task( fuel_cell,           800, 0);
}

/**
* Stops the game processes and resets asteroids.
*/
int end_game(int state){
    if(state==25) {
        os_reinit();
        os_add_task(end_game,100,0);
        int i = MAX_ASTEROIDS-1;
        while(i>=0){
            asteroids[i].alive=0;
            i--;
        }
        clear_screen();
        draw_centered_text("Game Over", -12);
        if(cells_collected> get_highscore()) {
            if(write_highscore(cells_collected)>0) {
                char scoreStr[20]= "NEW HIGHSCORE: ";
                char buffer[5];
                itoa(cells_collected, buffer, 10);
                char *result = malloc(strlen(buffer) + strlen(scoreStr) + 1);
                strcpy(result, scoreStr);
                strcat(result, buffer);
                draw_centered_text(&result,4);
            }
        }else{
            char scoreStr[10] = "Score: ";
            char buffer[5];
            itoa(cells_collected, buffer, 10);
            char *result = malloc(strlen(buffer) + strlen(scoreStr) + 1);
            strcpy(result, scoreStr);
            strcat(result, buffer);
            int len = strlen(result);
            draw_centered_text(&result,4);
        }
    }else if(state==55){
        char newGameMsg[] = "Press the centre button to start a new game.";
        draw_centered_text(newGameMsg,20);
    }else if(state>55){
        if(get_switch_press(_BV(SWC))){
         os_reinit();
         difficulty_select(0);
         os_add_task(difficulty_select, 100,0);
         }
    }
    return state+1;
}

/**
* Collects the rotary encoder changes and updates the ship
* with the new angle.
*/
int collect_delta(int state) {
	int v = os_enc_delta();
	if(v!=0){
	    decrease_speed(0);
	}
	encoder_position += v;
	if(state==1) {
        update_ship();
    }
	return state;
}

/**
* Decreases the speed of the ship.
*/
void decrease_speed(void){
    if(ship_speed<=base_speed){
        ship_speed=base_speed;
    }else{
        ship_speed--;
    }
}

/**
* Increases the speed of the ship.
*/
int increase_speed(int state){
    if(ship_speed>base_speed+6){
        ship_speed=base_speed+6;
    }else{
        ship_speed++;
    }
    return state;
}

/**
* Updates location and rotation of the ship based
* on the rotary encoder position.
*/
void update_ship(void) {
	Point n;
	move_ship(p, encoder_position*10, &n, ship_speed);
	p=n;
	draw_points(cells_collected);
}

/**
* Moves all of the asteroids using their velocities.
*/
int update_asteroids(int state) {
    if (state < current_asteroids) {
        if (asteroids[state].alive == 1) {
            move_asteroid(&asteroids[state]);
        } else {
            create_asteroid(&asteroids[state]);
        }
    }
	return state < MAX_ASTEROIDS ? state+1 : 0 ;
}

/**
* Checks for collisions between the objects.
* May end the game or de-activate the fuel cell.
*/
int check_collisions(int state){
	if(p.x > LCDHEIGHT -15 || p.x < 15 || p.y > LCDWIDTH -15 || p.y <15){
        os_reinit();
        os_add_task(end_game,100,0);
    }else{
        if(fuel_cell_active==1){
            int xDist = abs(fuel_cell_position.x - p.x);
            int yDist = abs(fuel_cell_position.y - p.y);
            if(yDist < 14 && xDist < 14){
                fuel_cell_active=0;
                draw_fuel_cell_timebar(0);
                remove_fuel_cell(fuel_cell_position);
                cells_collected=cells_collected+difficulty_factor+1;
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
                    os_reinit();
                    os_add_task(end_game,100,0);
					break;
				}
			}
		}
	}
		return state;
}

/**
* Creates a new asteroid object with random velocity and location.
*/
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
	(*a).rad = (int32_t) rand_num(asteroid_radius)+2;
}

/**
* Creates a fuel cell that must be obtained within a timeframe
* or the game will end.
*/
int fuel_cell(int state){
    if(state==(10-difficulty_factor)){
        fuel_cell_position.x = rand_num(LCDHEIGHT-60)+30;
        fuel_cell_position.y = rand_num(LCDWIDTH-50)+25;
        draw_fuel_cell(fuel_cell_position);
        draw_fuel_cell_timebar(state);
        fuel_cell_active=1;
    }
    if(state<(10-difficulty_factor) && fuel_cell_active){
        draw_fuel_cell(fuel_cell_position);
        if(state==0){
            os_reinit();
            os_add_task(end_game,100,0);
        }
        draw_fuel_cell_timebar(state);
    }
    return state > 0 ? state-1 : (14-difficulty_factor);
}

/**
* Increments the amount of asteroids, their speed and radius.
*/
int increase_difficulty(int state){
    if(state %2 && current_asteroids <= MAX_ASTEROIDS){
        current_asteroids++;
    }
    if(state % 3==0){
        max_asteroid_speed++;
    }
    if(state % 7==0){
        asteroid_radius++;
    }
    if(state % 5==0){
        ship_speed++;
    }
    return state+1;
}

/**
* Returns a random number between 0 and the limit (inclusive).
*/
uint16_t rand_num(uint16_t limit) {
    uint16_t divisor = RAND_MAX/(limit+1);
    uint16_t retval;
    do { 
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

/**
* Writes the new highscore to the file.
*/
int write_highscore(uint16_t score){
    int written=0;
    if (get_switch_long(_BV(OS_CD))) {
        f_mount(&FatFs, "", 0);
        if (f_open(&File, "dataastr.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
            f_write(&File, &score , 2, &written);
            f_close(&File);
            return 2;
        }
    }
    return 0;
}

/**
* Attempts to get the highscore from a file.
*/
uint16_t get_highscore(void){
    uint16_t highscore=0;
    UINT ignore;
    f_mount(&FatFs, "", 0);
    if (f_open(&File, "dataastr.txt", FA_READ) == FR_OK) {
        f_read(&File, &highscore, 2,&ignore);
        f_close(&File);
    }else{
        return 0;
    }
    return highscore;
}
