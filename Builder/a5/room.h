/*

room.h - version 1.2
1.1 fix : added body to description() and cost() function in furnitture class
1.2 fix: added return statement to cost() function in room class
*/

#ifndef ROOM_H
#define ROOM_H
#include <string>
using namespace std;

class Furniture {
	string description_;
	double cost_;
public:
	Furniture(string desc, double c) {
		description_ = desc;
		cost_ = c;
	}
	virtual string description() const {
		return description_;
	}
	virtual double cost() const {
		return cost_;
	}
};

class Table :public Furniture {
public:
	Table(string desc, double c) :Furniture(desc, c) {}
	virtual string description() const {
		return "Table: " + Furniture::description();
	}
	virtual double cost() const {
		return Furniture::cost();
	}
};
class Chair :public Furniture {
public:
	Chair(string desc, double c) :Furniture(desc, c) {}
	virtual string description() const {
		return "chair: " + Furniture::description();
	}
	virtual double cost() const {
		return Furniture::cost();
	}
};
class Computer :public Furniture {
	int tableID_;   //id of table computer is added to
public:
	Computer(string desc, double c, int tableID) :Furniture(desc, c) {
		tableID_ = tableID;
	}
	virtual string description() const {
		return "computer: " + Furniture::description();
	}
	virtual double cost() const {
		//50 more per computer for installation, networking cost
		return Furniture::cost() + 50;
	}
};
class Room {
	Furniture* furniture_[200];
	int numTables_;
	int numChairs_;
	int numComputers_;
	int numFurniture_;
public:
	Room() {
		numTables_ = 0;
		numChairs_ = 0;
		numFurniture_ = 0;
		numComputers_ = 0;
	}
	int numChairs() const { return numChairs_; }
	int numTables() const { return numTables_; }
	int numComputers() const { return numComputers_; }
	double cost() const {
		double cost = 0;
		for (int i = 0; i<numFurniture_; i++) {
			cost += furniture_[i]->cost();
		}
		return cost;
	}
	void addChair(string desc, double cost) {
		numChairs_++;
		furniture_[numFurniture_] = new Chair(desc, cost);
		numFurniture_++;
	}
	void addTable(string desc, double cost) {
		numTables_++;
		furniture_[numFurniture_] = new Table(desc, cost);
		numFurniture_++;
	}
	void addComputer(string desc, double cost, int tableID = 0) {
		numComputers_++;
		furniture_[numFurniture_] = new Computer(desc, cost, tableID);
		numFurniture_++;
	}
};
#endif