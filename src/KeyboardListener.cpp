/*
 * KeyboardListener.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include <sstream>
#include <string>
#include "KeyboardListener.h"
#include <ncurses.h>

KeyboardListener::KeyboardListener(Airspace* airspace, CommServer* commServer, int screenSize[], pthread_attr_t* threadAttr){
	this->airspace = airspace;
	this->commserver = commServer;
	this->threadAttr = threadAttr;
	memcpy(this->screenSize, screenSize, sizeof(this->screenSize));
	this->input_win = newwin(3, this->screenSize[1], this->screenSize[0]-4, 0);
}

KeyboardListener::~KeyboardListener() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

pthread_t KeyboardListener::run(){
	if (pthread_create(&kbListener, threadAttr, (KBLISTENER_FUNC_PTR) &KeyboardListener::keyboardListener, this) == 0){
		return kbListener;
	} else
		return -1;
}

void* KeyboardListener::keyboardListener(void*){
	int c;
	mvwprintw(input_win, 0, 0 , OPERATORCOMMANDS);
	while(!killFlag){
		c = wgetch(input_win);
		wmove(input_win, 2, 0);
		wclrtoeol(input_win);
		wrefresh(input_win);
		switch(c){
			case ERR:
				break;
			case 'a':
				altitude_change();
				break;
			case 's':
				speed_change();
				break;
			case 'd':
				request_report();
				break;
			case 'q':
				killFlag = true;
				break;
			default:
				mvwprintw(input_win, 2, 0, "Invalid command: '%c'", c);
				wrefresh(input_win);
				break;
		}
	}
	wclear(input_win);
	wrefresh(input_win);
	wclear(stdscr);
	wrefresh(stdscr);
	delwin(input_win);
	return nullptr;
}

void KeyboardListener::altitude_change(){
	readyInputWindow();
	int ID;
	int parsed = 0;
	do {
		wprintw(input_win, "Enter plane ID: ");
		parsed = wscanw(input_win,"%d", &ID);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);

		// TODO Check ID against Airspace
	} while (parsed == 0 && !wprintw(input_win, "Invalid ID! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	int y, x;
	mvwprintw(input_win, 2, 0, "New altitude for plane #%i:", ID);
	wrefresh(input_win);
	getyx(input_win, y, x);

	parsed = 0;
	int newAlt;
	wmove(input_win, 1, 0);
	do {
		wprintw(input_win, "Enter new altitude: ");
		parsed = wscanw(input_win,"%u", &newAlt);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);

		// TODO Check for out-of-bounds
	} while (parsed == 0 && !wprintw(input_win, "Invalid Altitude! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, y, x, " %i", newAlt);
	wrefresh(input_win);

	if (confirm()){
		wprintw(input_win, "Sending altitude change request to airplane #%i", ID);
		std::stringstream content;
		content << newAlt;
		CommMessage altChangeMsg = CommMessage(ALTREQUEST, content.str(), ID);
		commserver->send(altChangeMsg);

		airspace->chgAircraftAlt(ID, newAlt);
		std::stringstream altChangeLog;
		altChangeLog << "Sending altitude change request to airplane #" << ID;
		CommMessage logMsg = CommMessage(LOG, altChangeLog.str(), 0);
		commserver->send(logMsg);
	} else {
		wprintw(input_win, "Request canceled");
	}

	wrefresh(input_win);
}

void KeyboardListener::speed_change(){
	readyInputWindow();

	int ID;
	int parsed = 0;
	do {
		wprintw(input_win, "Enter plane ID: ");
		parsed = wscanw(input_win,"%d", &ID);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);

		// TODO Check ID vs. airspace
	} while (parsed == 0 && !wprintw(input_win, "Invalid ID! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, 2, 0, "New velocity for plane #%i:{Vx,Vy,Vz}={ ", ID);
	wrefresh(input_win);
	int y, x;
	getyx(input_win, y, x);

	parsed = 0;
	int newVX;
	wmove(input_win, 1, 0);
	do {
		wprintw(input_win, "Enter new X velocity: ");
		parsed = wscanw(input_win,"%d", &newVX);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);

		// TODO Check speed for ridiculous values
	} while (parsed == 0 && !wprintw(input_win, "Invalid speed! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, y, x, " %i, ", newVX);
	wrefresh(input_win);
	getyx(input_win, y, x);

	int newVY;
	wmove(input_win, 1, 0);
	do {
		wprintw(input_win, "Enter new Y velocity: ");
		parsed = wscanw(input_win,"%d", &newVY);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed == 0 && !wprintw(input_win, "Invalid speed! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, y, x, " %i, ", newVY);
	wrefresh(input_win);
	getyx(input_win, y, x);

	int newVZ;
	wmove(input_win, 1, 0);
	do {
		wprintw(input_win, "Enter new Z velocity: ");
		parsed = wscanw(input_win,"%d", &newVZ);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed == 0 && !wprintw(input_win, "Invalid speed! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, y, x, " %i, ", newVZ);
	wrefresh(input_win);

	if (confirm()){
		wprintw(input_win, "Sending speed change request to airplane #%i", ID);
		std::stringstream content;
		content << newVX << ' ' << newVY << ' ' << newVZ;
		CommMessage spdChangeMsg = CommMessage(SPDREQUEST, content.str(), ID);
		commserver->send(spdChangeMsg);

		int newSpd[3] = { newVX, newVY, newVZ };
		airspace->chgAircraftSpd(ID, newSpd);

		std::stringstream spdChangeLog;
		spdChangeLog << "Sending speed change request to airplane #" << ID;
		CommMessage logMsg = CommMessage(LOG, spdChangeLog.str(), 0);
		commserver->send(logMsg);
	} else {
		wprintw(input_win, "Request canceled");
	}
	wrefresh(input_win);
}

void KeyboardListener::request_report(){
	readyInputWindow();

	int ID;
	int parsed = 0;
	do {
		wprintw(input_win, "Enter plane ID: ");
		parsed = wscanw(input_win,"%d", &ID);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);

		// TODO Check ID vs. airspace
	} while (parsed == 0 && !wprintw(input_win, "Invalid ID! "));

	if (parsed == ERR){
		inputTimeout();
		return;
	}

	mvwprintw(input_win, 2, 0, "Requesting status report from plane #%i", ID);
	wrefresh(input_win);

	if (confirm()){

		wprintw(input_win, "Sending report request to airplane #%i", ID);
		CommMessage reportMsg = CommMessage(REPORT, " ", ID);
		commserver->send(reportMsg);

		std::stringstream report;
		report << "Sending report request to airplane #" << ID;
		CommMessage logMsg = CommMessage(LOG, report.str(), 0);
		commserver->send(logMsg);
	} else {
		wprintw(input_win, "Request canceled");
	}
	wrefresh(input_win);
}

void KeyboardListener::kill(){
	killFlag = true;
}

void KeyboardListener::readyInputWindow(){
	wmove(input_win, 1, 0);
	wclrtoeol(input_win);
	curs_set(2);
	raw();
	echo();
	wtimeout(input_win, 5000);
}

void KeyboardListener::inputTimeout(){
	wmove(input_win, 1 ,0);
	wclrtoeol(input_win);
	wmove(input_win, 2, 0);
	wclrtoeol(input_win);
	wrefresh(input_win);
	cbreak();
	curs_set(0);
	noecho();
}


bool KeyboardListener::confirm(){
	wmove(input_win, 1, 0);
	wclrtoeol(input_win);
	cbreak();
	curs_set(0);
	noecho();
	wprintw(input_win, "Confirm request? y/n ");
	int conf;
	conf = wgetch(input_win);
	wmove(input_win, 1, 0);
	wclrtoeol(input_win);
	wmove(input_win, 2, 0);
	wclrtoeol(input_win);
	if (conf == 'y'){
		return true;
	} else {
		return false;
	}
}
