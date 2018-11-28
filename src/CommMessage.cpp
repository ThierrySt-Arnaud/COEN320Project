/*
 * CommMessage.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#include "CommMessage.h"

CommMessage::CommMessage(CommMsgType type, std::string content, int remoteID = 0){
	this->type = type;
	this->remoteID = remoteID;
	this->content = content;
}

CommMsgType CommMessage::getType(){
	return this->type;
}

void CommMessage::setType(CommMsgType type){
	this->type = type;
}

int CommMessage::getRemoteID(){
	return this->remoteID;
}

void CommMessage::setRemoteID(int remoteID){
	this->remoteID = remoteID;
}

std::string CommMessage::getContent(){
	return this->content;
}

void CommMessage::setContent(std::string content){
	this->content = content;
}

