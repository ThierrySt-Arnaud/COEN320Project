/*
 * KeyboardListener.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef KEYBOARDLISTENER_H_
#define KEYBOARDLISTENER_H_

#include <queue>
#include <pthread.h>
#include <fstream>
#include "OperatorCommand.h"
#include "Airspace.h"

class KeyboardListener {
public:

//	KeyboardListener(Airspace, pthread_attr_t* = nullptr);
//	virtual ~KeyboardListener();
	const pthread_t* run();
	bool kill();
private:
	pthread_mutex_t commandMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t kbListener, cmdInterpreter;
//	std::queue<OperatorCommand> commandQueue;
	void* keyboardListener(void*);
	void* commandInterpreter(void*);
};

#endif /* KEYBOARDLISTENER_H_ */
