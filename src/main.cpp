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
#include <stdio.h>
#include "CommServer.h"
#include "KeyboardListener.h"

void ncursesSetup(int[]);

int main(){
	int screenSize[2];
	ncursesSetup(screenSize);

	CommServer commServer;
	Airspace airspace;
	KeyboardListener keyboardListener(&airspace, &commServer, screenSize);

	commServer.run();
	pthread_join(keyboardListener.run(), NULL);
	endwin();
	fflush(stdout);
	printf("Waiting for commServer to close.\n");
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
