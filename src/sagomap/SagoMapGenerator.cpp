#include "SagoMapGenerator.hpp"
#include <random>
#include <cmath>
#include <limits>


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

void sago::mapgenerator::SagoMapGenerator::carvePath(Point from, Point to) {
	int x = from.x;
	int y = from.y;

	// Carve horizontal corridor first
	while (x != to.x) {
		if (x < to.x) {
			x++;
		} else {
			x--;
		}
		if (x >= 0 && x < worldSizeX && y >= 0 && y < worldSizeY) {
			world.at(x*worldSizeY+y) = ' ';
		}
	}

	// Then carve vertical corridor
	while (y != to.y) {
		if (y < to.y) {
			y++;
		} else {
			y--;
		}
		if (x >= 0 && x < worldSizeX && y >= 0 && y < worldSizeY) {
			world.at(x*worldSizeY+y) = ' ';
		}
	}
}

bool sago::mapgenerator::SagoMapGenerator::areRoomsConnected(size_t roomIndex1, size_t roomIndex2, const std::vector<std::vector<bool>>& connected) const {
	std::vector<bool> visited(rooms.size(), false);
	std::vector<size_t> stack;

	stack.push_back(roomIndex1);
	visited[roomIndex1] = true;

	while (!stack.empty()) {
		size_t current = stack.back();
		stack.pop_back();

		if (current == roomIndex2) {
			return true;
		}

		for (size_t i = 0; i < rooms.size(); ++i) {
			if (!visited[i] && connected[current][i]) {
				visited[i] = true;
				stack.push_back(i);
			}
		}
	}

	return false;
}

double sago::mapgenerator::SagoMapGenerator::getDistanceBetweenRooms(const Room& room1, const Room& room2) const {
	Point center1 = room1.getMiddle();
	Point center2 = room2.getMiddle();
	double dx = center2.x - center1.x;
	double dy = center2.y - center1.y;
	return std::sqrt(dx * dx + dy * dy);
}

void sago::mapgenerator::SagoMapGenerator::generatePaths() {
	std::vector<std::vector<bool>> connected(rooms.size(), std::vector<bool>(rooms.size(), false));

	// Initialize connectivity for touching rooms
	for (size_t i = 0; i < rooms.size(); ++i) {
		connected[i][i] = true;
		for (size_t j = i + 1; j < rooms.size(); ++j) {
			if (rooms[i].isTouching(rooms[j])) {
				connected[i][j] = true;
				connected[j][i] = true;
			}
		}
	}

	// Connect each room to its closest unconnected room
	for (size_t i = 0; i < rooms.size(); ++i) {
		double minDistance = std::numeric_limits<double>::max();
		size_t closestRoom = SIZE_MAX;

		// Find the closest room that is not connected
		for (size_t j = 0; j < rooms.size(); ++j) {
			if (i != j && !rooms[i].isTouching(rooms[j]) && !areRoomsConnected(i, j, connected)) {
				double distance = getDistanceBetweenRooms(rooms[i], rooms[j]);
				if (distance < minDistance) {
					minDistance = distance;
					closestRoom = j;
				}
			}
		}

		// Connect to the closest room if one was found
		if (closestRoom != SIZE_MAX) {
			Point center1 = rooms[i].getMiddle();
			Point center2 = rooms[closestRoom].getMiddle();
			carvePath(center1, center2);
			connected[i][closestRoom] = true;
			connected[closestRoom][i] = true;
		}
	}
}