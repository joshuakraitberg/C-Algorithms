#include "room.h"
#include "a5.h"

#include <fstream>
#include <iostream>
using namespace std;

int main(void) {
	Director designer;
	ALCBuilder ALC;
	LabBuilder Lab;
	StepRoomBuilder Step;

	Room* theRoom;
	int choice;
	int numStudents;
	cout << "What type of classroom would you like to build?" << endl;
	do {
		cout << "1) Active Learning Classroom" << endl;
		cout << "2) Computer Lab" << endl;
		cout << "3) Step Classroom" << endl;
		cin >> choice;

	} while (choice < 1 || choice > 3);
	switch (choice) {
	case 1:
		designer.setBuilder(ALC);
		break;
	case 2:
		designer.setBuilder(Lab);
		break;
	case 3:
		designer.setBuilder(Step);
		break;
	}
	cout << "How many students does this room hold?" << endl;
	cin >> numStudents;
	theRoom = designer.buildRoom(numStudents);
	cout << choice << " " << numStudents << endl;
	cout << "Room summary: " << endl;
	cout << "Number of tables: " << theRoom->numTables() << endl;
	cout << "Number of Chairs: " << theRoom->numChairs() << endl;
	cout << "Number of Computers: " << theRoom->numComputers() << endl;
	cout << "Cost: " << theRoom->cost() << endl;
}