#include "state.h"

StateA::~StateA() {
	if (_instance) {
		delete _instance;
	}
}

void StateA::actionOne(IContext* context) {
	context->setState(StateB::getInstance());
}

void StateA::actionTwo(IContext* context) {
	context->setState(StateA::getInstance());
}

void StateA::actionThree(IContext* context) {
	context->setState(StateD::getInstance());
}

states StateA::state() const {
	return states::A;
}

StateA* StateA::getInstance() {
	if (!_instance) {
		_instance = new StateA();
	}

	return _instance;
}

StateA* StateA::_instance = nullptr;

StateB::~StateB() {
	if (_instance) {
		delete _instance;
	}
}

void StateB::actionOne(IContext* context) {
	context->setState(StateC::getInstance());
}

void StateB::actionTwo(IContext* context) {
	context->setState(StateA::getInstance());
}

void StateB::actionThree(IContext* context) {
	context->setState(StateD::getInstance());
}

states StateB::state() const {
	return states::B;
}

StateB* StateB::getInstance() {
	if (!_instance) {
		_instance = new StateB();
	}

	return _instance;
}

StateB* StateB::_instance = nullptr;

StateC::~StateC() {
	if (_instance) {
		delete _instance;
	}
}

void StateC::actionOne(IContext* context) {
	context->setState(StateB::getInstance());
}

void StateC::actionTwo(IContext* context) {
	context->setState(StateA::getInstance());
}

void StateC::actionThree(IContext* context) {
	context->setState(StateD::getInstance());
}

states StateC::state() const {
	return states::C;
}

StateC* StateC::getInstance() {
	if (!_instance) {
		_instance = new StateC();
	}

	return _instance;
}

StateC* StateC::_instance = nullptr;

StateD::~StateD() {
	if (_instance) {
		delete _instance;
	}
}

void StateD::actionOne(IContext* context) {
	context->setState(StateA::getInstance());
}

void StateD::actionTwo(IContext* context) {
	context->setState(StateC::getInstance());
}

void StateD::actionThree(IContext* context) {
	context->setState(StateD::getInstance());
}

states StateD::state() const {
	return states::D;
}

StateD* StateD::getInstance() {
	if (!_instance) {
		_instance = new StateD();
	}

	return _instance;
}

StateD* StateD::_instance = nullptr;