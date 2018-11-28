/*
 * EventMessage.h
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#ifndef OPERATORCOMMAND_H_
#define OPERATORCOMMAND_H_

enum OpCmdType {  ALT_CHG, SPD_CHG, DIR_CHG, TOG_HOLD, PING };

class OperatorCommand{
public:
	OpCmdType type;
	OperatorCommand(OpCmdType);
	OpCmdType getType();
	void setType(OpCmdType);
};

#endif /* OPERATORCOMMAND_H_ */
