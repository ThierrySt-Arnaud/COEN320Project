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
#include <string>
#include <vector>
#include <sstream>
#include "CommServer.h"
#include "KeyboardListener.h"

void ncursesSetup(int[]);

int main(){
	int screenSize[2];
	ncursesSetup(screenSize);
	WINDOW* msg_win;
	msg_win = newwin(1, screenSize[0]-1, screenSize[1], 0);

	CommServer commServer;
	Airspace airspace;
	KeyboardListener keyboardListener(&airspace, &commServer, msg_win, screenSize);

	commServer.run();
	const pthread_t* input_thread = keyboardListener.run();

	std::stringstream test2content;
	test2content << 1000 << ' ' << 1000 << ' ' << 1000;

	CommMessage test1 = CommMessage(REPORT, " ", 0);
	CommMessage test2 = CommMessage(SPDREQUEST, test2content.str(), 10);
	CommMessage test3 = CommMessage(HANDOFF, " ", 10);

	commServer.send(test1);
	commServer.send(test2);
	commServer.send(test3);

	pthread_join(*input_thread, nullptr);
}

void ncursesSetup(int screenSize[]){
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	curs_set(0);
	getmaxyx(stdscr, screenSize[0], screenSize[1]);
}
