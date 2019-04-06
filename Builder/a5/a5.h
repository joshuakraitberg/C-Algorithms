#pragma once

#include "room.h"


class Builder {
public:
	virtual ~Builder() {}
	virtual Room* buildRoom(int size) const = 0;
};

class Director {
	Builder* _builder;
public:
	Director();
	Room* buildRoom(int size) const;
	void setBuilder(Builder& builder);
};

class ALCBuilder : public Builder {
public:
	Room* buildRoom(int size) const;
};

class LabBuilder : public Builder{
	Room* buildRoom(int size) const;
};

class StepRoomBuilder : public Builder {
	Room* buildRoom(int size) const;
};
