#pragma once
#include "fsm.h"

class IContext;
enum states { A, B, C, D };

class IState {
public:
	virtual ~IState() {}

	virtual void actionOne(IContext* context) {}
	virtual void actionTwo(IContext* context) {}
	virtual void actionThree(IContext* context) {}

	virtual states state() const = 0;
};

// StateA

class StateA : public IState {
	static StateA* _instance;

	StateA() {}
public:
	~StateA();

	void actionOne(IContext* context);

	void actionTwo(IContext* context);

	void actionThree(IContext* context);

	states state() const;

	static StateA* getInstance();
};

// StateB

class StateB : public IState {
	static StateB* _instance;

	StateB() {}
public:
	~StateB();

	void actionOne(IContext* context);

	void actionTwo(IContext* context);

	void actionThree(IContext* context);

	states state() const;

	static StateB* getInstance();
};

// StateC

class StateC : public IState {
	static StateC* _instance;

	StateC() {}
public:
	~StateC();

	void actionOne(IContext* context);

	void actionTwo(IContext* context);

	void actionThree(IContext* context);

	states state() const;

	static StateC* getInstance();
};

// StateD

class StateD : public IState {
	static StateD* _instance;

	StateD() {}
public:
	~StateD();

	void actionOne(IContext* context);

	void actionTwo(IContext* context);

	void actionThree(IContext* context);

	states state() const;

	static StateD* getInstance();
};
