/*
 * KeyboardListener.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef KEYBOARDLISTENER_H_
#define KEYBOARDLISTENER_H_

#include <pthread.h>
#include <atomic>
#include <ncurses.h>
#include "Airspace.h"
#include "CommServer.h"
#include "CommMessage.h"

typedef void * (*KBLISTENER_FUNC_PTR)(void *);

const char OPERATORCOMMANDS[] = "a - Altitude change, s - Speed change, d - Report status, q - Quit";

class KeyboardListener {
public:
	KeyboardListener(Airspace*, CommServer*, int[], pthread_attr_t* = nullptr);
	virtual ~KeyboardListener();
	pthread_t run();
	void kill();
private:
	WINDOW* input_win;
	pthread_t kbListener = false;
	pthread_attr_t* threadAttr;
	Airspace* airspace;
	CommServer* commserver;
	int screenSize[2];
	std::atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void* keyboardListener(void*);
	void altitude_change();
	void speed_change();
	void request_report();
	void readyInputWindow();
	void inputTimeout();
	bool confirm();
};

#endif /* KEYBOARDLISTENER_H_ */
