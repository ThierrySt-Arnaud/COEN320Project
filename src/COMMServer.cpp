/*
 * COMMServer.cpp
 *
 *  Created on: Nov 24, 2018
 *      Author: thierry
 */

#include "CommServer.h"
#include <sstream>

CommServer::CommServer(pthread_attr_t* threadAttr): commOut(COMM_OUT, std::fstream::out | std::fstream::trunc){
	this->threadAttr = threadAttr;
	this->outgoingQueue = std::queue<CommMessage>();
}

CommServer::~CommServer(){
	this->kill();
	pthread_attr_destroy(threadAttr);
	this->commOut.close();
}

const pthread_t* CommServer::run(){
	if (pthread_create(&commServer, threadAttr, (COMMSERVER_FUNC_PTR) &CommServer::commSender, this) == 0){
		return &commServer;
	} else
		return nullptr;
}

void CommServer::send(CommMessage message){
	pthread_sleepon_lock();
	outgoingQueue.push(message);
	pthread_sleepon_signal(&outgoingQueue);
	pthread_sleepon_unlock();
}

void *CommServer::commSender(void *){
	while (!killFlag){
		pthread_sleepon_lock();
		while (outgoingQueue.empty()){
			pthread_sleepon_wait(&outgoingQueue);
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
			case REQUEST:
				printRequest(remoteID, content);
				break;
			case HANDOFF:
				printHandoff(remoteID);
				break;
			default:
				commOut << "Invalid Message type!";
		}
		commOut << '\n';
	}

	//When asked to quit, empty the outgoing queue
	pthread_sleepon_lock();
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
			case REQUEST:
				printRequest(remoteID, content);
				break;
			case HANDOFF:
				printHandoff(remoteID);
				break;
			default:
				commOut << "Invalid Message type!";
		}
		commOut << '\n';
	}
	commOut << "Communication output terminated" << std::endl;
	pthread_sleepon_unlock();
	return nullptr;
}

void CommServer::printReport(int ID){
	commOut << "Report request sent to ";
	if (ID < 0){
		commOut << "unidentified plane " << abs(ID) << "." ;
	} else if (ID > 0){
		commOut << "airplane " << ID << "." ;
	} else {
		commOut << "all airplanes in airspace.";
	}
}

void CommServer::printRequest(int ID, std::string content){
	std::stringstream parsingBuffer = std::stringstream(content, std::iostream::in);
	int vx, vy, vz, z ;
	parsingBuffer >> vx >> vy >> vz >> z;

	if (ID < 0){
		commOut << "Requesting unidentified plane " << abs(ID) << " changes to speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz << " and reach altitude " << z;
	} else if (ID > 0){
		commOut << "Requesting airplane " << ID << " changes to speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz << " and reach altitude " << z;
	} else {
		commOut << "Requesting all planes changes to speed to:\n"
				<< "VX: " << vx << " VY: " << vy << " VZ: " << vz << " and reach altitude " << z;
	}
}

void CommServer::printHandoff(int ID){
	if (ID < 0){
		commOut << "Unidentified plane " << abs(ID) << " is leaving the airspace." ;
	} else if (ID > 0){
		commOut << "Airplane " << ID << " is leaving the airspace." ;
	} else {
		commOut << "All planes in the airspace are leaving!";
	}
}

void CommServer::kill(){
	killFlag = true;
	pthread_sleepon_signal(&outgoingQueue);
	pthread_join(commServer, NULL);
}
