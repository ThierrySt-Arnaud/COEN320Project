/*
 * main.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#include <confname.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include "KeyboardListener.h"
#include "CommServer.h"
#include "RadarListener.h"
#include "Airspace.h"
#include "AirspaceDisplay.h"
#include <sstream>
#include <iostream>
using namespace std;

void ncursesSetup(int[]);

int main(){

	int screenSize[2];
	ncursesSetup(screenSize);

	CommServer commServer;
	Airspace airspace;
	KeyboardListener keyboardListener(&airspace, &commServer, screenSize);
	RadarListener radarListener(&airspace);
	AirspaceDisplay airspaceDisplay(&airspace, screenSize);

	airspaceDisplay.run();
	radarListener.run();
	commServer.run();
	pthread_join(keyboardListener.run(), NULL);
	airspaceDisplay.kill();
	endwin();
	fflush(stdout);
	commServer.kill();
	radarListener.kill();
	return 0;
}

void ncursesSetup(int screenSize[]){
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	curs_set(0);
	getmaxyx(stdscr, screenSize[0], screenSize[1]);
}
