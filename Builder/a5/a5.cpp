#include "a5.h"

Director::Director() : _builder(nullptr) {}

void Director::setBuilder(Builder & builder) {
	_builder = &builder;
}

Room* Director::buildRoom(int size) const {
	return (_builder) ? _builder->buildRoom(size) : nullptr;
}

Room* ALCBuilder::buildRoom(int size) const {
	Room* room = new Room();

	if (size > 0) {

		// calculate tables
		int nTables = (size + 2) / 3;
		nTables = (nTables & 1) ? nTables + 1 : nTables;

		// add tables to room
		for (auto i = 0u; i < nTables; ++i) {
			room->addTable("Half Hex Table", 600);
		}

		// add chairs to room
		for (auto i = 0u; i < size; ++i) {
			room->addChair("Low Back Plastic with Wheels", 100); // This is why tuition is so high
		}
	}

	return room;
}

Room* LabBuilder::buildRoom(int size) const {
	Room* room = new Room();

	room->addChair("Instructor Stool", 80); // Obligatory stool

	if (size > 0) {

		// calculate tables
		int nTables = (size + 1) / 2;

		// add tables to room
		for (auto i = 0u; i < nTables; ++i) {
			room->addTable("2 Station Computer Desk", 500);
		}

		// add computers to room
		for (auto i = 0u; i < size; ++i) {
			room->addTable("Mac Mini", 1000);
		}

		// add chairs to room
		for (auto i = 1u; i < size; ++i) {
			room->addChair("Student Desk Chair", 130);
		}
	}

	return room;
}

Room* StepRoomBuilder::buildRoom(int size) const {
	Room* room = new Room();

	room->addTable("Instructor Table", 200);
	room->addChair("Instructor Stool", 80);
	room->addChair("Regular Chair", 70);

	if (size > 0) {

		// calculate tables
		int nTables = (size + 7) / 8;
		int nChairs = 7 + (nTables - 1) * 8;


		// add tables to room
		for (auto i = 0u; i < nTables; ++i) {
			room->addTable("8 Student Long Table", 1000);
		}

		// add chairs to room
		for (auto i = 0u; i < nChairs; ++i) {
			room->addChair("Padded Lecture Theater Chair", 150); // These are really cheap why not just use these everywhere?
		}
	}

	return room;
}
