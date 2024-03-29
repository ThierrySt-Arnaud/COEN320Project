/*
 * COMMServer.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef COMMSERVER_H_
#define COMMSERVER_H_

#include <fstream>
#include <pthread.h>
#include <atomic>
#include <queue>
#include "CommMessage.h"

#define COMM_OUT "/home/qnxuser/ATC/CommOut.txt"
#define LOG_OUT "/home/qnxuser/ATC/Log.txt"

typedef void * (*COMMSERVER_FUNC_PTR)(void *);


class CommServer{
public:
	CommServer(pthread_attr_t* = nullptr);
	CommServer(const CommServer&) = delete;
	virtual ~CommServer();
	pthread_t run();
	void send(CommMessage);
	void kill();
private:
	time_t clk;
	pthread_attr_t* threadAttr;
	pthread_t commServer = false;
	std::queue<CommMessage> outgoingQueue;
	std::ofstream commOut;
	std::ofstream log;
	std::atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void *commSender(void *);
	void printReport(int ID);
	void printSpdRequest(int ID, std::string content);
	void printAltRequest(int ID, std::string content);
	void printHandoff(int ID);
	void printLog(std::string content);
	std::string timeStamp();
};

#endif /* COMMSERVER_H_ */
