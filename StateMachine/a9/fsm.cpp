#include "fsm.h"

FSMachine::FSMachine() {
	_state = StateA::getInstance();
}

void FSMachine::setState(IState * state) {
	_state = state;
}

IState * FSMachine::getState() const {
	return _state;
}

void FSMachine::actionOne() {
	_state->actionOne(this);
}

void FSMachine::actionTwo() {
	_state->actionTwo(this);
}

void FSMachine::actionThree() {
	_state->actionThree(this);
}

states FSMachine::state() const {
	return  _state->state();
}
