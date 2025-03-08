#include "pong.h"
#include <stdio.h>
#include <stdlib.h>


void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));
}

void set_left_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->left_pad), position);
}

void set_left_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->left_pad), speed);
}

void set_right_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->right_pad), position);
}

void set_right_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->right_pad), speed);
}

int db1=0, db2=0;
void bounce_ball(Pong* pong)
{
	
	//bal oldal visszapattanás
	
	if (pong->left_pad.y <= pong->ball.y && pong->ball.y <= pong->left_pad.y + pong->left_pad.height){

		if (pong->ball.x - pong->ball.radius < 50){
				
			pong->ball.x = pong->ball.radius + 50;
			pong->ball.speed_x *= -1;
				
		}
	}
	else if(pong->ball.x - pong->ball.radius < 0){
			
		pong->ball.x = pong->ball.radius;
		pong->ball.speed_x *= -1;
		db1++;
			
	}	
	
	//jobb oldali visszapattanás
	
	if (pong->right_pad.y <= pong->ball.y && pong->ball.y <= pong->right_pad.y + pong->right_pad.height){
				
		if (pong->ball.x + pong->ball.radius >= pong->width - pong->right_pad.width){
					
			pong->ball.x = pong->width - pong->ball.radius - 50;
			pong->ball.speed_x *= -1;
				
		}
			
	}
	else if(pong->ball.x + pong->ball.radius > pong->width){
			
		pong->ball.x = pong->width - pong->ball.radius;
		pong->ball.speed_x *= -1;
		db2++;
			
	}	

	//alul felül visszapattanás

    if (pong->ball.y - pong->ball.radius < 0) {
        pong->ball.y = pong->ball.radius;
        pong->ball.speed_y *= -1;
    }
    if (pong->ball.y + pong->ball.radius > pong->height) {
        pong->ball.y = pong->height - pong->ball.radius;
        pong->ball.speed_y *= -1;
    }
	
}