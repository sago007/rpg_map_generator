#include "SagoMapGenerator.hpp"
#include <random>


sago::mapgenerator::SagoMapGenerator::SagoMapGenerator() : SagoMapGenerator(100, 100) {
}

sago::mapgenerator::SagoMapGenerator::SagoMapGenerator(int sizeX, int sizeY) {
	rooms.clear();
	worldSizeX = sizeX;
	worldSizeY = sizeY;
	world = std::vector<char>(sizeX*sizeY);
	FillAll();
}

void sago::mapgenerator::SagoMapGenerator::FillAll(char standardTile) {
	for (size_t i = 0; i < world.size(); ++i) {
		world[i] = standardTile;
	}
}

void sago::mapgenerator::SagoMapGenerator::placeRoom() {
	int w = minRoomSize + std::rand() % (maxRoomSize - minRoomSize + 1);
	int h = minRoomSize + std::rand() % (maxRoomSize - minRoomSize + 1);
	int x = std::rand() % (worldSizeX - w - 1) + 1;
	int y = std::rand() % (worldSizeY - h - 1) + 1;
	Room room;
	room.upperLeft.set_value(x, y);
	room.lowerRight.set_value(x+w, y+h);
	for (const Room& otherRoom : rooms) {
		if (room.intersects(otherRoom)) {
			return;
		}
	}
	rooms.push_back(room);
}

void sago::mapgenerator::SagoMapGenerator::carveRooms() {
	for (const Room& room : rooms) {
		for (int i = room.upperLeft.x; i <= room.lowerRight.x; ++i) {
			for (int j = room.upperLeft.y; j <= room.lowerRight.y; ++j) {
				world.at(i*worldSizeY+j) = ' ';
			}
		}
	}
}

void sago::mapgenerator::SagoMapGenerator::placeRooms() {
	for (int i=0; i < maxNumberOfRooms; ++i) {
		placeRoom();
	}
}