/*
 * KeyboardListener.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include <sstream>
#include "KeyboardListener.h"
#include <ncurses.h>

KeyboardListener::KeyboardListener(Airspace* airspace, CommServer* commServer, WINDOW* msg_win, int screenSize[], pthread_attr_t* threadAttr){
	this->msg_win = msg_win;
	this->airspace = airspace;
	this->commserver = commServer;
	this->threadAttr = threadAttr;
	memcpy(this->screenSize, screenSize, sizeof(this->screenSize));
	this->input_win = newwin(2, screenSize[1], screenSize[0]-3, 0);
}

KeyboardListener::~KeyboardListener() {
	this->kill();
	pthread_attr_destroy(threadAttr);
	delwin(input_win);
}

const pthread_t* KeyboardListener::run(){
	if (pthread_create(&kbListener, threadAttr, (KBLISTENER_FUNC_PTR) &KeyboardListener::keyboardListener, this) == 0){
		return &kbListener;
	} else
		return nullptr;
}

void* KeyboardListener::keyboardListener(void*){
	int c;
	mvwprintw(input_win, 0, 0 , OPERATORCOMMANDS);
	while(!killFlag){
		c = wgetch(input_win);
		wclear(msg_win);
		wrefresh(msg_win);
		switch(c){
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
				break;
			default:
				mvwprintw(msg_win, 0, 0, "Invalid command: '%c'", c);
				wrefresh(msg_win);
				break;
		}
		if(c == 'q')	/* User asked to quit, come out of the infinite loop */
			break;
	}
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
	} while (parsed != 1 && !wprintw(input_win, "Invalid ID! "));

	wprintw(msg_win, "New altitude for plane #%i:", ID);
	wrefresh(msg_win);

	parsed = 0;
	int newAlt;
	do {
		wprintw(input_win, "Enter new altitude: ");
		parsed = wscanw(input_win,"%u", &newAlt);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed != 1 && !wprintw(input_win, "Invalid Altitude! "));

	wprintw(msg_win, " %i", newAlt);
	wrefresh(msg_win);
	if (confirm()){
		std::stringstream content;
		content << newAlt;
		CommMessage altChangeMsg = CommMessage(ALTREQUEST, content.str(), ID);
		commserver->send(altChangeMsg);
	}
	wclear(msg_win);
	wrefresh(msg_win);
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
	} while (parsed != 1 && !wprintw(input_win, "Invalid ID! "));

	wprintw(msg_win, "New velocity for plane #%i:{Vx,Vy,Vz}={ ", ID);
	wrefresh(msg_win);

	parsed = 0;
	int newVX;
	do {
		wprintw(input_win, "Enter new X velocity: ");
		parsed = wscanw(input_win,"%d", &newVX);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed != 1 && !wprintw(input_win, "Invalid speed! "));

	wprintw(msg_win, " %i, ", newVX);
	wrefresh(msg_win);

	int newVY;
	do {
		wprintw(input_win, "Enter new Y velocity: ");
		parsed = wscanw(input_win,"%d", &newVY);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed != 1 && !wprintw(input_win, "Invalid speed! "));

	wprintw(msg_win, " %i, ", newVY);
	wrefresh(msg_win);

	int newVZ;
	do {
		wprintw(input_win, "Enter new Z velocity: ");
		parsed = wscanw(input_win,"%d", &newVZ);
		wmove(input_win, 1,0);
		wclrtoeol(input_win);
	} while (parsed != 1 && !wprintw(input_win, "Invalid speed! "));

	wprintw(msg_win, " %i, ", newVZ);
	wrefresh(msg_win);

	if (confirm()){
		std::stringstream content;
		content << newVX << ' ' << newVY << ' ' << newVZ;
		CommMessage spdChangeMsg = CommMessage(SPDREQUEST, content.str(), ID);
		commserver->send(spdChangeMsg);
	}
	wclear(msg_win);
	wrefresh(msg_win);
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
	} while (parsed != 1 && !wprintw(input_win, "Invalid ID! "));

	wprintw(msg_win, "Requesting status report from plane #%i", ID);
	wrefresh(msg_win);

	if (confirm()){
		CommMessage reportMsg = CommMessage(REPORT, " ", ID);
		commserver->send(reportMsg);
	}
	wclear(msg_win);
	wrefresh(msg_win);
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
}

bool KeyboardListener::confirm(){
	wmove(input_win, 1,0);
	wclrtoeol(input_win);
	cbreak();
	curs_set(0);
	noecho();
	wprintw(input_win, "Confirm request? y/n ");
	int conf = wgetch(input_win);
	if (conf == 'y'){
		return true;
	} else {
		return false;
	}
}
