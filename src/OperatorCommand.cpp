/*
 * IncomingEvent.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#include "OperatorCommand.h"

OperatorCommand::OperatorCommand(OpCmdType type, std::array<int, 4> content){
	this->type = type;
	this->content = content;
}

OpCmdType OperatorCommand::getType(){
	return this->type;
}

void OperatorCommand::setType(OpCmdType type){
	this->type = type;
}

OpCmdType OperatorCommand::getContent(){
	return this->content;
}

void OperatorCommand::setContent(OpCmdType type){
	this->content = content;
}
