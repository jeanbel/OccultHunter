/*********************************************************************
How to compile this on different platforms:

gcc Main.c DrawUtils.c `pkg-config --cflags --libs sdl2 gl glew` -o CFramework
*/
#include "stdafx.h"
#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"DrawUtils.h"
#include<stdlib.h>
#include<time.h>

//master settings for width and height
int windowWidth = 640; //640
int windowHeight = 480; //480

//initialize position
double xPos = 0;
double yPos = 0;
double charX = 0;
double charY = 0;
double camX = 0;
double camY = 0;

//self-explanatory
double CHARSPEED = 0.5;
double CAMSPEED = 0.5;
int redraw = 0;
int shoot = 0;
int enemyShoot = 0;
int charDir = 0;
int enemyDir = 0;
int gameTime = 0;
int lastTime = 0;
int currentTime = 0;
int killed = 0;
int bgcoll[4000][6];
int moving = 0;

//background settings
int bgxPos = 0;
int bgyPos = 0;

//number of tiles
int kappaLR = 60;
int kappaUD = 60;

//map
int map[60][60] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 5, 5, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 5, 5, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

/* Set this to true to force the game to exit */
char shouldExit = 0;

/* The previous frame's keyboard state */
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

/* The current frame's keyboard state */
const unsigned char* kbState = NULL;

/* position of the sprite */
double spritePos[2] = { 29*10, 45*10 }; //double spritePos[2] = { 29, 45 }; original starting location commented out
double enemyPos[2] = { 10, 10 };
double shotPos[2] = { 10, 10 };
double enemyShotPos[2] = { 10, 10 };

/* Texture for the sprite */
GLuint spriteTex;

/* size of the sprite */
int spriteSize[2];

typedef struct AABB {
	int x, y, w, h;
} AABB;
typedef struct Tile {
	int typeSet;
	bool collided;
	AABB box;
} Tile;
class Camera {
	int x;
	int y;
} Camera;

bool AABBIntersect(const AABB* box1, const AABB* box2)
{
	// box1 to the right
	if (box1->x > box2->x + box2->w) {
		return false;
	}
	// box1 to the left
	if (box1->x + box1->w < box2->x) {
		return false;
	}
	// box1 below
	if (box1->y > box2->y + box2->h) {
		return false;
	}
	// box1 above
	if (box1->y + box1->h < box2->y) {
		return false;
	}
	return true;
}

int main(void)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return 1;
	}

	/* Create the window, OpenGL context */
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"TestSDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowWidth, windowHeight,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	/* Make sure we have a recent version of OpenGL */
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_3_0) {
		fprintf(stderr, "OpenGL 3.0 or greater supported: Version=%s\n",
			glGetString(GL_VERSION));
	}
	else {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	/* Setup OpenGL state */

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, windowWidth, windowHeight, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Load the texture */
	spriteTex = glTexImageTGAFile("ethanleft.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyTex = glTexImageTGAFile("enemyleft.tga", &spriteSize[0], &spriteSize[1]);

	GLuint bgtex = glTexImageTGAFile("kappa.tga", &spriteSize[0], &spriteSize[1]);
	GLuint edgetex = glTexImageTGAFile("snorlax.tga", &spriteSize[0], &spriteSize[1]);

	GLuint ledgelefttex = glTexImageTGAFile("ledgeleft.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ledgerighttex = glTexImageTGAFile("ledgeright.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ledgetoptex = glTexImageTGAFile("ledgetop.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ledgebottomtex = glTexImageTGAFile("ledgebottom.tga", &spriteSize[0], &spriteSize[1]);

	GLuint ethanleft = glTexImageTGAFile("ethanleft.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanleftmoving = glTexImageTGAFile("ethanleftmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanright = glTexImageTGAFile("ethanright.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanrightmoving = glTexImageTGAFile("ethanrightmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethantop = glTexImageTGAFile("ethantop.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethantopmoving = glTexImageTGAFile("ethantopmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethantopmoving2 = glTexImageTGAFile("ethantopmoving2.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanbottom = glTexImageTGAFile("ethanbottom.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanbottommoving = glTexImageTGAFile("ethanbottommoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint ethanbottommoving2 = glTexImageTGAFile("ethanbottommoving2.tga", &spriteSize[0], &spriteSize[1]);

	GLuint enemyleft = glTexImageTGAFile("enemyleft.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyleftmoving = glTexImageTGAFile("enemyleftmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyleftmoving2 = glTexImageTGAFile("enemyleftmoving2.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyright = glTexImageTGAFile("enemyright.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyrightmoving = glTexImageTGAFile("enemyrightmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemyrightmoving2 = glTexImageTGAFile("enemyrightmoving2.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemytop = glTexImageTGAFile("enemytop.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemytopmoving = glTexImageTGAFile("enemytopmoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemytopmoving2 = glTexImageTGAFile("enemytopmoving2.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemybottom = glTexImageTGAFile("enemybottom.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemybottommoving = glTexImageTGAFile("enemybottommoving.tga", &spriteSize[0], &spriteSize[1]);
	GLuint enemybottommoving2 = glTexImageTGAFile("enemybottommoving2.tga", &spriteSize[0], &spriteSize[1]);

	GLuint projectileleft = glTexImageTGAFile("pikachuleft.tga", &spriteSize[0], &spriteSize[1]);
	GLuint projectileright = glTexImageTGAFile("pikachuright.tga", &spriteSize[0], &spriteSize[1]);
	GLuint projectileup = glTexImageTGAFile("pikachuup.tga", &spriteSize[0], &spriteSize[1]);
	GLuint projectiledown = glTexImageTGAFile("pikachudown.tga", &spriteSize[0], &spriteSize[1]);

	GLuint definesize = glTexImageTGAFile("Mega-Man-transparent.tga", &spriteSize[0], &spriteSize[1]);
	kbState = SDL_GetKeyboardState(NULL);

	/* The game loop */
	while (!shouldExit) {
		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		/* Handle OS message pump */
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				shouldExit = 1;
			}
		}

		/* Game logic */
		//game controls
		if (kbState[SDL_SCANCODE_LEFT] && moving == 0) {
			moving = 1; //if the character is moving already, invalidate other movement controls to prevent diagonal movement
			spritePos[0] -= CHARSPEED; //moves relative to character speed setting set
			if (spritePos[0] < 0) //character cannot leave a certain area
			{
				spritePos[0] = 0;
			}
			spriteTex = ethanleft; //changes texture to face proper direction

								   //oscillate between frames to simulate walking movement
			if (redraw > 100)
				spriteTex = ethanleft;
			else
				spriteTex = ethanleftmoving;

			//the numbers 100 and 200 make the frames redraw slower so it looks smoother
			redraw++;
			if (redraw > 200)
				redraw = 0;

			glMatrixMode(GL_MODELVIEW); //needed to update camera movement
			glLoadIdentity(); //loads the modelview
			xPos += CAMSPEED; //moves camera
			charX = xPos; //defines character's x position
			charY = yPos; //defines character's y position
			camX = xPos; //defines camera's x position
			camY = yPos; //defines camera's y position
			charDir = 0; //define character direction
			glTranslated(xPos, yPos, 0); //translates the camera to follow the character
		}
		if (kbState[SDL_SCANCODE_RIGHT] && moving == 0) {
			moving = 1;
			//move based on player speed
			spritePos[0] += CHARSPEED;
			//boundary
			if (spritePos[0] > spriteSize[0] * (kappaLR - 1))
				spritePos[0] = spriteSize[0] * (kappaLR - 1);

			//oscillate between frames to simulate walking movement
			if (redraw > 100)
				spriteTex = ethanright;
			else
				spriteTex = ethanrightmoving;

			//the numbers 100 and 200 make the frames redraw slower so it looks smoother
			redraw++;
			if (redraw > 200)
				redraw = 0;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			xPos -= CAMSPEED;
			charX = xPos;
			charY = yPos;
			camX = xPos;
			camY = yPos;
			charDir = 1;
			glTranslated(xPos, yPos, 0);
		}
		if (kbState[SDL_SCANCODE_UP] && moving == 0) {
			moving = 1;
			spritePos[1] -= CHARSPEED;
			if (spritePos[1] < 0)
				spritePos[1] = 0;

			//oscillate between frames to simulate walking movement
			if (redraw > 300)
				spriteTex = ethantop;
			else if (redraw > 200)
				spriteTex = ethantopmoving;
			else if (redraw > 100)
				spriteTex = ethantop;
			else
				spriteTex = ethantopmoving2;

			//the numbers 100 and 200 make the frames redraw slower so it looks smoother
			redraw++;
			if (redraw > 400)
				redraw = 0;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			yPos += CAMSPEED;
			charX = xPos;
			charY = yPos;
			camX = xPos;
			camY = yPos;
			charDir = 2;
			glTranslated(xPos, yPos, 0);
		}
		if (kbState[SDL_SCANCODE_DOWN] && moving == 0) {
			moving = 1;
			spritePos[1] += CHARSPEED;
			if (spritePos[1] > spriteSize[1] * (kappaUD - 1))
				spritePos[1] = spriteSize[1] * (kappaUD - 1);

			//oscillate between frames to simulate walking movement
			if (redraw > 300)
				spriteTex = ethanbottom;
			else if (redraw > 200)
				spriteTex = ethanbottommoving;
			else if (redraw > 100)
				spriteTex = ethanbottom;
			else
				spriteTex = ethanbottommoving2;

			//the numbers 100 and 200 make the frames redraw slower so it looks smoother
			redraw++;
			if (redraw > 400)
				redraw = 0;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			yPos -= CAMSPEED;
			charX = xPos;
			charY = yPos;
			camX = xPos;
			camY = yPos;
			charDir = 3;
			glTranslated(xPos, yPos, 0);
		}

		//camera controls
		if (kbState[SDL_SCANCODE_A]) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			xPos += CAMSPEED;
			camX = xPos;
			camY = yPos;
			glTranslated(xPos, yPos, 0);
		}
		if (kbState[SDL_SCANCODE_D]) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			xPos -= CAMSPEED;
			camX = xPos;
			camY = yPos;
			glTranslated(xPos, yPos, 0);
		}
		if (kbState[SDL_SCANCODE_W]) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			yPos += CAMSPEED;
			camX = xPos;
			camY = yPos;
			glTranslated(xPos, yPos, 0);
		}
		if (kbState[SDL_SCANCODE_S]) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			yPos -= CAMSPEED;
			camX = xPos;
			camY = yPos;
			glTranslated(xPos, yPos, 0);
		}
		//camera finds the player
		if (kbState[SDL_SCANCODE_C]) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			xPos = charX;
			yPos = charY;
			camX = xPos;
			camY = yPos;
			glTranslated(xPos, yPos, 0);
		}
		//shoot a projectile
		if (kbState[SDL_SCANCODE_SPACE]) {
			shoot = 1;
			//shotPos[0] = spritePos[0] - spriteSize[0];
			//shotPos[1] = spritePos[1];

			if (charDir == 0) {
				shotPos[0] = spritePos[0] - spriteSize[0];
				shotPos[1] = spritePos[1];
			}
			else if (charDir == 1) {
				shotPos[0] = spritePos[0] + spriteSize[0];
				shotPos[1] = spritePos[1];
			}
			else if (charDir == 2) {
				shotPos[0] = spritePos[0];
				shotPos[1] = spritePos[1] - spriteSize[1];
			}
			else {
				shotPos[0] = spritePos[0];
				shotPos[1] = spritePos[1] + spriteSize[1];;
			}
		}

		//reset the moving variable to allow movement
		moving = 0;

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw tiles : draw only the stuff in the camera's view
		for (int i = 0; i < kappaUD; i++) {
			for (int j = 0; j < kappaLR; j++) {
				{
					/*if (camX > spriteSize[0] * i - windowWidth)
					{
					glDrawSprite(bgtex, bgxPos + i*spriteSize[0], bgyPos + j*spriteSize[1], spriteSize[0], spriteSize[1]);
					}*/

					//draws background tiles only if the character is on the screen
					if (spritePos[0] > spriteSize[0] * j - windowWidth && spritePos[0] < spriteSize[0] * j + windowWidth
						&& spritePos[1] > spriteSize[1] * i - windowHeight && spritePos[1] < spriteSize[1] * i + windowHeight)
					{
						if (map[i][j] == 0) {
							glDrawSprite(bgtex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
						else if (map[i][j] == 1) {
							glDrawSprite(edgetex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
						else if (map[i][j] == 2) {
							glDrawSprite(ledgelefttex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
						else if (map[i][j] == 3) {
							glDrawSprite(ledgerighttex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
						else if (map[i][j] == 4) {
							glDrawSprite(ledgetoptex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
						else if (map[i][j] == 5) {
							glDrawSprite(ledgebottomtex, bgxPos + j*spriteSize[0], bgyPos + i*spriteSize[1], spriteSize[0], spriteSize[1]);
						}
					}
				}
			}
		}

		//defines the coordinates for every tile
		int tilect = 0;
		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 60; j++) {
				if (map[i][j] == 0) {
					bgcoll[tilect][0] = 0;
					bgcoll[tilect][1] = 0;
					bgcoll[tilect][2] = 0;
					bgcoll[tilect][3] = 0;
					bgcoll[tilect][4] = 0;
				}
				else {
					bgcoll[tilect][0] = spriteSize[0] * j; //left border
					bgcoll[tilect][1] = spriteSize[0] * j + spriteSize[0]; //right border
					bgcoll[tilect][2] = spriteSize[1] * i; //top border
					bgcoll[tilect][3] = spriteSize[1] * i + spriteSize[1]; //bottom border
					bgcoll[tilect][4] = map[i][j];
				}
				tilect++;
			}
		}

		/*//tester for seeing if the coordinate array of tiles is working
		for (int i = 0; i < 70; i++) {
		for (int j = 0; j < 4; j++) {
		printf("%d ", bgcoll[i][j]);
		}
		printf("\n");
		}*/

		//player character collision detection with background
		for (int i = 0; i < tilect; i++) {
			if (spritePos[0] > bgcoll[i][0] - spriteSize[0]
				&& spritePos[0] < bgcoll[i][1]
				&& spritePos[1] > bgcoll[i][2] - spriteSize[1]
				&& spritePos[1] < bgcoll[i][3]) {
				//printf("collision detected");
				if (bgcoll[i][4] == 1) {
					/*if (spritePos[0] < bgcoll[i][1] && charDir == 0) {
						spritePos[0] = bgcoll[i][1];
					}
					if (spritePos[0] > bgcoll[i][0] - spriteSize[0] && charDir == 1) {
						spritePos[0] = bgcoll[i][0] - spriteSize[0];
					}
					if (spritePos[1] < bgcoll[i][3] && charDir == 2) {
						spritePos[1] = bgcoll[i][3];
					}
					if (spritePos[1] > bgcoll[i][2] - spriteSize[1] && charDir == 3) {
						spritePos[1] = bgcoll[i][2] - spriteSize[1];
					}*/
				}
				else if (bgcoll[i][4] == 2) {
					/*if (spritePos[0] > bgcoll[i][0] - spriteSize[0] && charDir == 1) {
						spritePos[0] = bgcoll[i][0] - spriteSize[0];
					}
					if (spritePos[1] < bgcoll[i][3] && charDir == 2) {
						spritePos[1] = bgcoll[i][3];
					}
					if (spritePos[1] > bgcoll[i][2] - spriteSize[1] && charDir == 3) {
						spritePos[1] = bgcoll[i][2] - spriteSize[1];
					}*/
				}
				else if (bgcoll[i][4] == 3) {
					/*if (spritePos[0] < bgcoll[i][1] && charDir == 0) {
						spritePos[0] = bgcoll[i][1];
					}
					if (spritePos[1] < bgcoll[i][3] && charDir == 2) {
						spritePos[1] = bgcoll[i][3];
					}
					if (spritePos[1] > bgcoll[i][2] - spriteSize[1] && charDir == 3) {
						spritePos[1] = bgcoll[i][2] - spriteSize[1];
					}*/
				}
				else if (bgcoll[i][4] == 4) {
					/*if (spritePos[0] < bgcoll[i][1] && charDir == 0) {
						spritePos[0] = bgcoll[i][1];
					}
					if (spritePos[0] > bgcoll[i][0] - spriteSize[0] && charDir == 1) {
						spritePos[0] = bgcoll[i][0] - spriteSize[0];
					}
					if (spritePos[1] > bgcoll[i][2] - spriteSize[1] && charDir == 3) {
						spritePos[1] = bgcoll[i][2] - spriteSize[1];
					}*/
				}
				else if (bgcoll[i][4] == 5) {
					/*if (spritePos[0] < bgcoll[i][1] && charDir == 0) {
						spritePos[0] = bgcoll[i][1];
					}
					if (spritePos[0] > bgcoll[i][0] - spriteSize[0] && charDir == 1) {
						spritePos[0] = bgcoll[i][0] - spriteSize[0];
					}
					if (spritePos[1] < bgcoll[i][3] && charDir == 2) {
						spritePos[1] = bgcoll[i][3];
					}*/
				}
			}
		}

		//shoot projectile
		if (shoot == 1) {
			if (charDir == 0) {
				shotPos[0] -= 1;
				glDrawSprite(projectileleft, shotPos[0], shotPos[1], spriteSize[0], spriteSize[1]);
			}
			else if (charDir == 1) {
				shotPos[0] += 1;
				glDrawSprite(projectileright, shotPos[0], shotPos[1], spriteSize[0], spriteSize[1]);
			}
			else if (charDir == 2) {
				shotPos[1] -= 1;
				glDrawSprite(projectileup, shotPos[0], shotPos[1], spriteSize[0], spriteSize[1]);
			}
			else {
				shotPos[1] += 1;
				glDrawSprite(projectiledown, shotPos[0], shotPos[1], spriteSize[0], spriteSize[1]);
			}

			//shoot = 0;
		}

		//projectile collision detection with background
		for (int i = 0; i < tilect; i++) {
			if (shotPos[0] > bgcoll[i][0] - spriteSize[0]
				&& shotPos[0] < bgcoll[i][1]
				&& shotPos[1] > bgcoll[i][2] - spriteSize[1]
				&& shotPos[1] < bgcoll[i][3]) {
				shoot = 0;
			}
		}

		//enemy starting position
		currentTime = SDL_GetTicks();
		if ((currentTime % 1000) == 0) {
			if (enemyTex == enemyright) {
				enemyTex = enemyleft;
				enemyDir = 0;
			}
			else {
				enemyTex = enemyright;
				enemyDir = 1;
			}
		}
		else {
			enemyPos[0] = spriteSize[0] * 11;
			enemyPos[1] = spriteSize[1] * 5;

			if (enemyTex == enemyright && killed != 1) {
				glDrawSprite(enemyTex, enemyPos[0], enemyPos[1], spriteSize[0], spriteSize[1]);
			}
			else {
				if (killed != 1) {
					enemyPos[0] = spriteSize[0] * 10;
					glDrawSprite(enemyTex, enemyPos[0], enemyPos[1], spriteSize[0], spriteSize[1]);
				}
			}
		}

		//enemy shoots projectile condition
		srand(time(NULL));
		int chooseBehavior = 0;
		chooseBehavior = rand() % 5;

		if (chooseBehavior == 1 && killed == 0) {
			enemyShoot = 1;
			if (enemyDir == 0) {
				enemyShotPos[0] = enemyPos[0] - spriteSize[0];
				enemyShotPos[1] = enemyPos[1];
			}
			else {
				enemyShotPos[0] = enemyPos[0] + spriteSize[0];
				enemyShotPos[1] = enemyPos[1];
			}
		}
		else {
			//enemyShoot = 0;
		}

		//enemy shoots projectile
		if (enemyShoot == 1 && killed == 0) {
			if (enemyDir == 0) {
				enemyShotPos[0] -= 1;
				glDrawSprite(projectileleft, enemyShotPos[0], enemyShotPos[1], spriteSize[0], spriteSize[1]);
			}
			else if (enemyDir == 1) {
				enemyShotPos[0] += 1;
				glDrawSprite(projectileright, enemyShotPos[0], enemyShotPos[1], spriteSize[0], spriteSize[1]);
			}
		}

		//enemy projectile collision detection with background
		for (int i = 0; i < tilect; i++) {
			if (enemyShotPos[0] > bgcoll[i][0] - spriteSize[0]
				&& enemyShotPos[0] < bgcoll[i][1]
				&& enemyShotPos[1] > bgcoll[i][2] - spriteSize[1]
				&& enemyShotPos[1] < bgcoll[i][3]) {
				enemyShoot = 0;
			}
		}

		//check if there is collision with player
		if (spritePos[0] > enemyPos[0] - spriteSize[0] && spritePos[0] < enemyPos[0] + spriteSize[0]
			&& spritePos[1] > enemyPos[1] - spriteSize[1] && spritePos[1] < enemyPos[1] + spriteSize[1])
		{
			killed = 1;
		}

		//check if there is collision with projectile
		if (shotPos[0] > enemyPos[0] - spriteSize[0] && shotPos[0] < enemyPos[0] + spriteSize[0]
			&& shotPos[1] > enemyPos[1] - spriteSize[1] && shotPos[1] < enemyPos[1] + spriteSize[1] && killed != 1)
		{
			killed = 1;
			shoot = 0;
		}

		//starting position
		glDrawSprite(spriteTex, spritePos[0], spritePos[1], spriteSize[0], spriteSize[1]);

		/* Present to the player */
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();

	return 0;
}
