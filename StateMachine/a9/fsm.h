#pragma once
#include "state.h"

class IState;
enum states;

class IContext {
public:
	virtual ~IContext() {}

	virtual void setState(IState* state) = 0;

	virtual IState* getState() const = 0;
};

class FSMachine : public IContext {
	IState* _state;
public:
	FSMachine();

	~FSMachine() {}

	void setState(IState* state);

	IState* getState() const;

	void actionOne();

	void actionTwo();

	void actionThree();

	states state() const;
};