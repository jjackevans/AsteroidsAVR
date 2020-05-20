#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "graphics.h"

#define MAX_ASTEROIDS 15

/*
 * TODO: Add start interface
 * TODO: Add end game interface - explosion?
 * TODO: Add defined "rounds" of the game
 * TODO: Add "stages" - consisting of rounds, where the difficulty increases at each stage.
 *                      ship colour and speed will also increase.
 * TODO: Add game-completion and supporting interfaces.
 * TODO: Add new ships that can be unlocked with different game-modes.
 * TODO: Use ROM to make each game different
 */

void init(void);
int end_game(int state);
int difficulty_select(int state);
int fuel_cell(int state);
int increase_difficulty(int);
int collect_delta(int);
int refresh_screen(int);
void update_ship(void);
int update_asteroids(int);
int check_pos(int);
void start_game(int state);
uint16_t rand_num(uint16_t limit) ;
void create_asteroid(Asteroid *a);
int write_highscore(uint16_t score);
void increment_randomseed(void);
uint32_t get_randomseed(void) ;
uint16_t get_highscore(void);

Asteroid asteroids[MAX_ASTEROIDS];
int encoder_position = 0;
int asteroid_state=0;
int cells_collected=0;
Point p;
Point fuel_cell_position;
int fuel_cell_active;
int difficulty_factor;
FIL File;

uint8_t max_asteroid_speed;
uint8_t current_asteroids;
uint8_t max_asteroid_radius;
uint8_t ship_speed;
uint8_t base_speed;
uint8_t msElapsed;

void main(void) {
    os_init();
	init_graphics();
    _delay_ms(3000);
    init();
    sei();;
    for(;;){
    }
}

int decrease_speed(int state){
    if(ship_speed<=base_speed){
        ship_speed=base_speed;
    }else{
        ship_speed--;
    }
}
int increase_speed(int state){
    if(ship_speed>base_speed+6){
        ship_speed=base_speed+6;
    }else{
        ship_speed++;
    }
}

void init(void){
    srand(0);
    os_add_task(difficulty_select,100,0);
}

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

int refresh_screen(int state){
    clear_screen();
    return state;
}

void update_ship(void) {
	Point n;
	move_ship(p, encoder_position*10, &n, ship_speed);
	p=n;
	draw_points(cells_collected);
}

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

int check_pos(int state){
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

int end_game(int state){
    if(state==25) {
        os_reinit();
        os_add_task(end_game,100,0);
        clear_screen();
        if(cells_collected> get_highscore()) {
            if(write_highscore(cells_collected)>0) {
                char scoreStr[20]= "NEW HIGHSCORE: ";
                char buffer[5];
                itoa(cells_collected, buffer, 10);
                char *result = malloc(strlen(buffer) + strlen(scoreStr) + 1);
                strcpy(result, scoreStr);
                strcat(result, buffer);
                int len = strlen(result);
                display_string_xy(result,LCDHEIGHT/2-len*3, LCDWIDTH/2 +4);
            }
        }else{
            char scoreStr[10] = "Score: ";
            char buffer[5];
            itoa(cells_collected, buffer, 10);
            char *result = malloc(strlen(buffer) + strlen(scoreStr) + 1);
            strcpy(result, scoreStr);
            strcat(result, buffer);
            int len = strlen(result);
            display_string_xy(result,LCDHEIGHT/2-len*3, LCDWIDTH/2 +4);
        }
        display_string_xy("Game Over", LCDHEIGHT/2 -(9*3) , LCDWIDTH/2 -12);
        int i = MAX_ASTEROIDS-1;
        while(i>=0){
            asteroids[i].alive=0;
            i--;
        }
    }
    if(state==55){
        char newGameMsg[] = "Press the centre button to start a new game.";
        display_string_xy(newGameMsg, LCDHEIGHT/2 -(strlen(newGameMsg)*3) , LCDWIDTH/2 +20);
    }
    if(state>55){
        if(get_switch_press(_BV(SWC))){ os_reinit(); difficulty_select(0); os_add_task(difficulty_select, 100,0);}
    }
    return state+1;
}

int difficulty_select(int state){
    if(state==0) {
        os_add_task( collect_delta,     60,0 );
        clear_screen();
        uint16_t highscore = get_highscore();
        if(highscore==0){
            char noHighscoreStr[] = "No highscore found";
            display_string_xy(noHighscoreStr, LCDHEIGHT / 2 - (3 * strlen(noHighscoreStr)), LCDWIDTH / 2 - 21);
        }else{
            char buffer[20] = "Highscore: ";
            itoa(highscore, &buffer[11], 10);
            display_string_xy(buffer, LCDHEIGHT / 2 - (3 * strlen(buffer)), LCDWIDTH / 2 - 21);
        }
        char s1[] = "Please select a difficulty";
//        char s3[] = "and less time to get the fuel cell.";
//        char s4[] = "It also means you gain points faster.";
        char s5[] = "  Normal         Difficult       Impossible";
        display_string_xy(s1, LCDHEIGHT / 2 - (3 * strlen(s1)), LCDWIDTH / 2 - 34);
//        display_string_xy(s3, LCDHEIGHT / 2 - (3 * strlen(s3)), LCDWIDTH / 2 - 13);
//        display_string_xy(s4, LCDHEIGHT / 2 - (3 * strlen(s4)), LCDWIDTH / 2);
        display_string_xy(s5, LCDHEIGHT / 2 - (3 * strlen(s5)), LCDWIDTH / 2 + 32);
    }
    if(get_switch_press(_BV(SWC))){ start_game(state-1); return 1;}
    if(get_switch_press(_BV(SWE))) state++;
    if(get_switch_press(_BV(SWW))) state--;
    if(state<1){state=1;}
    if(state>3){state=3;}
    highlight_difficulty(state);
    return state;
}

void start_game(int difficulty){
    p = highlight_difficulty(difficulty+1);
    difficulty_factor=difficulty;
    clear_screen();
    os_reinit();
    encoder_position=0;
    fuel_cell_active=0;
    base_speed=3;
    ship_speed=3;
    cells_collected = 0;
    max_asteroid_speed = 2+difficulty_factor;
    current_asteroids = 3+difficulty_factor;
    max_asteroid_radius = 2+difficulty_factor;
    os_add_task( increase_difficulty, 10000 + (-150 * difficulty_factor), 0);
    os_add_task( collect_delta,     80 + (-10 * difficulty_factor), 1 );
    os_add_task( update_asteroids,  10, 0 );
    os_add_task( increase_speed , 200  + (-20 * difficulty_factor), 0);
    os_add_task( check_pos,        120, 0 );
    os_add_task( fuel_cell, 800,0);
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

uint32_t get_randomseed(void){
    return -1;
}

void increment_randomseed(void){

}

//    char buffer[25];
//    itoa(p.x, buffer, 10);
//    display_string("Point: (");
//    display_string(buffer);
//    display_string(", ");
//    itoa(p.y, buffer, 10);
//    display_string(buffer);
//    display_string(" )\n");
//}