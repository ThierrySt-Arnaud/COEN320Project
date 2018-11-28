/*
 * IncomingEvent.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#include "OperatorCommand.h"

OperatorCommand::OperatorCommand(OpCmdType type){
	this->type = type;
}

OpCmdType OperatorCommand::getType(){
	return this->type;
}

void OperatorCommand::setType(OpCmdType type){
	this->type = type;
}
