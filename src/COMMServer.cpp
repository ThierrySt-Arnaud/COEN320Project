/*
 * COMMServer.cpp
 *
 *  Created on: Nov 24, 2018
 *      Author: thierry
 */

#include "CommServer.h"
#include <sstream>
#include <time.h>

CommServer::CommServer(pthread_attr_t* threadAttr):
commOut(COMM_OUT, std::fstream::out | std::fstream::trunc),
log(LCOMM_OUT, std::fstream::out | std::fstream::trunc ){
	this->threadAttr = threadAttr;
	this->outgoingQueue = std::queue<CommMessage>();
	commOut <<  timeStamp() << "Communication channel open" << std::endl;
	log << timeStamp() << "Log file open" << std::endl
	this->clk = time(Null);
}

CommServer::~CommServer(){
	this->kill();
	pthread_attr_destroy(threadAttr);
	this->commOut.close();
	this->log.close();
}

pthread_t CommServer::run(){
	if (pthread_create(&commServer, threadAttr, (COMMSERVER_FUNC_PTR) &CommServer::commSender, this) == 0){
		return commServer;
	} else
		return -1;
}

void CommServer::send(CommMessage message){
	pthread_sleepon_lock();
	outgoingQueue.push(message);
	pthread_sleepon_signal(&outgoingQueue);
	pthread_sleepon_unlock();
}

void *CommServer::commSender(void *){
	commOut <<  timeStamp() << "Output thread ready" << std::endl;
	while (!killFlag){
		pthread_sleepon_lock();
		while (!killFlag && outgoingQueue.empty()){
			pthread_sleepon_wait(&outgoingQueue);
		}

		if (killFlag){
			break;
		}

		CommMessage message = outgoingQueue.front();
		outgoingQueue.pop();
		pthread_sleepon_unlock();
		CommMsgType type = message.getType();
		int remoteID = message.getRemoteID();
		std::string content = message.getContent();

		switch(type) {
			case REPORT:
				printReport(remoteID);
				break;
			case SPDREQUEST:
				printSpdRequest(remoteID, content);
				break;
			case ALTREQUEST:
				printAltRequest(remoteID, content);
				break;
			case HANDOFF:
				printHandoff(remoteID);
				break;
			case LOG:
				printLog(content);
				break;
			default:
				commOut << "Invalid Message type!";
		}
		commOut << std::endl;
	}

	//When asked to quit, empty the outgoing queue
	while (!outgoingQueue.empty()){
		CommMessage message = outgoingQueue.front();
		outgoingQueue.pop();
		CommMsgType type = message.getType();
		int remoteID = message.getRemoteID();
		std::string content = message.getContent();

		switch(type) {
			case REPORT:
				printReport(remoteID);
				break;
			case SPDREQUEST:
				printSpdRequest(remoteID, content);
				break;
			case ALTREQUEST:
				printAltRequest(remoteID, content);
				break;
			case HANDOFF:
				printHandoff(remoteID);
				break;
			case LOG:
				printLog(content);
				break;
			default:
				commOut << "Invalid Message type!";
		}
	}
	const char* tStamp =  timeStamp();
	commOut <<  tStamp << "Communication output terminated" << std::endl;
	log <<  tStamp << "Log terminated" << std::endl;
	pthread_sleepon_unlock();
	return nullptr;
}

void CommServer::printReport(int ID){
	commOut << timeStamp();

	commOut << "Report request sent to ";
	if (ID < 0){
		commOut << "unidentified plane " << abs(ID) << "." ;
	} else if (ID > 0){
		commOut << "airplane " << ID << "." ;
	} else {
		commOut << "all airplanes in airspace.";
	}
	commOut << std::endl;
}

void CommServer::printSpdRequest(int ID, std::string content){
	commOut << timeStamp();

	std::stringstream parsingBuffer = std::stringstream(content, std::iostream::in);
	int vx, vy, vz;
	parsingBuffer >> vx >> vy >> vz;

	if (ID < 0){
		commOut << "Requesting unidentified plane " << abs(ID) << " change speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz;
	} else if (ID > 0){
		commOut << "Requesting airplane " << ID << " change speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz;
	} else {
		commOut << "Requesting all planes change speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz;
	}
	commOut << std::endl;
}

void CommServer::printAltRequest(int ID, std::string content){
	commOut << timeStamp();

	std::stringstream parsingBuffer = std::stringstream(content, std::iostream::in);
	int z ;
	parsingBuffer >> z;

	if (ID < 0){
		commOut << "Requesting unidentified plane " << abs(ID) << " changes to altitude to: " << z << " feet.";
	} else if (ID > 0){
		commOut << "Requesting airplane " << ID << " changes to altitude to: " << z << " feet.";
	} else {
		commOut << "Requesting all planes changes to altitude to:" << z << " feet.";
	}
	commOut << std::endl;
}

void CommServer::printHandoff(int ID){
	commOut << timeStamp();

	if (ID < 0){
		commOut << "Unidentified plane " << abs(ID) << " is leaving the airspace." ;
	} else if (ID > 0){
		commOut << "Airplane " << ID << " is leaving the airspace." ;
	} else {
		commOut << "All planes in the airspace are leaving!";
	}
	commOut << std::endl;
}

void CommServer::printLog(std::string content){
	log << timeStamp() << content << std::endl;
}

const char* CommServer::timeStamp(){
	char timeStamp[26];
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime(timeStamp, sizeof(timeStamp),"%D %T - ",timeinfo);
	return timeStamp;
}

void CommServer::kill(){
	killFlag = true;
	pthread_sleepon_lock();
	pthread_sleepon_signal(&outgoingQueue);
	pthread_sleepon_unlock();
	pthread_join(commServer, NULL);
}
